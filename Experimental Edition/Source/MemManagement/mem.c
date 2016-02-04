#include "mem.h"
#include "paging.h"
#include "vmem.h"
// end is defined in the linker script.
extern page_directory_t *kernel_directory;

uint32_t BlockFinder(uint32_t addr)
{
    return (addr/4096)+(uint32_t)Mblock;
}

inline int clearBits(uint32_t map,uint32_t sz) //gives the output as number of continuous clear bits
{
  //uint32_t map=tmp->map;

  //sz/=32; //convert to index of 128 byte chunks

  uint8_t count=0;

  for(int i=0;i<32;i++)
  {
      if(count*128>=sz)
      {
        //printf(" 1 ");
        return i; //our job is done!
      }
      else if(!(map & (1<<i)))
      {
        count++;
      }
      else
      {
        count=0;
        i++;
      }
  }
  return 0;
}

uint32_t processID=10; //id 4 for paging, rest reserved

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys,int purpose,int packed,int processId)
{
    uint32_t mb=0;
    if(purpose==1) mb=40; //for kernel
    else if(purpose==2) mb=21;
    else mb=100;
    tempBlock1=Kblock;
    uint32_t addr;
    uint32_t sz4096=sz/4096;
    uint32_t sz128=sz/128;
    if(!sz128) sz128=1;
    uint32_t tsz128=(sz%4096)/128;
    while(1)
    {
        addr=tempBlock1->addr;
        if(addr>=(mb*1024*1024)+32) //If the block is in user memory; +32 for buffer
        {
          out:
          if(tempBlock1->used==0)
          {
            in:
              if (align == 1 && (addr & 0xFFFFF000)) //if alignment required
              {
                addr&=0xFFFFF000; //get an aligned address.
                addr+=0x1000;
                while(addr<=tempBlock1->addr) //check if the aligned address is in some block.
                {
                    ++tempBlock1;
                }
              }
              if(tempBlock1->used==0) //if that block is unused, return the address.
              {
                MemMap_t* tm=tempBlock1;
                if(sz>4096)
                {
                  for(uint32_t i=0;i<sz4096+1;i++) //check if adjacent blocks are empty too!
                  {
                    ++tm;
                    if(tm->used) //cant fit here!
                    {
                      tempBlock1=tm;
                      //printf(" 1");
                      goto out;
                    }
                  }
                  tm=tempBlock1;
                  for(uint32_t j=0;j<sz4096;j++)
                  {
                    //for(uint32_t i=0;i<4096;i++)
                    tm->map=0xFFFFFFFF;//|=(1<<(i/128)); //set the bit;
                    ++tm;
                  }
                  //sz%=4096;
                  for(uint32_t i=0; i<tsz128;i++)
                  {
                    tm->map|=(1<<(i));
                  }
                  addr=tempBlock1->addr;
                  break;
                }
                MemMap_t* tm2=tempBlock1;
                //If you reach here, you are lucky!
                for(uint32_t i=0; i<sz128;i++)
                {
                  tm2->map|=(1<<(i));
                }
                //just get out now!
                addr=tempBlock1->addr;
                //tempBlock1=tm2;
                break;
              }
              else
              {
                //++tempBlock1;
                goto in;
              }
            }
            else if(!packed) //if packing required (0=required, 1=not)
            {
              if(sz<4096-10) //if memory can be accomodated in single block.
              {
                int tm;
                out2:
                if(tempBlock1->map==0xFFFFFFFF) //block already full, no space
                {
                  ++tempBlock1;
                  goto out;
                }
                tm=clearBits(tempBlock1->map,sz);
                if(tm) //if enough memory is in the region
                {
                  tm-=sz128;
                  //tm++;
                  for(uint32_t i=tm;i<sz128+tm;i++)
                    tempBlock1->map|=(1<<(i)); //set the bit;
                  addr=tempBlock1->addr + (128*tm);
                  break;
                }
                else
                { 
                  //printf(" 1 ");
                  ++tempBlock1; //search in some other block for the same
                  goto out2;
                }
              }
              else
              {
                out3:
                while(tempBlock1->used)
                  ++tempBlock1; //find a completely empty block
                MemMap_t* tm=tempBlock1;
                for(uint32_t i=0;i<sz4096+1;i++) //check if adjacent blocks are empty too!
                {
                  ++tm;
                  if(tm->used) //cant fit here!
                  {
                    tempBlock1=tm;
                    goto out3;
                  }
                }
                MemMap_t* tm2=tempBlock1;
                //If you reach here, you are lucky!
                for(uint32_t j=0;j<sz4096;j++)
                {
                    tempBlock1->map=0xFFFFFFFF;// //set the whole block as used
                  ++tempBlock1;
                }
                for(uint32_t i=0; i<tsz128;i++)
                {
                  tm->map|=(1<<(i));
                }
                //just get out now!
                addr=tm2->addr;
                tempBlock1=tm2;
                break;
              }
            }
            if(tempBlock1->addr>=maxmem*1024)
            {
                printf("\nNo memory");
                return 0;
            }
          }
        ++tempBlock1;
    }
    final:
    if(phys)
        *phys=addr;
    uint32_t size=sz;
    uint32_t tid=processID;
    if(pag==0) //If paging not enabled
        for(uint32_t i=0;i<=(sz-1)/4096;i++,tempBlock1++,size-=4096)
        {
          if(!processId)
            tempBlock1->used=tid;
          else tempBlock1->used=processId;
          //tempBlock1->map+=size;
        }
    else
    {
        for(uint32_t i=0;i<=(sz-1)/4096;i++,tempBlock1++,size-=4096)
        {
            if(!processId)
              tempBlock1->used=tid;
            else tempBlock1->used=processId;
            //tempBlock1->map+=(size);
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
    return kmalloc_int(sz, 1, 0,1,1,0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys,1,0,0);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,1,1,0);
}

uint32_t pmalloc(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,2,1,4);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,1,0,0);
}

uint32_t malloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,0,0,0);
}
