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
#include "kb_handle.h"

#include "vfs.h"
#include "FSspecific/aqfs.h"
#include "FSspecific/aqfs2.h"
#include "FSspecific/ext2.h"

void Command_cd()
{
  char* dir = (char*)CSI_Read(1);
  Current_Partition->FS.cd(dir);
}

void Command_ls()
{
  Current_Partition->FS.ls();
}

void Command_mkdir()
{
  char* arguments = (char*)CSI_Read(1);
  Current_Partition->FS.mkdir(arguments);
}

void Command_cp()
{
  char* dpath = (char*)CSI_ReadAPS("-d");  //destination Path
  char* spath = (char*)CSI_ReadAPS("-s");  //Source Path
  char* nname = (char*)CSI_ReadAPS("-r");  //New Name
  if(!dpath && !spath)
  {
    dpath = (char*)CSI_Read(1);
    spath = (char*)CSI_Read(2);
  }
  Current_Partition->FS.cp(dpath, spath, nname);
}

void Command_cat()
{
  char* path = (char*)CSI_Read(1);

  uint32_t off = StrToInt((char*)CSI_ReadAPS("-x"));
  uint32_t sz = StrToInt((char*)CSI_ReadAPS("-z"));

  Current_Partition->FS.cat(path, off, sz);
}

void Command_mkfl()
{
  char* path = (char*)(char*)CSI_Read(1);
  char* dir_name = (char*)(char*)CSI_Read(2);

  Current_Partition->FS.mkfl(path, dir_name);
}

void Command_rm()
{
  char* path = (char*)(char*)CSI_Read(1);

  Current_Partition->FS.rm(path);
}

void Command_mv()
{

}

void Command_editfl()
{
  char* path = (char*)(char*)CSI_ReadAPS("-f");
  if(!path)
  {
    path = (char*)CSI_Read(1);
    if(!path)
    {
      printf("\nPlease Specify a File Name.");
      return;
    }
  }
  uint32_t* data = (char*)CSI_ReadAPS("-d");
  uint32_t* type = (char*)CSI_ReadAPS("-a");

  uint32_t off = StrToInt((char*)CSI_ReadAPS("-x"));
  uint32_t osz = StrToInt((char*)CSI_ReadAPS("-z"));
  if(!osz) osz = strlen(data);

  Current_Partition->FS.editfl(path, data, type, off, osz);
}
