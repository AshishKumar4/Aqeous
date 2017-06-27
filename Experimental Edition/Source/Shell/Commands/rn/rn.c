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

#include "rn.h"

char fs_path[256];

void Command_rn()
{
  char* path = (char*)CSI_Read(1);
  strcpy(fs_path, path);

  //Activate_task_direct_SP(create_task("rn_Main", rn_Main, 18, 0x202, Shell_proc), Get_Scheduler());/*/
  rn_Main();//*/
}

void rn_Main()
{
  asm volatile("cli");
  char* path = fs_path;// = (char*)CSI_Read(1);

  if(!file_loadOGP(path))
  {
    printf("\n%s file dosent exist!\n",path);
  //  kill();
    return;
  }

  uint32_t off = StrToInt((char*)CSI_ReadAPS("-x"));
  uint32_t sz = StrToInt((char*)CSI_ReadAPS("-z"));

  if(!sz) sz = file_size(path);
  printf("\nSize of the file: %x \n", sz);

  uint32_t* buffer = kmalloc(8192);

  file_readTM(buffer,0,sz,path);

  //file_close("test2.txt");
  file_closeOGP(path);

  Shell_CFexecute(buffer, sz);

  kfree(buffer);

//  kill();
}
