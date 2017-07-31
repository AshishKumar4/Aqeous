#ifndef VFS_H
#define VFS_H

#include "stdio.h"
#include "common.h"
#include "string.h"
#include "AqFS/fs.h"

typedef struct FSnode_Desc
{
  uint32_t type;        // File or Folder or Hardlink etc.
  uint32_t fs_type;     // AqFS or Ext2 or FAT etc
  uint32_t desc_sz;
  uint32_t desc_type;
  uint32_t sDesc[];          // Specific Descriptor Structure
}FSnode_Desc_t;

typedef struct VDirectory
{
  char name[64];
  uintptr_t dir_desc;
}VDirectory_t;

VDirectory_t current_dir;

typedef struct FS_Desc
{
  char name[8];

  uint32_t f_type;
  VDirectory_t root;

  func_t mkfl;
  func_t mkdir;
  func_t ls;
  func_t cd;
  func_t cp;
  func_t editfl;
  func_t cat;
  func_t mv;
  func_t rm;
}FS_Desc_t;

typedef struct Partition_Desc
{
  char name[8];
  FS_Desc_t FS;
}Partition_Desc_t;

Partition_Desc_t* Current_Partition;

typedef struct FILE_desc
{
  File_handle_t* handle;
  uint32_t fstream_ptr;
  uint32_t fsize;
  char mode[8];
}FILE;

void vfs_init();
void vfs_setup_aqfs();
void vfs_setup_ext2();

#endif
