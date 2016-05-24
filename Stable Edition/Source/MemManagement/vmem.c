#include "vmem.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "mem.h"
#include "paging.h"

inline MemMap_t* BlockFinder(uint32_t addr) /// returns the corresponding memory block for a given physical address
{
    MemMap_t *Block;
    addr/=4096;
    Block=(MemMap_t*)((uint32_t)Mblock + sizeof(MemMap_t)*(addr));
    Block++; //there was an offset in blocks from the start; first block is at Mblock+sizeof(MemMap_t)
    return Block;
}

inline uint32_t clearBlks(uint32_t* addr, uint32_t szBlks)
{
    uint_fast32_t counter = 0, strips = 0;
    uint16_t *last_strip = (uint16_t*)(addr + 1023);
    uint32_t tmp = 0;
    for(int i=0; i<1024; i++, ++addr)
    {
      if((*addr & 0xFFFF) == 42847) //Found a strip
      {
        ++strips;
        if(szBlks <= counter - (*addr >> 16)) //There is clear space not accounted
        {
          if(strips == 1)
            return tmp;
          else
          return tmp + 1;
        }
        tmp = i;
        counter = 0;
      }
      else if(strips == *last_strip && 1023-(uint32_t)i > szBlks)
      {
        return i;
      }
      else
      {
        ++counter;
      }
    }
    return 0;
}

void Mapper()
{
  ///it can be that 14th mb to 16th be memory mapped ISA hardware. So better not to use it.
    Mblock=(MemMap_t*)(0x1000000); ///start of the memory map linked list from 16th mb from kernel base.
    MemMap_t* tempBlock2=Mblock;
    uint32_t i=0;
    MemMap_t NullBlock;
    uint8_t* ptr;
    memset((void*)&NullBlock,0,sizeof(MemMap_t));
    for(i=0;i<(1024);i++) //Make blocks!!!
    {
        for(uint32_t j=0;j<1024;j++)
        {
            tempBlock2++;
            *tempBlock2=NullBlock;
            //memset((void*)tempBlock2,0,sizeof(MemMap_t));
          //  tempBlock2->addr=(j+(i*1024))*4096;
            if(i>maxmem/4096)
            {
              tempBlock2->used=4096;
              ptr=(uint8_t*)&tempBlock2->id;
              ptr++;
              *ptr=1;
            }
            else if(mmap_info)
            {
              tempBlock2->used=(mmap_info->type-1)*4096;
              ptr=(uint8_t*)&tempBlock2->id;
              ptr++;
              *ptr=mmap_info->type-1;
            }
            else
            {
              tempBlock2->used=4096;
              ptr=(uint8_t*)&tempBlock2->id;
              ptr++;
              *ptr=1;
            }/*
            if(tempBlock2->id)
            {
              tempBlock2->used=4096;
            }*/
            uint32_t add=((j+(i*1024))*4096);
            if(add>=(mmap_info->startHi+mmap_info->sizeHi))
            {
                 ++mmap_info;
            }
        }
        //printf(" ");
    }
    lastBlock=tempBlock2;
    tempBlock2=Mblock;
    for(uint32_t i=0; i<6*1024; i++)
    {
        tempBlock2->used=4096;
        tempBlock2++;
    }
    lastBlockAddr = (MemMap_t*)(24*1024*1024);
    Kblock=(MemMap_t*)(lastBlockAddr+sizeof(mblock));
    //printf(" a%x",BlockFinder(30*1024*1024)->used);
  //  while(1);
}

void bitmap_init()
{
    uint32_t base=8388608; //8th mb to 12mb
    uint32_t *ptr=(uint32_t*)base;
    MemMap_t* tmp=Mblock;
    for(int i=0;i<1024*1024;i++)
    {
      if(i<7*1024) ///used
      {
        *ptr=0xFFFFFFFF;
        tmp->used = 4096;
      }
      else if(i>12000&&i<15000)
      {
        *ptr=0xFFFFFFFF;
        tmp->used = 4096;
      }
      else if(i>125*1024&&i<127*1024)
      {
        *ptr=0xFFFFFFFF;
        tmp->used = 4096;
      }
      else if(tmp->used)
      {
        *ptr=tmp->id;
      }
      else
        *ptr=0;
      ptr++;
      tmp++;
    }
}

uint32_t VMem_Alloc(uint32_t sz, int align, int processId)
{
    pdirectory* dir=_cur_directory;
    Switch_to_system_dir();
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
    if(!sz4)//if size is less then 4 bytes, roundof it to 4 which is the least memory allocable
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
      for(uint32_t i=2;i<1024;i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          //page=&table->m_entries[j];
          if(!*page)//(!(*page & CUSTOM_PTE_AVAIL_1)
          {
            ++temp;
            if(temp>=pgs)
            {
              page=&table->m_entries[j-temp+1];
              uint32_t virt_addr=(i*4096*1024)+((j-temp+1)*4096); ///i=table offset, j=page offset
              for(uint32_t k=0;k<temp;k++)
              {
                phy_mem=Phy_alloc(processId)*4096;
                //pt_entry_set_frame ( page, phy_mem);
                *page = phy_mem | 1027 | CUSTOM_PTE_AVAIL_2;
                block=BlockFinder(phy_mem);
                block->page=page;
                block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                block->used=4096;
                ++block;
                ++page;
              }
              if(tsz4)
              {
                phy_mem=Phy_alloc(processId)*4096;
                *page = phy_mem | 1027;
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
              Switch_back_from_System();
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
      for(uint32_t i=2; i<1024; i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          if(!*page)//(!(*page & CUSTOM_PTE_AVAIL_1)) //The whole page is unused, so its not been mapped yet
          {
            phy_mem=Phy_alloc(processId)*4096;
            *page = phy_mem | 1027;
            block=BlockFinder(phy_mem);
            block->page=page;
            block->id=(curr_pgdir.ID & 0xff) | (1<<8);
            block->used=(sz4+1)*4;
            uint32_t* strip = (uint32_t*)(phy_mem + (sz4*4));
            *strip = (42847&0xFFFF) | (sz4 << 16);
            strip = (uint32_t*)(phy_mem + 4092);
            *strip = (1&0xffff) | (42847 << 16);
            Switch_back_from_System();
            return (i*4096*1024)+(j*4096);
          }
          else if(!(*page & CUSTOM_PTE_AVAIL_2))
          {
            phy_mem=pt_get_frame(page);
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
                Switch_back_from_System();
                return ((i*4096*1024)+(j*4096))+(4*clrBlks);
              }
            }
          }
          ++page;
        }
      }
    }
}

void free(uint32_t* ptr)
{
    Switch_to_system_dir();
    uint32_t offset = ((uint32_t)ptr)%4096;
    page_t* page = get_page((uint32_t)ptr,0,_prev_directory);
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
            *page = 0;
            ++last_strip;
            *last_strip = 0;
            Block->used = 0;
            Block->id = 0;
            Block->page = 0;
          }
          printf(" D");
          Switch_back_from_System();
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
          *page = 0;
          PhyMap_unSet(frame);
          ++page;
          frame = PAGE_GET_PHYSICAL_ADDRESS(page);
          last_strip = (uint16_t*)(frame + 4092);
          ++last_strip;
          blk->id = 0;
          blk->used = 0;
          blk->page = 0;
          blk = BlockFinder(frame);
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
                *page = 0;
                ++last_strip;
                *last_strip = 0;
                blk->used = 0;
                blk->id = 0;
                blk->page = 0;
              }
              printf(" d");
              Switch_back_from_System();
              return;
            }
            ++strip;
          }
        }
        else
        {
          Switch_back_from_System();
          return;
        }
      }
    }
}

uint32_t malloc(uint32_t sz)
{
    return VMem_Alloc(sz, 0, 2);
}
