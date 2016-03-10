#ifndef FS_h
#define FS_h

#include "ahci.h"
#include "ata.h"

#define BASE        167772160 //160*1024*1024
#define BASE_SIZE   41943040 //40*1024*1024

typedef struct directory
{
    char name[58];
    uint16_t perm;
    uint32_t sz;
    uint32_t files;
    uint32_t folders;
    uint64_t parent;
    uint64_t location;
    uint64_t First_child;
    uint64_t First_file;
    uint64_t last_file;
    uint64_t last_child;
    uint64_t Next_Friend;
}Directory_t;

typedef struct __attribute__ ((packed)) file
{
    char name[86]; //long file names supported
    uint16_t perm;
    uint32_t sz;
    uint32_t last_edited;
    uint64_t parent;
    uint64_t location;
    uint64_t first_header;
    uint64_t Next_Friend;
}File_t;

typedef struct File_Header
{
    uint64_t Next_Header;
    uint64_t Previous_Header;
}File_Header_t;

typedef struct Special_dirs
{
    Directory_t dir;
    uint16_t type;
    char full_name[100];
}Special_Dirs_t;

typedef struct __attribute__ ((packed)) Boot_sectors
{
    char name[32];  //Storage media Name
    uint8_t partitions; //partitions i.e, number of root directories.
    uint32_t Number_of_sectors;
    uint32_t bytes_per_sector;
    uint64_t partition_locations[32]; //locations of the partitions. 32 partitions on 1 disk supported
    uint64_t reserved;
}Boot_sectors_t;

Special_Dirs_t curr_dir;

Directory_t current_;

HBA_PORT* curr_port;

Disk_dev_t* curr_disk;

ahci_t* curr_ahci;

uint32_t sectors;

File_t curr_file;

void find_dir(char* name);

void create_directory(char *name, uint16_t perm, char* destination);

void create_file(char *name,uint16_t perm,char* destination);

void make_boot_sector();

void file_load(char* name);

void set_curr_dir(uint64_t location);

Directory_t* search_folder(char* name);

#endif
