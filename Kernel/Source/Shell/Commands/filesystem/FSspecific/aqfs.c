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
#include "kb_handle.h"

void aqfs_cd(char* dir)
{
   if(!strncmp(dir,"..",2))
   {
      set_curr_dir(curr_dir.dir->parent, "..");
      strcpy((char*)current_dir.name, (const char*)curr_dir.full_name);
   }
   else
   {
      Directory_t* new_dir = search_folderOGP(dir);
      if(new_dir)
      {
        //printf("%g%s%g", 10, dir, 15);
        set_curr_dir(new_dir->location, dir);
        strcpy((char*)current_dir.name, (const char*)curr_dir.full_name);
      }
    //  while(1);
      //  set_curr_dir(curr_dir.dir.location);
      //printf("%s",new_dir->name);
   }
}

void aqfs_ls()
{
   find_friendDirs(0);
   find_childFiles(0);
}

void aqfs_cp(char* dpath, char* spath, char* nname)
{
  char* tmpstr = spath;
  char* fname;
  int i = 0;
  for(i = strlen(spath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];

  if(!nname) nname = fname;

  file_loadOGP(spath);

  uint32_t sz = file_size(spath);

  uint32_t* buffer = kmalloc(sz);

  file_readTM(buffer, 0, sz, spath);

  file_closeOGP(spath);

  char* npath = kmalloc(strlen(dpath)+strlen(nname)+1);
  strcpy(npath, (const char*)dpath);
  npath[strlen(dpath)] = '/';
  strcpy(npath + strlen(dpath) + 1 , (const char*)nname);

  create_file(nname, 1, dpath);

  file_loadOGP(npath);

  file_writeAppend(buffer, sz, npath);
  //file_flushOGP(npath);

  file_closeOGP(npath);
}

void aqfs_del(char* path)
{
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
    File_t* file = (File_t*)handle->file;
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

void aqfs_mkdir(char* path)
{
  char* tmpath = kmalloc(strlen((const char*)path)+1);
  strcpy(tmpath, path);

  char* tmpstr = tmpath;
  char* fname;
  int i = 0;
  for(i = strlen((const char*)tmpath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];
  if(i>0)
    tmpath[i] = '\0';
  else tmpath = 0;

  create_directory(fname, 1, tmpath);
  printf("\n%s directory created\n",path);
}

void aqfs_mkfl(char *path)
{
  char* tmpath = kmalloc(strlen((const char*)path)+1);
  strcpy(tmpath, path);

  char* tmpstr = tmpath;
  char* fname;
  int i = 0;
  for(i = strlen((const char*)tmpath) - 1; tmpstr[i]!='/' && i>=0; i--);

  fname = &tmpstr[i+1];
  if(i>0)
    tmpath[i] = '\0';
  else tmpath = 0;

  ///char* destination_folder = (char*)CSI_Read(2);
  create_file(fname, 1, tmpath);
  printf("\n%s file created\n",path);
}

void aqfs_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz)
{
  //File_handle_t* fh = file_loadOGP(path);
  if(!file_loadOGP(path))
  {
    printf("\n%s file dosent exist!\n",path);
    return;
  }

  if(data)
  {
    if(!type || !strcmp("app", (char*)type))
    {
      printf("\n Data written!");
      printf(" %x\n", file_writeAppend(data, strlen((const char*)data), path));
    }
    else if(!strcmp("edit", (char*)type))
    {
      if(!osz) osz = strlen((char*)data);
      file_editFM(off, osz, data, strlen((char*)data), path);
      printf("\n File edited Successfully!\n");
    }
    else printf("\n Edit Type not recognized! %x\n", type);
  }
  else
  {
    printf("\nEnter the data (max size: 4096 bytes)->\n");
    uint32_t* buffer = kmalloc(4096);
    kb_getline((char*)buffer, 4096);
    data = buffer;
    printf("\nWritting the Data");
    if(!type || !strcmp("app", (char*)type))
    {
      printf("\n Data written!");
      printf(" %x\n", file_writeAppend(data, strlen((char*)data), path));
    }
    else if(!strcmp("edit", (char*)type))
    {
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

void* aqfs_rfl(char* path, uint32_t off, uint32_t sz)
{
  if(!file_loadOGP(path))
  {
    printf("\n%s file dosent exist!\n",path);
    return NULL;
  }

  if(!sz) sz = file_size(path);
  printf("\nSize of the file: %x, off: %d \n", sz, off);

  uint32_t* buffer = kmalloc(sz);

  printf("\t\tError: %x\n", file_readTM(buffer,0,sz,path));

  //file_close("test2.txt");
  file_closeOGP(path);

  printf("%s\n", buffer);
  return (void*)buffer;
}

void aqfs_mv(char* path, char* spath)
{
  printf("%s %s", path, spath);
}
