#include "fs.h"
#include "Shell/shell.h"
#include "common.h"
#include "string.h"
#include "stdio.h"
#include "ext2/ext2_fs.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "MultiTasking/tasking.h"

#include "rn.h"

char fs_path[256];

void Command_rn()
{
  char* path = (char*)CSI_Read(1);
  strcpy(fs_path, path);

  Activate_task_direct_SP(create_task("rn_Main", rn_Main, 18, 0x202, Shell_proc), Get_Scheduler());/*/
  rn_Main();//*/
}

void rn_Main()
{
  asm volatile("cli");
  char* path = fs_path;// = (char*)CSI_Read(1);
  FILE* fl = fopen(path, "rbw");
  if(!fl)
  {
    printf("\n%s file dosent exist!\n",path);
    kill();
    return;
  }

  uint32_t off = StrToInt((char*)CSI_ReadAPS("-x"));
  uint32_t sz = StrToInt((char*)CSI_ReadAPS("-z"));

  if(!sz)
  {
    fseek(fl, 0, SEEK_END);
    sz = ftell(fl);
    fseek(fl, 0, SEEK_CUR);

    printf("\nSize: %d", sz);
  }

  printf("\nSize of the file: %x \n", sz);

  uint32_t* buffer = kmalloc(sz);

  if(!fread(buffer, 1, sz, fl))
  {
    printf("\nFile Read Error!");
    kill();
    return;
  }

  //file_close("test2.txt");
  fclose(fl);

  Shell_CFexecute(buffer, sz);

  kfree(buffer);
  kill();
}
