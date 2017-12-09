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

void aqfs_cp()
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

void aqfs_del()
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

void aqfs_mkdir()
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

void aqfs_mkfl()
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

void aqfs_editfl()
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

void aqfs_rfl()
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



void ext2_mkdir()
{
  asm volatile("cli");
  char* arguments = CSI_Read(1);
//  ext2_inode_t* ptr_currentDir = ext2_root;
  void *initDir = ptr_currentDir;
  uint32_t length = strlen(arguments);

  uint32_t i;

  /*int32_t count would equal the number of characters before and
   * including the last '/' in the char *arguments, if it equals
   * -1 then that means there were no '/' in the char *arguments */
  int32_t count = -1;
  for(i = 0; i < length; i++)
    if(*(arguments + i) == '/')
      count = i;

  if(count != -1) //if count was changed, ie: in the char *arguments there was atleast one "/"
  {
    char *dirString;
    /* +1 being the \000 at the end, and the other +1 is due to
     * that we start counting from 0, 1, 2, etc. so if count was to equal
     * 0, that would mean 1 character, so we boost count up by one to equal
     * the number of characters */
    dirString = (char*)kmalloc(count + 2);

    // +1 for same reason as above, count starts from 0 for the first character
    memcpy(dirString, arguments, count + 1);
    *(dirString + count + 1) = 0; // add the \000 at the end of the string "dirString"

    uint32_t work = 1;//program_cd(dirString);

    if(!work) //if the program_cd() did not return an error
    {
      char *restString;
      restString = (char*)kmalloc(length - count);

      //get the contents of char *arguments after the final '/'
      memcpy(restString, arguments + count + 1, length - count - 1);
      *(restString + length - count - 1) = 0; //add the \000 at the end

      void *dir;

      dir = ext2_create_dir(ptr_currentDir, restString);

      setCurrentDir(initDir);

    //  kfree(restString);
    }

  //  kfree(dirString);
  }else{
    void *dir;
    printf("\nasds");
    dir = ext2_create_dir(ptr_currentDir, arguments);
  }

  return;
}

void ext2_cd()
{
  char* arguements = CSI_Read(1);
  int argLen = strlen(arguements);
  int i = 0, originalI = 0;

  int isInitDirRoot = FALSE;
  void *initDir = ptr_currentDir;

}
