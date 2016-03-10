#include "vmem.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "paging.h"

uint32_t BlockFinder(uint32_t addr)
{
    MemMap_t *Block;
    addr/=4096;
    Block=(MemMap_t*)((uint32_t)Mblock + sizeof(MemMap_t)*(addr));
    Block++; //there was an offset in blocks from the start; first block is at Mblock+sizeof(MemMap_t)
    return (uint32_t)Block;
}

void Mapper()
{
  //it can be that 14th mb to 16th be memory mapped ISA hardware. So better not to use it.
    Mblock=(MemMap_t*)(0x01000000); //start of the memory map linked list from 16th mb.
    StartFrame=(FrameMap_t*)(0x1000);
    MemMap_t* tempBlock2=Mblock;
    uint32_t i=0;
    MemMap_t NullBlock;
    memset((void*)&NullBlock,0,sizeof(MemMap_t));
    for(i=0;i<(maxmem/4096);i++) //Make blocks!!!
    {
        for(uint32_t j=0;j<1024;j++)
        {
            tempBlock2++;
            *tempBlock2=NullBlock;
            //memset((void*)tempBlock2,0,sizeof(MemMap_t));
            tempBlock2->addr=(j+(i*1024))*4096;
            if(mmap_info)
              tempBlock2->used=mmap_info->type-1;
            else tempBlock2->used=1;
            if(tempBlock2->used)
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
        lastBlockAddr=tempBlock2;
        tempBlock2++;
    }
    tempBlock2=Mblock;
    Kblock=(MemMap_t*)(lastBlockAddr+sizeof(mblock));
}
