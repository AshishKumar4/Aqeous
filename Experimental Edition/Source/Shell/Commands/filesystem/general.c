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

void Command_cp()
{
  asm volatile("cli");
  char* dpath = CSI_ReadAPS("-d");  //destination Path
  char* spath = CSI_ReadAPS("-s");  //Source Path
  char* nname = CSI_ReadAPS("-r");  //New Name

  char* tmpstr = spath;
  char* fname;
  int i = 0;
  for(i = strlen(spath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];

  if(!nname) nname = fname;
  if(!dpath && !spath)
  {
    dpath = CSI_Read(1);
    spath = CSI_Read(2);
  }

  file_loadOGP(spath);

  uint32_t sz = file_size(spath);

  uint32_t* buffer = kmalloc(sz);

  file_readTM(buffer, 0, sz, spath);

  file_closeOGP(spath);

  char* npath = kmalloc(strlen(dpath)+strlen(nname)+1);
  strcpy(npath, dpath);
  npath[strlen(dpath)] = '/';
  strcpy(npath + strlen(dpath) + 1 , nname);

  create_file(nname, 1, dpath);

  file_loadOGP(npath);

  file_writeAppend(buffer, sz, npath);
  //file_flushOGP(npath);

  file_closeOGP(npath);
  printf("\nFile copied successfully\n");
}

void Command_del()
{
  asm volatile("cli");
  char* path = (char*)CSI_Read(1);
  uint32_t tmp = (uint32_t)search_folderOGP(path);
  if(tmp) //is it a folder?
  {
    Directory_t* dir = (Directory_t*)tmp;
    if(dir->magic != DIR_MAGIC)
    {
      printf("\nThe Folder is corrupt.\n");
      return;
    }
    //printf(dir->name);
    //Delete the directory!
    if(delete_dir(dir))
      printf("\n Folder Successfully deleted!\n");
  }
  else
  {
    File_handle_t* handle=file_searchOGP(path);
    if(!handle)
    {
      file_loadOGP(path);
      handle=file_searchOGP(path);
    }
    File_t* file = handle->file;
    if(!file)
    {
      printf("\n%s is not a file, nor a folder!\n",path);
      return;
    }
    if(file->magic != FIL_MAGIC)
    {
      printf("\nThe File is corrupt.\n");
      return;
    }
    //printf(file->name);
    //Delete the file!
    if(delete_file(file))
      printf("\n File Successfully deleted!\n");
  }
}

void Command_mkdir()
{
  asm volatile("cli");
  char* path = (char*)CSI_Read(1);

  char* tmpath = kmalloc(strlen(path)+1);
  strcpy(tmpath, path);

  char* tmpstr = tmpath;
  char* fname;
  int i = 0;
  for(i = strlen(tmpath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];
  if(i>0)
    tmpath[i] = '\0';
  else tmpath = 0;

  create_directory(fname, 1, tmpath);
  printf("\n%s directory created\n",path);
}

void Command_mkfl()
{
  asm volatile("cli");
  char* path = (char*)CSI_Read(1);
  char* dir_name = (char*)CSI_Read(1);

  char* tmpath = kmalloc(strlen(path)+1);
  strcpy(tmpath, path);

  char* tmpstr = tmpath;
  char* fname;
  int i = 0;
  for(i = strlen(tmpath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];
  if(i>0)
    tmpath[i] = '\0';
  else tmpath = 0;

  ///char* destination_folder = (char*)CSI_Read(2);
  create_file(fname, 1, tmpath);
  printf("\n%s file created\n",path);
}

void Command_editfl()
{
  asm volatile("cli");
  char* path = (char*)CSI_ReadAPS("-f");

  if(!path)
  {
    path = CSI_Read(1);
    if(!path)
    {
      printf("\nPlease Specify a File Name.");
      return;
    }
  }
  File_handle_t* fh = file_loadOGP(path);
  if(!fh)
  {
    printf("\n%s file dosent exist!\n",path);
    return;
  }

  uint32_t* data = CSI_ReadAPS("-d");


  uint32_t* type = CSI_ReadAPS("-a");

  // 1-> append
  // 2-> edit in between
  if(data)
  {
    if(!type || !strcmp("app",type))
    {
      printf("\n Data written!");
      printf(" %x\n", file_writeAppend(data, strlen((char*)data), path));
    }
    else if(!strcmp("edit",type))
    {
      uint32_t off = StrToInt(CSI_ReadAPS("-x"));
      uint32_t osz = StrToInt(CSI_ReadAPS("-z"));
      if(!osz) osz = strlen(data);
      file_editFM(off, osz, data, strlen((char*)data), path);
      //file_flushOGP(path);
      printf("\n File edited Successfully!\n");
    }
    else printf("\n Edit Type not recognized! %x\n", type);
  }
  else
  {
    printf("\nEnter the data (max size: 4096 bytes)->\n");
    uint32_t* buffer = kmalloc(4096);
    kb_getline(buffer, 4096);
    data = buffer;
    printf("\nWritting the Data");
    if(!type || !strcmp("app",type))
    {
      printf("\n Data written!");
      printf(" %x\n", file_writeAppend(data, strlen((char*)data), path));
    }
    else if(!strcmp("edit",type))
    {
      uint32_t off = StrToInt(CSI_ReadAPS("-x"));
      uint32_t osz = StrToInt(CSI_ReadAPS("-z"));
      if(!osz) osz = strlen(data);
      file_editFM(off, osz, data, strlen((char*)data), path);
      //file_flushOGP(path);
      printf("\n File edited Successfully!\n");
    }
    else printf("\n Edit Type not recognized! %s\n", type);
    kfree(buffer);
  }
  file_closeOGP(path);
}

void Command_rfl()
{
  asm volatile("cli");
  char* path = CSI_Read(1);
  //file_load("test2.txt");
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

  printf("\t\tError: %x\n", file_readTM(buffer,0,sz,path));

  //file_close("test2.txt");
  file_closeOGP(path);

  printf("%s\n", buffer);
  kfree(buffer);

}
