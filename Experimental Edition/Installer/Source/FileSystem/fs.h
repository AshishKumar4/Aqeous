/*** AqFS File System, Aqeous OS's default FS ***/

#ifndef FS_h
#define FS_h

#include "ahci.h"
#include "ata.h"

#define FHR_MAGIC 0xAFE472

typedef enum
{
    in = 0x01,
    out = 0x02,
    trunc = 0x04,
    ate = 0x08,
    app = 0x10,
}ios;

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
    char name[74]; //long file names supported
    uint16_t perm;
    uint32_t sz;
    uint32_t last_edited;
    uint32_t headers;
    uint64_t parent;
    uint64_t location;
    uint64_t first_header;
    uint64_t last_header;
    uint64_t Next_Friend;
}File_t;

typedef struct __attribute__ ((packed)) File_Header
{
    uint32_t used;
    uint32_t spread; // Amount of memory the header can have.
    uint32_t reserved;
    uint32_t magic;
    uint64_t File_location;
    uint64_t location;
    uint64_t Next_Header;
    uint64_t Previous_Header;
}File_Header_t;

typedef struct Special_dirs
{
    Directory_t dir;
    uint16_t type;
    char full_name[100];
}Special_Dirs_t;

typedef struct __attribute__ ((packed)) Identity_Sectors
{
    char name[8];  //Storage media Name
    uint16_t active_partition;
    //uint8_t partitions; //partitions i.e, number of root directories.
    //uint32_t Number_of_sectors;
    //uint32_t bytes_per_sector;
    //uint64_t partition_locations[32]; //locations of the partitions. 32 partitions on 1 disk supported
    //uint64_t reserved;
}Identity_Sectors_t;

typedef struct __attribute__((packed)) Partition_struct
{
  uint8_t boot_indicator;
  uint8_t starting_head; //255
  uint8_t starting_sector; //63
  uint8_t starting_cylinder; //1023
  uint8_t sys_id; //Use C2 or ED
  uint8_t ending_end;
  uint8_t ending_sector;
  uint8_t ending_cylinder;
  uint32_t relative_sector;
  uint32_t total_sectors;
}Partition_struct_t;

typedef struct __attribute__ ((packed)) File_handle
{
    char* name;
    uint32_t file; //location of loaded file struct in RAM
    uint32_t current_header;
    uint32_t  ios;
    uint64_t file_location;
    struct File_handle* next;
}File_handle_t;

File_handle_t* start_handle,*end,*current; //file queue

Special_Dirs_t curr_dir;

Directory_t current_;

HBA_PORT* curr_port;

Disk_dev_t* curr_disk;

ahci_t* curr_ahci;

uint32_t sectors;

uint32_t bytemap_off;

uint32_t bytemap_end;

File_t curr_file;

uint32_t fsbuf;

size_t header_data=1024-sizeof(File_Header_t);

void find_friendDirs(char* name);
void find_childFiles(char* name);

inline void flush_header(File_Header_t* header);

//File_Header_t* file_header_search(uint32_t foffset, File_t* file);

int del_header(File_Header_t* header);

File_Header_t* get_header(uint64_t location);

File_Header_t* nx_header(File_Header_t* prev_header);

void create_directory(char *name, uint16_t perm, char* destination);

void create_file(char *name,uint16_t perm,char* destination);

void make_boot_sector();

File_handle_t* file_loader(char* name);

void set_curr_dir(uint64_t location);

Directory_t* search_folder(char* name);

File_Header_t* File_Header_Creator(File_t* file, uint16_t blocks);

File_Header_t* File_Header_Creator_sdw(File_t* file, File_Header_t* left_header,  uint16_t blocks);

File_handle_t* file_search(char* name);

void file_load(char *name);

void file_truncate(File_handle_t* handle);

void file_flush(char* name);

void file_close(char *name);

int file_readTM(uint32_t* buffer, uint32_t offset, uint32_t size, char* file_name); //Read file content and write to memory.

int file_writeAppend(uint32_t* buffer, uint32_t size, char* file_name); //Write to a file from memory.

int file_editFM(uint32_t offset, uint32_t osize, uint32_t *buffer, uint32_t fsize, char* file_name);

#endif
