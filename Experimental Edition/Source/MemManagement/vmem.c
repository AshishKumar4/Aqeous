#include "vmem.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "paging.h"

void Mapper()
{
    Mblock=(MemMap_t*)(4*1024*1024); //start the memory map linked list from 4th mb.
    MemMap_t* tempBlock2=Mblock;
    uint32_t i=0;
    for(i=0;i<(maxmem/(4096));i++) //Make blocks!!!
    {
        for(uint32_t j=0;j<1024;j++)
        {
            tempBlock2++;
            tempBlock2->addr=(j+(i*1024))*4096;
            tempBlock2->used=mmap_info->type-1;
            if(tempBlock2->used==0)
            {
              tempBlock2->map[0]=0;
              tempBlock2->map[1]=0;
              tempBlock2->map[2]=0;
              tempBlock2->map[3]=0;
            }
            else
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
    tempBlock2=Mblock;
    while(tempBlock2->addr<=(uint32_t)lastBlock)
    {
        tempBlock2->used=1;
        lastBlockAddr=tempBlock2;
        tempBlock2++;
    }
    tempBlock2=Mblock;
    Kblock=(MemMap_t*)(lastBlockAddr+sizeof(mblock));
}
