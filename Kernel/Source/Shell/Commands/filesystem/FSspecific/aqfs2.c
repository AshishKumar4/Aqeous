#include "Aqfs.h"
#include "AqfsTools.h"
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

void aqfs2_cd(char* dir)
{
  AqDhdrEntry_t* entry = Aq_LoadEntry(dir);
  AqRead(&(entry->location), (uint32_t*)Aq_CurrentDir, 1);
  AqfsMainFile->current_dir = Aq_CurrentDir;
}

void aqfs2_ls()
{
  char* path = CSI_Read(1);
  if(path)
  {
    Aq_ListEntrys(path);
  }
  else Aq_ListEntrys_direct(Aq_CurrentDir);
}

void aqfs2_cp(char* dpath, char* spath, char* nname)
{
  printf("%s %s %s", dpath, spath, nname);
}

void aqfs2_del(char* path)
{
  printf("\nReturned with code %d", Aq_Delete_File(path));
}

void aqfs2_mkdir(char* path)
{
  Aq_CreateNew_Directory(path, 1);
}

void aqfs2_mkfl(char *path)
{
  Aq_CreateNew_File(path, 1);
  printf("\n%s file created\n",path);
}

void aqfs2_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz)
{
  AqHandle_t* handle = Aq_LoadFile(path);
  if(!handle)
  {
    printf("\nFile [%s] could not be loaded!", path);
    return;
  }
  
  if(!osz) osz = Aq_HandleGet_FileSize(handle);

  if(data == 0)
  {
    printf("\nEnter the data (max size: 4096 bytes)->\n");
    uint32_t* buffer = kmalloc(4096);
    kb_getline((char*)buffer, 4096);
    data = buffer;

    printf("\nWritting the Data");

    if(!type || !strcmp("app", (char*)type))
    {
      printf(" %x\n", Aq_WriteFile_Append(data, strlen((char*)data), handle));
      printf("\n Data written!");
    }

    kfree(buffer);
  }
  else
  {
    printf("{%s, %d}", (char*)data, data);
   // Current_Partition->FS.editfl(path, data, type, off, osz);
  }
  Aq_CloseFile(handle);
}

void* aqfs2_rfl(char* path, uint32_t off, uint32_t sz)
{
  AqHandle_t* handle = Aq_LoadFile(path);
  if(!handle)
  {
    printf("\nFile [%s] could not be loaded!", path);
    return;
  }
  
  if(!sz) sz = Aq_HandleGet_FileSize(handle);
  printf("\nSize of the file: %x \n", sz);

  uint32_t* buffer = kmalloc(sz);

  printf("\t\tError: %d\n", Aq_ReadFile(buffer,0,sz,handle));

  //file_close("test2.txt");
  Aq_CloseFile(handle);

  printf("%s\n", buffer);
  return (void*)buffer;
}

void aqfs2_mv(char* path, char* spath)
{
}
