#include "vmem.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "mem.h"
#include "paging.h"

inline uint32_t BlockFinder(uint32_t addr) /// returns the corresponding memory block for a given physical address
{
    MemMap_t *Block;
    addr/=4096;
    Block=(MemMap_t*)((uint32_t)Mblock + sizeof(MemMap_t)*(addr));
    Block++; //there was an offset in blocks from the start; first block is at Mblock+sizeof(MemMap_t)
    return (uint32_t)Block;
}

void Mapper()
{
  ///it can be that 14th mb to 16th be memory mapped ISA hardware. So better not to use it.
    Mblock=(MemMap_t*)(0x1000000); ///start of the memory map linked list from 16th mb from kernel base.
    StartFrame=(FrameMap_t*)(0x1000);
    MemMap_t* tempBlock2=Mblock;
    uint32_t i=0;
    MemMap_t NullBlock;
    uint16_t* ptr;
    memset((void*)&NullBlock,0,sizeof(MemMap_t));
    for(i=0;i<(1024);i++) //Make blocks!!!
    {
        for(uint32_t j=0;j<1024;j++)
        {
            tempBlock2++;
            *tempBlock2=NullBlock;
            //memset((void*)tempBlock2,0,sizeof(MemMap_t));
            tempBlock2->addr=(j+(i*1024))*4096;
            if(i>maxmem/4096)
            {
              tempBlock2->used=4096;
              ptr=&tempBlock2->id;
              ptr++;
              *ptr=1;
            }
            else if(mmap_info)
            {
              tempBlock2->used=(mmap_info->type-1)*4096;
              ptr=&tempBlock2->id;
              ptr++;
              *ptr=mmap_info->type-1;
            }
            else
            {
              tempBlock2->used=4096;
              ptr=&tempBlock2->id;
              ptr++;
              *ptr=1;
            }
            if(tempBlock2->id)
            {
              tempBlock2->map[0]=0xFFFFFFFF;
              tempBlock2->map[1]=0xFFFFFFFF;
              tempBlock2->map[2]=0xFFFFFFFF;
              tempBlock2->map[3]=0xFFFFFFFF;
            }
            uint32_t add=((j+(i*1024))*4096);
            if(add>=(mmap_info->startHi+mmap_info->sizeHi))
            {
                 ++mmap_info;
            }
        }
        //printf(" ");
    }
    lastBlock=tempBlock2;
    tempBlock2=(MemMap_t*)BlockFinder((uint32_t)Mblock);
    while(tempBlock2->addr<=(uint32_t)lastBlock)
    {
        tempBlock2->used=1;
        tempBlock2->map[0]=0xFFFFFFFF;
        tempBlock2->map[1]=0xFFFFFFFF;
        tempBlock2->map[2]=0xFFFFFFFF;
        tempBlock2->map[3]=0xFFFFFFFF;
        lastBlockAddr=tempBlock2;
        tempBlock2++;
    }
    tempBlock2=Mblock;
    Kblock=(MemMap_t*)(lastBlockAddr+sizeof(mblock));
}

void bitmap_init()
{
    uint32_t base=8388608; //8th mb to 12mb
    uint32_t *ptr=(uint32_t*)base;
    MemMap_t* tmp=Mblock;
    for(int i=0;i<1024*1024;i++)
    {
      if(i<13*1024) ///used
        *ptr=0xFFFFFFFF;
      else if(tmp->used)
      {
        *ptr=tmp->addr;
      }
      else
        *ptr=0;
      ptr++;
    }
}

uint32_t VMem_Alloc(uint32_t sz, int align, uint32_t *phys,int purpose,int packed,int processId)
{
    pdirectory* dir=_cur_directory;
    Switch_to_system_dir();
    uint32_t sz4096=sz/4096;
    uint32_t sz32=sz/32;
    table_t* entry;
    ptable* table;
    page_t* page;
    MemMap_t* block;
    uint32_t phy_mem=0;
    if(!sz32)//if size is less then 32 bytes, roundof it to 32 which is the least memory allocable
    {
      sz=33;
      sz32=1;
    }

    uint32_t tsz32=(sz%4096)/32;
    if(sz>=4096||align) ///4096-32 as 32 is the least allocable size
    {
      uint32_t pgs=sz/4096;
      pgs++;
      uint32_t temp=0;
      for(uint32_t i=2;i<1024;i++)
      {
        entry = get_table(i,1,dir);
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(entry);
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          if(!(*page & CUSTOM_PTE_AVAIL_1))
          {
            temp++;
          }
          else
          {
            temp=0;
          }
          if(temp>=pgs)
          {
            page=&table->m_entries[j-temp+1];
            uint32_t* pg=page;
            uint32_t virt_addr=(i*4096*1024)+(j*4096); ///i=table offset, j=page offset
            for(uint32_t k=0;k<temp-1;k++)
            {
              phy_mem=Phy_alloc(2)*4096;
              pt_entry_set_frame ( page, phy_mem);
              pt_entry_add_attrib ( page, I86_PTE_PRESENT);
              pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
              pt_entry_add_attrib ( page, I86_PTE_USER);
              pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_1);
              pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_2);
              block=(MemMap_t*)BlockFinder(phy_mem);
              block->page=page;
              block->id=curr_pgdir.ID;
              uint16_t* up=&block->id;
              up++;
              *up=blockID;
              blockID++;
              block->used=4096;
              block->map[0]=0xFFFFFFFF; //set the bits
              block->map[1]=0xFFFFFFFF;
              block->map[2]=0xFFFFFFFF;
              block->map[3]=0xFFFFFFFF;
              ++block;
              ++page;
            }
            phy_mem=Phy_alloc(2)*4096;
            pt_entry_set_frame ( page, phy_mem);
            pt_entry_add_attrib ( page, I86_PTE_PRESENT);
            pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
            pt_entry_add_attrib ( page, I86_PTE_USER);
            pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_1);
            if((sz%4096)>=2048)
            {
              pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_2);
            }
            block=(MemMap_t*)BlockFinder(phy_mem);
            block->page=page;
            block->id=curr_pgdir.ID;
            uint16_t* up=&block->id;
            up++;
            *up=blockID;
            blockID++;
            block->used=tsz32*32;
            for(uint32_t k=0; k<tsz32;k++)
            {
              block->map[k/32]|=(1<<(k%32)); ///turn the remaining bits.
            }
            page=pg;
            Switch_back_from_System();
            return virt_addr;
          }
        }
      }
    }
    else
    {
      uint32_t sz2048=sz/2048;
      int clrBits=0;
      uint32_t phy_mem=0;
      for(uint32_t i=2; i<4096; i++)
      {
        entry = get_table(i,1,dir);
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(entry);
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          if(!(*page & CUSTOM_PTE_AVAIL_1)) //The whole page is unused, so its not been mapped yet
          {
            phy_mem=Phy_alloc(2)*4096;
            pt_entry_set_frame ( page, phy_mem);
            pt_entry_add_attrib ( page, I86_PTE_PRESENT);
            pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
            pt_entry_add_attrib ( page, I86_PTE_USER);
            pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_1);
            block=(MemMap_t*)BlockFinder(phy_mem);
            block->page=page;
            block->id=curr_pgdir.ID;
            uint16_t* up=&block->id;
            up++;
            *up=1;
            block->used=sz32*32;
            for(uint32_t k=0; k<sz32;k++)
            {
              block->map[k/32]|=(1<<(k%32)); ///turn the remaining bits.
            }
            Switch_back_from_System();
            return (i*4096*1024)+(j*4096);
          }
          else if(!(*page & CUSTOM_PTE_AVAIL_2))
          {
            phy_mem=pt_get_frame(page);
            block=(MemMap_t*)BlockFinder(phy_mem);
            if(block->used<=(4064-(sz32*32)))
            {
              clrBits=clearBits(block->map,sz);
              if(clrBits>=0)
              {
                for(uint32_t k=clrBits; k<sz32+clrBits;k++)
                {
                  block->map[k/32]|=(1<<(k%32)); ///turn the remaining bits.
                }
                if(clearBits(block->map,64)<0) //block already full, no space
                {
                  pt_entry_add_attrib(page, CUSTOM_PTE_AVAIL_2);
                }
                block->used+=sz32*32;
                Switch_back_from_System();
                return ((i*4096*1024)+(j*4096))+(32*clrBits);
              }
            }
          }
        }
      }
    }
}

void free(uint32_t* ptr)
{
  Switch_to_system_dir();
  uint32_t addr=(uint32_t)get_page(ptr,0,_prev_directory);
  addr=pt_get_frame((page_t*)addr);
  MemMap_t* Block=(MemMap_t*)BlockFinder(addr);
  uint32_t tsz=((uint32_t)ptr)%4096;
  addr+=tsz;
  uint32_t bitOff=tsz/32;
  if(!bitOff) bitOff=1;
  if(!(Block->map[3] & (1<<(31)))) //Check if the last bit is unset
  {
    for(int i=bitOff;;i++)
    {
      if(Block->map[i/32] & (1<<(i%32)))
      {
        Block->map[i/32]^=(1<<(i%32));
      }
      else
      {
        memset((void*)addr,0,(i-bitOff+1)*32);
        Block->used-=(i-bitOff+1)*32;
        break;
      }
    }
  }
  else //it can only happen if the block is solely for a single process
  {
    uint16_t* tmp=&Block->id;
    tmp++;
    uint32_t pid=*tmp;
    MemMap_t* tm=Block,*tm2=Block;
    tm2++;
    while(tm2->used==pid)
    {
      tm->used=0;
      tm->map[0]=0;
      tm->map[1]=0;
      tm->map[2]=0;
      tm->map[3]=0;
      tm->used=0;
      free_page(tm->page);
      ++tm;
      ++tm2;
    }
    for(uint32_t i=0;;i++)
    {
      if(Block->map[i/32] & (1<<(i%32)))
      {
        Block->map[i/32]^=(1<<(i%32));
      }
      else
      {
        ++i;
        memset((void*)addr,0,(i)*32);
        Block->used-=(i)*32;
        break;
      }
    }
  }
  if(!Block->used)
    {
      free_page(Block->page);
    }
    Switch_back_from_System();
}

uint32_t malloc(uint32_t sz)
{
    return VMem_Alloc(sz, 0, 0,0,0,0);
}
