/*** AqFS File System, Aqeous OS's default FS ***/

#include "fs.h"
#include "mem.h"
#include "fs_alloc.h"

void fs_alloc_init()
{
    uint32_t buf=fsalloc(512);
    memset((void*)buf,0,512);
    for(uint32_t i=start_off;i<=bytemap_end;i++)
    {
      write(curr_port,i,1,(DWORD)buf);
    }
    memset((void*)buf,0xff,512);
    for(uint32_t i=start_off;i<bytemap_off;i++)
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
    for(uint64_t i=bytemap_off;i<=(bytemap_end);i++)
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
              block=(k+(j*8)+((i-start_off)*8*512))-blocks;
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
    printf("\nNo Memory left!");
    while(1);
    return 0;
    out:
    //printf("\nFound :D at block %x ",block*64);
    return block*64;
}

uint64_t sec_alloc(uint32_t sectors)
{
  uint32_t buf=fsalloc(512);
  uint8_t* ptr=(uint16_t*)buf;
  uint8_t* tmp;
  uint64_t block=0;
  uint32_t stmp=0;
  for(uint64_t i=bytemap_off;i<=(bytemap_end);i++)
  {
    ptr=(uint8_t*)buf;
    memset((void*)buf,0,512);
    read(curr_port,i,1,(DWORD)buf);
    for(uint32_t j=0;j<512;j++)
    {
      if(!*ptr) //is 512 aligned
      {
        stmp++;
        if(stmp==1)
          tmp=ptr;
      }
      else stmp=0;
      if(stmp==sectors)
      {
        block=j+(512*(i-start_off))+4;
        block-=sectors;
        ptr=tmp;
        for(uint32_t m=0;m<sectors;m++,ptr++)
          *ptr=0xff;
        write(curr_port,i,1,(DWORD)buf);
        goto out;
      }
      ++ptr;
    }
  }
  printf("\nNo Memory left!");
  while(1);
  return 0;
  out:
//  printf("\n block %x ",block*1024);
  return block*512;
}

uint64_t sblk_alloc(uint32_t blocks) //gives 1024 bytes aligned address only
{
    uint32_t buf=fsalloc(512);
    uint16_t* ptr=(uint16_t*)buf;
    uint16_t* tmp;
    uint64_t block=0;
    uint32_t stmp=0;
    for(uint64_t i=bytemap_off;i<=(bytemap_end);i++)
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
          block=j+(256*(i-start_off))+2;
          block-=blocks;
          ptr=tmp;
          for(uint32_t m=0;m<blocks;m++,ptr++)
            *ptr=0xffff;
          write(curr_port,i,1,(DWORD)buf);
          goto out;
        }
        ++ptr;
      }
    }
    printf("\nNo Memory left!");
    while(1);
    return 0;
    out:
  //  printf("\n block %x ",block*1024);
    return block*1024;
}

void del_blocks(uint64_t location, uint64_t size)
{
    uint64_t blocks=size;
    uint64_t offset=location/512;
    uint32_t buf=fsalloc(512);
    read(curr_port,offset/512,1,(DWORD)buf);
    uint16_t byte_off=(uint16_t)(offset=offset%512);
    uint8_t *byte=(uint8_t*)(buf+byte_off);
    uint8_t bit_off=(uint8_t)(location%512);
    for(uint32_t i=0;i<blocks;i++)
    {
      *byte^=(1<<bit_off);
      ++bit_off;
    }
    write(curr_port,offset/512,1,(DWORD)buf);
}
