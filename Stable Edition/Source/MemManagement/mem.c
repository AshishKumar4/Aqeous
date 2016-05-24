#include "mem.h"
#include "paging.h"
#include "vmem.h"

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
  /*
    uint32_t mb=0;   ///dont give kernel memory below this point
    if(purpose==1) mb=+400; //for kernel
    else if(purpose==3) mb+=52; //for tasking
    else if(purpose==4) mb+=300; //for tasking
    else if(purpose==5) mb+=200; //for filesystem
    else mb+=500;*/
    uint32_t volatile sz4;
    if(!sz%4)
    {
      sz4=sz/4;
    }
    else
    {
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
      if(tsz4)
      {
        if(tsz4 < 4)
          tsz4 = 1;
        tsz4 /= 4;
      }
      uint32_t temp=0;
      for(uint32_t i=8;i<1024;i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          block=BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
          if(!block->used)
          {
            temp++;
            if(temp==pgs)
            {
              page=&table->m_entries[j-temp+1];
              virt_addr=(i*4096*1024)+((j-temp+1)*4096); ///i=table offset, j=page offset
              phy_mem = virt_addr;
              for(uint32_t k=0;k<temp;k++)
              {
                phy_mem+=k*4096;
                //pt_entry_set_frame ( page, phy_mem);
                *page |= CUSTOM_PTE_AVAIL_1 | CUSTOM_PTE_AVAIL_2;
                block=BlockFinder(phy_mem);
                block->page=page;
                block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                block->used=4096;
                ++block;
                ++page;
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
    }
    else
    {
      int clrBlks=0;
      uint32_t phy_mem=0;
      for(uint32_t i=8; i<1024; i++)
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
                return virt_addr+(4*clrBlks);
              }
            }
          }
          ++page;
        }
      }
    }
}

void kfree(uint32_t* ptr)
{
  uint32_t offset = ((uint32_t)ptr)%4096;
  page_t* page = get_page((uint32_t)ptr,0,system_dir);
  uint32_t frame = PAGE_GET_PHYSICAL_ADDRESS(page); ///Physical address
  uint32_t phy_addr = frame + offset;
  uint16_t* last_strip = (uint16_t*)(frame + 4092);
  ++last_strip;
  MemMap_t* Block = BlockFinder(frame);
  if(*last_strip == 42847) //the block isnt used solely for one allocation
  {
    strip_t* strip = (strip_t*)phy_addr;
    for(int i=0;;i++)
    {
      if(strip->magic == 42847)
      {
        memset((void*)phy_addr,0,(i+1)*4);
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
    for(; ;)
    {
      if(blk->id == id && *last_strip != 42847)
      {
        memset((void*)frame, 0, 4096);
        pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_1);
        pt_entry_del_attrib(page,CUSTOM_PTE_AVAIL_2);
        PhyMap_unSet(frame);
        ++page;
        frame+=4096;
        last_strip = (uint16_t*)(frame + 4092);
        ++last_strip;
        blk->id = 0;
        blk->used = 0;
        blk->page = 0;
        ++blk;
      }
      else if(blk->id == id)
      {
        phy_addr = frame;
        strip_t* strip = (strip_t*)phy_addr;
        for(int i=0;;i++)
        {
          if(strip->magic == 42847)
          {
            memset((void*)phy_addr,0,(i+1)*4);
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
            return;
          }
          ++strip;
        }
      }
      else
      {
        return;
      }
    }
  }
}

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

uint32_t st = 210*1024*1024;

uint32_t* tmalloc(uint32_t sz) //for tasks(threads)
{
    uint32_t ab = st;
    st += sz;
    return (uint32_t*)ab;
    //return (uint32_t*)kmalloc_int(sz, 0);
}

uint32_t* smalloc(uint32_t sz) //for tasks(threads) Stakcs
{
    uint32_t ab = st;
    st += sz;
    return (uint32_t*)ab;
    //return kmalloc_int(sz, 0);
}

uint32_t fsalloc(uint32_t sz)
{
  uint32_t addr=kmalloc_int(sz, 0);
  //printf(" Ab%x",addr);
  return addr;
}
