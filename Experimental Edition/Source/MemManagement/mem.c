#include "mem.h"
#include "paging.h"
#include "vmem.h"
// end is defined in the linker script.
extern page_directory_t *kernel_directory;

uint32_t BlockFinder(uint32_t addr)
{
    return (addr/4096)+(uint32_t)Mblock;
}

uint32_t processID=10; //id 4 for paging, rest reserved

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys,int kernel,int packed,int processId)
{
    uint32_t mb=0;
    if(kernel) mb=40;
    else mb=100;
    tempBlock1=Kblock;
    uint32_t addr;
    for(int i=0;;i++)
    {
        addr=tempBlock1->addr;
        if(addr>=mb*1024*1024) //If the block is in user memory
        {
          out:
            if(tempBlock1->used&&packed==0)
            {
                if (align == 1 && (addr & 0xFFFFF000)) //if alignment required
                {
                  in2:
                  addr&=0xFFFFF000; //get an aligned address.
                  addr+=0x1000;
                  while(addr<=tempBlock1->addr) //check if the aligned address is in some block.
                  {
                      ++tempBlock1;
                  }
                  if(tempBlock1->used==0) //if that block is unused
                  {
                    break;
                  }
                  else if(tempBlock1->map + sz +16 + (addr-tempBlock1->addr) <4096)
                  {
                      break; //if that memory is unused, return the address.
                  }
                  else ++tempBlock1;
                  goto in2; //else search for some more such blocks
                }
                if(tempBlock1->map + sz +16 <4096) //check if Block has enough space, keep 16B for buffer
                {
                  addr+=tempBlock1->map +16;
                  break; //the block is fit to accomodate the data.
                }
                else
                {
                  ++tempBlock1;
                  addr=tempBlock1->addr;
                  goto out;
                }
            }
            if(tempBlock1->used==0)
            {
              if (align == 1 && (addr & 0xFFFFF000)) //if alignment required
              {
                in:
                addr&=0xFFFFF000; //get an aligned address.
                addr+=0x1000;
                while(addr<=tempBlock1->addr) //check if the aligned address is in some block.
                {
                    ++tempBlock1;
                }
                if(tempBlock1->used==0)
                {
                    break; //if that block is unused, return the address.
                }
                else goto in; //else search for some more such blocks
            }
            else
            {
                break; //used
            }
          }
        }
        else if(tempBlock1->addr>=maxmem*1024)
        {
            printf("\nNo memory");
            return 0;
        }
        ++tempBlock1;
    }
    if(phys)
        *phys=addr;
    uint32_t size=sz;
    if(pag==0) //If paging not enabled
        for(uint32_t i=0;i<=(sz-1)/4096;i++,tempBlock1++,size-=4096)
        {
          if(!processId)
            tempBlock1->used=processID;
          else tempBlock1->used=processId;
          tempBlock1->map+=size;
        }
    else
    {
        for(uint32_t i=0;i<=(sz-1)/4096;i++,tempBlock1++,size-=4096)
        {
            if(!processId)
              tempBlock1->used=processID;
            else tempBlock1->used=processId;
            tempBlock1->map+=(size);
            page_t* page=(page_t*)tempBlock1->page;
            page->present=1;
            page->rw=1;
            page->user=1;
            page->frame=tempBlock1->addr/4096;
        }
        switch_page_directory(kernel_directory);
    }
    ++processID;
    return addr;
}

void kfree(uint32_t p)
{
    tempBlock1=(MemMap_t*)BlockFinder(p);
    uint32_t id=tempBlock1->used;
    if(!id)
      return; //Block Already Free
    if(id<=3) //memory is reserved?
    {
      printf("\nCant free reserved memory!!!\n");
      return;
    }
    while(tempBlock1->used==id) //Destroy all the blocks with the same processID
    {
      memset((void*)tempBlock1->addr,0,4096); //clear the memory it points to
      tempBlock1->used=0;
      page_t *page=(page_t*)tempBlock1->page;
      page->frame=1024*1024*70/4096; //destroy the pages
      ++tempBlock1;
    }
    switch_page_directory(kernel_directory); //re enable paging
    printf("\nMemory Freed\n");
}

uint32_t kmalloc_a(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0,1,0,0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys,1,0,0);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,1,0,0);
}

uint32_t pmalloc(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,1,1,4);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,1,0,0);
}

uint32_t malloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,0,0,0);
}
