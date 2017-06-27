#include "loader/loader.h"
#include "AqFS/fs.h"
#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "tasking.h"
#include "process.h"


BinHeader_t* load_bin(char* path)
{
  /*
    Get The File name from the path
    Load the file into memory
    Create process, allocate stack and memory
    Load bin headers
  */
  if(!file_loadOGP(path))
  {
    printf("\nFile Not found!");
    return;
  }

  uint32_t sz = file_size(path);
  printf("\nSize of the file: %x \n", sz);

  uint32_t* buffer = kmalloc(512);

  file_readTM(buffer, 0, 512, path);

  BinHeader_t* hd = buffer;
  uint32_t bsz = hd->prog_size;

  buffer = (uint32_t)kmalloc(bsz);

  file_readTM(buffer, 512, bsz, path);

  hd->relative_first = (uint32_t*)buffer;

  file_closeOGP(path);

  return hd;
}
