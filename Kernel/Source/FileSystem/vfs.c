#include "stdio.h"
#include "string.h"
#include "common.h"
#include "ahci.h"

#include "vfs.h"
#include "ext2/ext2_fs.h"
#include "AqFS/fs.h"
#include "AqFS/fs_alloc.h"
#include "ahci.h"
#include "ata.h"

Partition_Desc_t* ptmp;

int Disk_Setup()
{ 
    curr_ahci=AHCI_Controllers;
    curr_disk=&curr_ahci->Disk[0];
    curr_port=curr_disk->port;
  	if(!curr_disk||!curr_port||!sata)
  	{
  		printf("\nNo DISK Found");
  		strcpy(curr_dir.full_name,"no_disk");
  		return 1;
  	}
    return 0;
}

void vfs_init()
{
  if(Disk_Setup()) return;
	SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
	sectors=info->lba_capacity;
	printf("\nTotal Sectors: %x\n",sectors);

  ptmp = kmalloc(sizeof(Partition_Desc_t));
  strcpy(ptmp->name, "C:\\");

  if(!Aqfs2_Checkfs())
  {
    vfs_setup_aqfs2();
  }
  else if(!AqFS_init())
  {
    vfs_setup_aqfs();
  }
  else if(!ext2_initialize(4096*10, 0))
  {
    vfs_setup_ext2();
  }
  else
  {
    printf("\nNo Recognized filesystem found\n");
  }
  Current_Partition = ptmp;
}

void vfs_setup_aqfs()
{
  strcpy(ptmp->FS.name, "AqFS");
  ptmp->FS.f_type = 0;
  ptmp->FS.mkfl = &aqfs_mkfl;
  ptmp->FS.ls = &aqfs_ls;
  ptmp->FS.mkdir = &aqfs_mkdir;
  ptmp->FS.cd = &aqfs_cd;
  ptmp->FS.cp = &aqfs_cp;
  ptmp->FS.editfl = &aqfs_editfl;
  ptmp->FS.cat = &aqfs_rfl;
  ptmp->FS.mv = &aqfs_mv;
  ptmp->FS.rm = &aqfs_del;

  ptmp->FS.load = &file_vfsLoad;
  ptmp->FS.close = &file_vfsClose;
  ptmp->FS.read = &file_read;
  ptmp->FS.write = &file_write;
  ptmp->FS.getSize = &file_GetSize;

  ptmp->FS.root.dir_desc = (uintptr_t)aqfs_root;
  current_dir.dir_desc = (uintptr_t)aqfs_root;
  strcpy(current_dir.name, curr_dir.full_name);
  printf("\nAqFS Initialized\n");
}

void vfs_setup_aqfs2()
{
  strcpy(ptmp->FS.name, "AqFS2");
  ptmp->FS.f_type = 0;
  ptmp->FS.mkfl = &aqfs2_mkfl;
  ptmp->FS.ls = &aqfs2_ls;
  ptmp->FS.mkdir = &aqfs2_mkdir;
  ptmp->FS.cd = &aqfs2_cd;
  ptmp->FS.cp = &aqfs2_cp;
  ptmp->FS.editfl = &aqfs2_editfl;
  ptmp->FS.cat = &aqfs2_rfl;
  ptmp->FS.mv = &aqfs2_mv;
  ptmp->FS.rm = &aqfs2_del;
  
  ptmp->FS.load = &Aq_LoadFile;
  ptmp->FS.close = &Aq_CloseFile;
  ptmp->FS.read = &Aq_ReadFile;
  ptmp->FS.write = &Aq_WriteFile;
  ptmp->FS.getSize = &Aq_HandleGet_FileSize;

  ptmp->FS.root.dir_desc = (uintptr_t)AqfsMainFile->root_dir;
  current_dir.dir_desc = (uintptr_t)AqfsMainFile->root_dir;
  strcpy(current_dir.name, "root");
  printf("\nAqFS 2 Initialized\n");
}

void vfs_setup_ext2()
{
  strcpy(ptmp->FS.name, "EXT2");
  ptmp->FS.f_type = 0;
  ptmp->FS.mkfl = ext2_mkfl;
  ptmp->FS.ls = ext2_ls;
  ptmp->FS.mkdir = ext2_mkdir;
  ptmp->FS.cd = ext2_cd;
  ptmp->FS.cp = ext2_cp;
  ptmp->FS.editfl = ext2_editfl;
  ptmp->FS.cat = ext2_rfl;
  ptmp->FS.mv = ext2_mv;
  ptmp->FS.rm = ext2_del;

  ptmp->FS.root.dir_desc = (uintptr_t)ext2_root;
  current_dir.dir_desc = (uintptr_t)ext2_root;
  strcpy(current_dir.name, ext2_root_name);
  printf("\nEXT2 Initialized\n");
}

uint32_t Detect_FS(uint32_t disk_id, uint32_t partition_id)
{
  printf("\n%d %d", disk_id, partition_id);
}
/*
int vfs_SetCurrDir(void* directory)
{

  //if the current directory is the one wanted to be switched to, exit gracefully
  if(VFS_ptr_currentDir == directory)
  return 0;

 // kfree(path); //frees the contents of the char array pointer, path

  VFS_ptr_currentDir = directory; //sets the value of the dir inode to the cuurentDir_inode

  FILE *node = ;// GET THE INNODE OF THE DIR __open__(directory, name_of_dir(directory), 0, FALSE);
  FILE *copy = 0;

  //sucess!
  return 0;
}

FILE* vfs_Open(void *node, char *name, char *mask, uint8_t open)
{

}

FILE* vfs_findDir(void *node, char *name)
{

}

void vfs_findDir_Close(FILE *node)*/