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
    curr_ahci=ahci_start;
  	curr_disk=&curr_ahci->Disk[1];
  	curr_port=&abar->ports[1];
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

  if(!AqFS_init())
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

  ptmp->FS.root.dir_desc = aqfs_root;
  current_dir.dir_desc = aqfs_root;
  strcpy(current_dir.name, curr_dir.full_name);
  printf("\nAqFS Initialized\n");
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

  ptmp->FS.root.dir_desc = ext2_root;
  current_dir.dir_desc = ext2_root;
  strcpy(current_dir.name, ext2_root_name);
  printf("\nEXT2 Initialized\n");
}

uint32_t Detect_FS(uint32_t disk_id, uint32_t partition_id)
{

}
