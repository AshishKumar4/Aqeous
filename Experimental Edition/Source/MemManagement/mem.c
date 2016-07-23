#include "mem.h"
#include "paging.h"
#include "vmem.h"
#include "memfunc.h"

uint32_t processID=10; ///id 4 for paging, rest reserved

inline void PhyMap_unSet(uint32_t addr)
{
    uint32_t* ptr = (uint32_t*)(BITMAP_LOCATION);
    addr /= 4096;
    ptr += addr;
    *ptr = 0;
}

inline uint32_t Phy_alloc(uint32_t purpose) ///Gives frames for pages
{
    uint32_t* ptr=(uint32_t*)(BITMAP_LOCATION);
    ptr+=6144;
    for(int i=6144;i<1024*1024;i++)  ///start allocation from beyond 24MB point
    {
      if(!*ptr)
      {
        *ptr=purpose+1;
    //    printf("\nPhy Frame: %x %x",i,tmp->addr);
        return i; ///return the frame
      }
      ++ptr;
    }
    printf("\nNo Physical Memory space left!!!!!");
    return 0;
}

inline uint32_t Phy_alloc_pg(uint32_t processID) ///Gives frames for page structures
{
    uint32_t* ptr=(uint32_t*)(BITMAP_LOCATION);
    ptr+=25*1024;
    for(int i=25*1024;i<1024*1024;i++)  ///start allocation from beyond 100MB point
    {
      if(!*ptr)
      {
        MemMap_t* tmp=(MemMap_t*)BlockFinder(i*4096);
        tmp->id=processID+1;
        tmp->used=4096;
        *ptr=processID;
      //  printf("\nPhy Frame: %x %x",i,tmp->addr);
        return i; ///return the frame
      }
      ++ptr;
    }
    printf("\nNo Physical Memory space left!!!!!");
    return 0;
}

uint32_t kmalloc_int(uint32_t sz, int align)
{
  uint32_t volatile sz4;
  if(!sz%4)
  {
    sz4=sz/4;
  }
  else
  {
    sz+=(4-(sz%4));
    sz4=sz/4;
    //sz4++;
    //sz=sz*4;
  }
  ptable* table;
  page_t* page;
  MemMap_t* block;
  uint32_t phy_mem=0;
  uint32_t virt_addr=0;
  if(!sz4)//if size is less then 32 bytes, roundof it to 32 which is the least memory allocable
  {
    sz=4;
    sz4=1;
  }

  if(sz>=4096||align) ///4096-32 as 32 is the least allocable size
  {
    uint32_t pgs=sz/4096;
    uint32_t tsz4=sz%4096;
 //   printf("\ntsz4: %x sz: %x pgs: %x", tsz4, sz, pgs);
    if(tsz4)
    {
      if(tsz4 < 4)
        tsz4 = 1;
      tsz4 /= 4;
    }
    uint32_t temp=0;
    for(uint32_t i=20;i<1024;i++)
    {
      table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[i]);
      if(!(system_dir->m_entries[i] & CUSTOM_PDE_AVAIL_2))
      {
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          block=BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
          if(!block->used)
          {
            temp++;
            if(temp==pgs)
            {
              virt_addr=(i*4096*1024)+((j-temp+1)*4096); ///i=table offset, j=page offset
              if(j+1 >= temp)
              {
                 //printf("\nA");
                 //printf(" i=%x, j=%x, temp=%x tsz4=%x pgs=%x", i, j, temp, tsz4, pgs);
                 page=&table->m_entries[j-temp+1];
                 phy_mem = virt_addr;
                 for(uint32_t k=0;k<temp;k++)
                 {
                //   printf("\nABCD @#@#");
                   //pt_entry_set_frame ( page, phy_mem);
                   *page |= CUSTOM_PTE_AVAIL_1 | CUSTOM_PTE_AVAIL_2;
                   block=BlockFinder(phy_mem);
                   block->page=page;
                   block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                   block->used=4096;
                   ++block;
                   ++page;
                   phy_mem+=4096;
                 }
                 if(tsz4)
                 {
                   phy_mem+=4096;
                    *page |= CUSTOM_PTE_AVAIL_1;
                   block=BlockFinder(phy_mem);
                   block->page=page;
                   block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                   block->used=(tsz4+1)*4;
                   uint32_t* strip = (uint32_t*)(phy_mem + (tsz4*4));
                   *strip = 42847 | (tsz4 << 16);
                   strip = (uint32_t*)(phy_mem + 4092);
                   *strip = (1&0xffff) | (42847 << 16);
                 }
              }
              else
              {
                 printf("\nB");
              //    printf("\n Hello");
                 //printf(" i=%x, j=%x, temp=%x tsz4=%x pgs=%x", i, j, temp, tsz4, pgs);
                 //while(1);
                //
                // block=BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
                 if(tsz4)
                 {
                    //printf(" Ab");
                   // while(1);
                   phy_mem = PAGE_GET_PHYSICAL_ADDRESS(page);
                   *page |= CUSTOM_PTE_AVAIL_1;
                   block->page=page;
                   block->id=(1 & 0xff) | ((blockID&0xff)<<8);
                   block->used=(tsz4+1)*4;
                   uint32_t* strip = (uint32_t*)(phy_mem + (tsz4*4));
                   *strip = 42847 | (tsz4 << 16);
                   strip = (uint32_t*)(phy_mem + 4092);
                   *strip = (1&0xffff) | (42847 << 16);
                   --page;
                   --j;
                   --temp;
                 }
                 uint32_t tmp_var = j + 1;
                 for(uint32_t m = i + 1; m && temp ; --m)
                 {
                  //  printf(" xA");
                    for(uint32_t n = tmp_var; n && temp ; --n)
                    {
                      // printf(" xB");
                       *page |= CUSTOM_PTE_AVAIL_1 | CUSTOM_PTE_AVAIL_2;
                       block->page=page;
                       block->id=(1 & 0xff) | ((blockID&0xff)<<8);
                       block->used=4096;
                       --block;
                       --page;
                       --temp;
                    }
                   tmp_var = 1024;
                   table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[m-2]);
                   page=&table->m_entries[1023];
                   block=BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
                   system_dir->m_entries[m-2] |= CUSTOM_PTE_AVAIL_2;
                 }
                   //while(1);
                }
                blockID++;
                return virt_addr;
              }
            }
            else
            {
              temp=0;
            }
            ++page;
          }
        }
        else
        {
          temp=0;
        }
      }
    }
    else
    {
      int clrBlks=0;
      uint32_t phy_mem=0;
      for(uint32_t i=20; i<1024; i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          if(!(*page & CUSTOM_PTE_AVAIL_1)) //The whole page is unused, so its not been mapped yet
          {
            virt_addr = (i*4096*1024)+(j*4096);
            phy_mem = virt_addr;
            *page |= CUSTOM_PTE_AVAIL_1;
            block=BlockFinder(phy_mem);
            if(!block->used)
            {
               //printf("\nC");
              block->page=page;
              block->id=(curr_pgdir.ID & 0xff) | (1<<8);
              block->used=(sz4+1)*4;
              uint32_t* strip = (uint32_t*)(phy_mem + (sz4*4));
              *strip = (42847&0xFFFF) | (sz4 << 16);
              strip = (uint32_t*)(phy_mem + 4092);
              *strip = 1 | (42847 << 16);
              return virt_addr;
            }
          }
          else if(!(*page & CUSTOM_PTE_AVAIL_2))
          {
            virt_addr = (i*4096*1024)+(j*4096);
            phy_mem = virt_addr;
            block=BlockFinder(phy_mem);
            if(block->used<=(4092-(sz4*4)))
            {
              clrBlks=clearBlks((uint32_t*)phy_mem,sz4+1);
              if(clrBlks>=0)
              {
                uint32_t* strip = (uint32_t*)(phy_mem + (sz4*4) + (clrBlks*4));
                *strip = (42847&0xFFFF) | (sz4 << 16);
                uint16_t* last_strip = (uint16_t*)(phy_mem + 4092);
                ++*last_strip;
                block->used+=sz4*4;
                if(block->used > 4088) //block already full, no space
                {
                  pt_entry_add_attrib(page, CUSTOM_PTE_AVAIL_2);
                }
                  //printf("\nD");
                return virt_addr+(4*clrBlks);
              }
            }
          }
          ++page;
        }
      }
    }
    printf("\n Something went wrong");
    return 0;
}//*/

void kfree(uint32_t* ptr)
{
  uint32_t offset = ((uint32_t)ptr)%4096;
  uint32_t frame = ((uint32_t)ptr)/4096; ///Physical address
  uint32_t volatile frame_4k_aligned = frame*4096;
  uint32_t phy_addr = (uint32_t)ptr;//frame + offset;
  table_t* tbl = &system_dir->m_entries[frame/(1024)];
  ptable* table = (ptable*)PAGE_GET_PHYSICAL_ADDRESS(tbl);
  page_t* page = &table->m_entries[frame%1024];
  uint16_t* last_strip = (uint16_t*)(frame_4k_aligned + 4092);
  ++last_strip;
  MemMap_t* Block = BlockFinder(frame_4k_aligned);
  if(*last_strip == 42847) //the block isnt used solely for one allocation
  {
    strip_t* strip = (strip_t*)phy_addr;
    for(int i=0;;i++)
    {
      if(strip->magic == 42847)
      {
        memset_faster((uint32_t*)phy_addr,0,i+1);
        Block->used -= (i+1)*4;
        --last_strip;
        --*last_strip;
        pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
        if(!last_strip)
        {
          pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_1);
          pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
          ++last_strip;
          *last_strip = 0;
          Block->used = 0;
          Block->id = 0;
          Block->page = 0;
        }
        return;
      }
      ++strip;
    }
  }
  else
  {
    MemMap_t* blk = Block;
    uint32_t id = Block->id;
    for(; ;frame++)
    {
      if(blk->id == id && *last_strip != 42847)
      {
        memset_fast((void*)frame_4k_aligned, 0, 4096);
        pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_1);
        pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
        PhyMap_unSet(frame_4k_aligned);
        frame_4k_aligned+=4096;
        last_strip = (uint16_t*)(frame_4k_aligned + 4092);
        ++last_strip;
        blk->id = 0;
        blk->used = 0;
        blk->page = 0;
        ++blk;
        page = blk->page;
        tbl = &system_dir->m_entries[frame/(1024)];
        pt_entry_del_attrib(tbl,CUSTOM_PDE_AVAIL_2);
      }
      else if(blk->id == id)
      {
        phy_addr = frame_4k_aligned;
        strip_t* strip = (strip_t*)phy_addr;
        for(int i=0;;i++)
        {
          if(strip->magic == 42847)
          {
            memset_faster((uint32_t*)phy_addr,0,i+1);
            blk->used -= (i+1)*4;
            --last_strip;
            --*last_strip;
            pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
            if(!last_strip)
            {
              pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_1);
              pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
              ++last_strip;
              *last_strip = 0;
              blk->used = 0;
              blk->id = 0;
              blk->page = 0;
            }
            tbl = &system_dir->m_entries[frame/(1024)];
            pt_entry_del_attrib(tbl,CUSTOM_PDE_AVAIL_2);
            return;
          }
          ++strip;
        }
        tbl = &system_dir->m_entries[frame/(1024)];
        pt_entry_del_attrib(tbl,CUSTOM_PDE_AVAIL_2);
      }
      else
      {
        return;
      }
    }
  }
}//*/

inline uint32_t pmalloc(uint32_t id)
{
    //return kmalloc_int(sz, 1, 0,2,1,4);
    uint32_t mem=Phy_alloc_pg(id)*4096;
  //  if(pag)
  //    MapPage((void*)mem,(void*)mem);
   return mem;
}

uint32_t kmalloc_aligned(uint32_t sz)
{
    return kmalloc_int(sz, 1);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0);
}
const uint32_t mtalc_start = 220*1024*1024;
const uint32_t mtalc_end = 500*1024*1024;

uint32_t mtalloc(uint32_t pages)
{
   uint32_t tmp = 0;
   ptable* table;
   page_t* page;
   table_t* table_entry = &system_dir->m_entries[mtalc_start/(1024*4096)];
   for(int i=0; i<71 ; i++)
   {
      table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(table_entry);
      page=&table->m_entries[0];
      for(int j=0; j<1024 ; j++)
      {
         if(!(*page & CUSTOM_PTE_AVAIL_1))
         {
            ++tmp;
            if(pages == tmp)
            {
               pt_entry_add_attrib( page, CUSTOM_PTE_AVAIL_1);
               return (PAGE_GET_PHYSICAL_ADDRESS(page));
               //TODO: Return the address.
            }
         }
         else tmp = 0;
         ++page;
      }
      ++table_entry;
   }
   _printf("\nCould not find memory, sorry. Try kmalloc()");
   return 0;
   //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
}

/*
uint32_t mtalloc(uint32_t pages)
{
   uint32_t tmp = 0;
   ptable* table;
   page_t* page;
   MemMap_t* block;
   table_t* table_entry = &system_dir->m_entries[mtalc_start/(1024*4096)];
   for(int i=0; i<71 ; i++)
   {
      table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(table_entry);
      page=&table->m_entries[0];
      block = BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
      for(int j=0; j<1024 ; j++)
      {
         if(!block->used)
         {
            ++tmp;
            if(pages == tmp)
            {
               block->page=page;
               block->id=(1 & 0xff) | ((blockID&0xff)<<8);
               block->used=4096;
               pt_entry_add_attrib( page, CUSTOM_PTE_AVAIL_1);
               //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
               return (PAGE_GET_PHYSICAL_ADDRESS(page));
               //TODO: Return the address.
            }
         }
         else tmp = 0;
         ++page;
         ++block;
      }
      ++table_entry;
   }
   _printf("\nCould not find memory, sorry. Try kmalloc()");
   return 0;
   //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
}
*/
void mtfree(uint32_t addr, uint32_t size)
{
   ptable* table_sys;
   page_t* page_sys;
   ptable* table;
   page_t* page;
   addr /= 4096;
   table_sys=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[mtalc_start/(1024)]);
   page_sys=&table_sys->m_entries[addr%1024];
   table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&_cur_directory->m_entries[mtalc_start/(1024)]);
   page=&table->m_entries[addr%1024];
   for(;size;size--)
   {
      pt_entry_del_attrib( page_sys, CUSTOM_PTE_AVAIL_1);
      *page = 0;
      ++page_sys;
      ++page;
   }
}

uint32_t fsalloc(uint32_t sz)
{
  uint32_t addr=kmalloc_int(sz, 0);
  //printf(" Ab%x",addr);
  return addr;
}
