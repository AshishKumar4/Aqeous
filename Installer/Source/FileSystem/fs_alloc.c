#include "fs.h"
#include "mem.h"
#include "fs_alloc.h"

void fs_alloc_init()
{
    uint32_t buf=fsalloc(512);
    memset((void*)buf,0,512);
    for(uint64_t i=start_off;i<=bytes/512;i++)
    {
      write(curr_port,i,1,(DWORD)buf);
    }
    memset((void*)buf,0xff,512);
    for(uint64_t i=start_off;i<off;i++)
    {
      write(curr_port,i,1,(DWORD)buf);
    }
}

uint64_t fs_alloc(uint32_t blocks)
{
    uint32_t buf=fsalloc(512);
    uint8_t* ptr=(uint8_t*)buf;
    uint64_t block=0;
    uint32_t tmp=0;
    for(uint64_t i=off+start_off;i<=bytes/512;i++)
    {
      ptr=(uint8_t*)buf;
      memset((void*)buf,0,512);
      read(curr_port,i,1,(DWORD)buf);
      for(uint32_t j=0;j<512;j++)
      {
        for(uint32_t k=0;k<8;k++)
        {
          if(!(*ptr & (1<<k)))
          {
            if(tmp==blocks-1)
            {
              block=(k+(j*8)+(i*8*512))-blocks;
              block--;
              *ptr|=(1<<k);
              write(curr_port,i,1,(DWORD)buf);
              goto out;
            }
            ++tmp;
          }
          else tmp=0;
        }
        tmp=0;
        ++ptr;
      }
    }
    out:
    //printf("\nFound :D at block %x ",block*64);
    return block*64;
}

uint64_t sec_alloc(uint32_t blocks) //gives 1024 bytes aligned address only
{
    uint32_t buf=fsalloc(512);
    uint16_t* ptr=(uint16_t*)buf;
    uint16_t* tmp;
    uint64_t block=0;
    uint32_t stmp=0;
    for(uint64_t i=off+start_off;i<=bytes/512;i++)
    {
      ptr=(uint16_t*)buf;
      memset((void*)buf,0,512);
      read(curr_port,i,1,(DWORD)buf);
      for(uint32_t j=0;j<256;j++)
      {
        if(!*ptr) //is 1024 aligned
        {
          stmp++;
          if(stmp==1)
            tmp=ptr;
        }
        else stmp=0;
        if(stmp==blocks)
        {
          block=j+(256*i)+2;
          block-=blocks;
          ptr=tmp;
          for(uint32_t i=0;i<blocks;i++,ptr++)
            *ptr=0xffff;
          write(curr_port,i,1,(DWORD)buf);
          goto out;
        }
        ++ptr;
      }
    }
    out:
  //  printf("\n block %x ",block*1024);
    return block*1024;
}
