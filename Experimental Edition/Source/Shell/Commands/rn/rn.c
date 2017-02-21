#include "fs.h"
#include "Shell.h"
#include "common.h"
#include "string.h"
#include "stdio.h"
#include "ext2/ext2_fs.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "tasking.h"

void Command_rn()
{
  char* path = CSI_Read(1);

  if(!file_loadOGP(path))
  {
    printf("\n%s file dosent exist!\n",path);
    return;
  }

  uint32_t off = StrToInt(CSI_ReadAPS("-x"));
  uint32_t sz = StrToInt(CSI_ReadAPS("-z"));

  if(!sz) sz = file_size(path);
  printf("\nSize of the file: %x \n", sz);

  uint32_t* buffer = kmalloc(8192);

  file_readTM(buffer,0,sz,path);

  //file_close("test2.txt");
  file_closeOGP(path);

  Shell_CFexecute(buffer, sz);

  kfree(buffer);

}
