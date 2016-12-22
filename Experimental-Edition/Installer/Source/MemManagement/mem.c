#include "mem.h"
#include "paging.h"
#include "vmem.h"
// end is defined in the linker script.
//extern page_directory_t *kernel_directory;

inline int clearBits(uint32_t map[],uint32_t sz) //gives the output as number of continuous clear bits
{
  uint8_t count=0;

  for(int i=0;i<128;i++)
  {
      if(count*32>=sz)
      {
        //printf(" 1 ");
        return i-(sz/32); //our job is done!
      }
      else if(!(map[i/32] & (1<<(i%32))))
      {
        ++count;
      }
      else
      {
        count=0;
        //++i;
      }
  }
  return -1;
}

uint32_t processID=10; //id 4 for paging, rest reserved

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys,int purpose,int packed,int processId)
{
    uint32_t mb=0;
    if(purpose==1) mb=200; //for kernel
    else if(purpose==2) mb=49; //for paging
    else if(purpose==3) mb=100; //for tasking
    else if(purpose==4) mb=110; //for tasking
    else if(purpose==5) mb=160; //for filesystem
    else mb=250;
    MemMap_t* Block=(MemMap_t*)BlockFinder(mb*1024*1024);
    uint32_t addr;
    uint32_t sz4096=sz/4096;
    uint32_t sz32=sz/32;
    if(!sz32)//if size is less then 32 bytes, roundof it to 32 which is the least memory allocable
    {
      sz=33;
      sz32=1;
    }
    uint32_t tsz32=(sz%4096)/32;
    while(1)
    {
        addr=Block->addr;
        if(addr>=(mb*1024*1024)+32) //If the block is in user memory; +32 for buffer
        {
          out:
          if(Block->used==0)
          {
              if (align == 1 && (addr & 0xFFFFF000)) //if 4k alignment required
              {
                while(Block->used) ++Block; //Simply get a comlpetely unused block!
                //Because Natively,Every Block's starting addr is 4k aligned!
              }
              MemMap_t* tm=Block;
              if(sz>4096)
              {
                  for(uint32_t i=0;i<sz4096+1;i++) //check if adjacent blocks are empty too!
                  {
                    ++tm;
                    if(tm->used) //cant fit here!
                    {
                      Block=tm;
                      goto out;
                    }
                  }
                  tm=Block;
                  for(uint32_t j=0;j<sz4096;j++)
                  {
                    tm->map[0]=0xFFFFFFFF; //set the bits to full
                    tm->map[1]=0xFFFFFFFF;
                    tm->map[2]=0xFFFFFFFF;
                    tm->map[3]=0xFFFFFFFF;
                    ++tm;
                  }
                  for(uint32_t i=0; i<tsz32;i++)
                  {
                    tm->map[i/32]|=(1<<(i%32));
                  }
                  addr=Block->addr;
                  break;
                }
                MemMap_t* tm2=Block;
                //If you reach here, you are lucky!
                for(uint32_t i=0; i<sz32;i++)
                {
                  tm2->map[i/32]|=(1<<(i%32));
                }
                //printf("map1-> %x ",tm2->map[0]);
                //just get out now!
                addr=Block->addr;
                break;
            }
            else if(!packed) //if packing required (0=required, 1=not)
            {
              if(sz<4096-8) //if memory can be accomodated in single block.
              {
                int tm;
                out2:
                if(Block->map[0]==0xFFFFFFFF &&
                  Block->map[1]==0xFFFFFFFF &&
                  Block->map[2]==0xFFFFFFFF &&
                  Block->map[3]==0xFFFFFFFF) //block already full, no space
                {
                  ++Block;
                  goto out;
                }
                tm=clearBits(Block->map,sz);
                //printf("Clear Bits-> %x ",tm);
                if(tm>=0) //if enough memory is in the region
                {
                  //tm++;
                  //if(tsz<14) //allocating for some variable probably :/
                  //  tm--; //dont leave a blank bit for it!
                  for(uint32_t i=tm;i<sz32+tm;i++)
                    Block->map[i/32]|=(1<<(i%32)); //set the bit;
                  //printf("map2-> %x ",Block->map[0]);
                  addr=Block->addr + (32*tm);
                  break;
                }
                else
                {
                  //printf(" 1 ");
                  ++Block; //search in some other block for the same
                  goto out2;
                }
              }
              else
              {
                out3:
                while(Block->used)
                  ++Block; //find a completely empty block
                MemMap_t* tm=Block;
                for(uint32_t i=0;i<sz4096+1;i++) //check if adjacent blocks are empty too!
                {
                  ++tm;
                  if(tm->used) //cant fit here!
                  {
                    Block=tm;
                    goto out3;
                  }
                }
                MemMap_t* tm2=Block;
                //If you reach here, you are lucky!
                for(uint32_t j=0;j<sz4096;j++)
                {
                    tm2->map[0]=0xFFFFFFFF; //set the bits
                    tm2->map[1]=0xFFFFFFFF;
                    tm2->map[2]=0xFFFFFFFF;
                    tm2->map[3]=0xFFFFFFFF;
                    ++tm2;
                }
                for(uint32_t i=0; i<tsz32;i++)
                {
                  tm->map[i/32]|=(1<<(i%32));
                }
                //just get out now!
                addr=Block->addr;
                break;
              }
            }
            if(Block->addr>=maxmem*1024)
            {
                printf("\nNo memory");
                return 0;
            }
          }
        ++Block;
    }
    if(phys)
        *phys=addr;
    uint32_t size=sz;
    uint32_t tid=processID;
    if(pag==0) //If paging not enabled
      {
        if(sz>4096||Block->used==0)
        for(uint32_t i=0;i<=(sz-1)/4096;i++,Block++,size-=4096)
        {
          if(!processId && !Block->used)
              Block->used=tid;
          else Block->used=processId;
          //tempBlock1->map+=size;
        }
      }
    else
    {
      if(sz>4096||Block->used==0)
        for(uint32_t i=0;i<=(sz-1)/4096;i++,Block++,size-=4096)
        {
            if(!processId && !Block->used)
                Block->used=tid;
            else Block->used=processId;
            Block->page=MapPage((void*)Block->addr,(void*)Block->addr);
        }
    }
    ++processID;
    return addr;
}

void free(uint32_t* ptr)
{
    uint32_t addr=(uint32_t)ptr;
    MemMap_t* Block=(MemMap_t*)BlockFinder(addr);
    uint32_t tsz=addr%4096;
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
          break;
        }
      }
    }
    else //it can only happen if the block is solely for a single process
    {
      uint16_t pid=Block->used; //get the process id in this case!
      MemMap_t* tm=Block,*tm2=Block;
      tm2++;
      while(tm2->used==pid)
      {
        tm->used=0;
        tm->map[0]=0;
        tm->map[1]=0;
        tm->map[2]=0;
        tm->map[3]=0;
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
          break;
        }
      }
    }
    if(Block->used && !Block->map[0]
      && !Block->map[1]
      && !Block->map[2]
      && !Block->map[3])
      {
        Block->used=0;
        free_page(Block->page);
      }
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

uint32_t pmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0,2,1,4);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,1,0,0);
}

uint32_t fsalloc(uint32_t sz) //for tasks(threads) Stakcs
{
    return kmalloc_int(sz, 0, 0,5,0,0);
}

uint32_t malloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,0,0,0);
}
