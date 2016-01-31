#include "vmem.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "paging.h"

void Mapper()
{
    Mblock=4*1024*1024*5; //start the memory map linked list from 20th mb.
    Mblock->link=(uint32_t)Mblock;
    tempBlock2=Mblock;
    uint32_t i=0;
    for(i=0;i<(maxmem/(4096));i++) //Make blocks!!!
    {
        for(uint32_t j=0;j<1024;j++)
        {
            tempBlock2=(uint32_t)tempBlock2->link;
            tempBlock2->addr=(j+(i*1024))*4096;
            tempBlock2->used=mmap_info->type-1;
            tempBlock2->link=(uint32_t)(tempBlock2+1);
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
    while(tempBlock2->addr<=lastBlock)
    {
        tempBlock2->used=1;
        lastBlockAddr=tempBlock2;
        tempBlock2++;
    }
    tempBlock2=Mblock;
    Kblock=lastBlockAddr+sizeof(mblock);
}
