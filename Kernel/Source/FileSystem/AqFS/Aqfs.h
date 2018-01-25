#ifndef AQFS_INTERNALS_H
#define AQFS_INTERNALS_H 

#include "stdio.h"
#include "stdint.h"
#include "common.h"

#define AQ_DHR_MAGIC            0xAFE471      // Directory Header Magic
#define AQ_FHR_MAGIC            0xAFE472      // File Header Magic
#define AQ_DIR_MAGIC            0xAEF473      // Directory Magic
#define AQ_FIL_MAGIC            0xAEF474      // File Magic

#define AQ_VFILE_MAGIC          0xAEF111      // Volatile File Magic
#define AQ_MHDR_MAGIC           0xAEF011     // Main Header Magic

#define AQFS2_SYSID             0xED

#define AQ_MAX_NAME_LENGTH         64

#define AQ_FILE                     1
#define AQ_DIR                      2
#define AQ_SOFTLINK_FILE            3
#define AQ_HARDLINK_FILE            4
#define AQ_SOFTLINK_DIR             5
#define AQ_HARDLINK_DIR             6

#define AQ_ALLOCTABLE_SIZE          8192
#define AQ_ALLOCTABLE_MAX_ENTRIES   8192/sizeof(AqAllocEntry_t)
#define AQ_ALLOCTABLE_BLOCKS        8192/512

#define AQ_EXISTANCE_REAL           0
#define AQ_EXISTANCE_VIRTUAL        1

// Bundle is smallest unit which is I/O from the Disk. It is some multiple of sector size
// Blocks are smallest unit of Disk Memory. Every structure is block aligned.
uint32_t AQ_BUNDLES_PER_SECTOR  =  1;
uint32_t AQ_SECTOR_SIZE         =  512;
uint32_t AQ_BUNDLE_SIZE         =  512;//AQ_SECTOR_SIZE * AQ_BUNDLES_PER_SECTOR;
uint32_t AQ_BLOCKS_PER_SECTOR   =  1;
uint32_t AQ_BLOCK_SIZE          =  512;//(AQ_SECTOR_SIZE/AQ_BLOCKS_PER_SECTOR);

#define AQ_DHEADER_SIZE            4096     // Directory Headers are like Lists of Descriptor tables for the child dir/files of any dir.
#define AQ_FHEADER_SIZE            4096     // File Headers are lists of contents of file.

#define AQ_FENTRY_SIZE             512     // Minimum block of data that can be placed in a file

typedef struct AqLocation 
{
    uint32_t        lower32;                        // This address is 512 aligned.
    uint32_t        higher32;
 //   uint16_t        Offset;
}AqLocation_t;

typedef struct AqDhdrEntry
{
    uint32_t        magic;                         // This itself describes its type
    uint16_t        offset;
    uint16_t        type;
    AqLocation_t    location;

    char            name[AQ_MAX_NAME_LENGTH];
}AqDhdrEntry_t;

typedef struct AqFhdrEntry
{
    uint32_t        magic;                         // This itself describes its type
    uint32_t        spread;                          // Size divided by 512
    uint32_t        used;
    uint32_t        offset;

    uint16_t        id;
    uint16_t        nextOff;

    AqLocation_t    NextHdr;
    AqLocation_t    location;
}AqFhdrEntry_t;

typedef struct AqInfo
{
    uint32_t        perm;
    uint32_t        size;
    uint32_t        type;                           // Is Soft Link/Hard Link/Original
    //  Date
    uint32_t        data_size;
    uint32_t        onDisk_size;
    //  Last Edit 
    //  Author
}AqInfo_t;

typedef struct AqDirectory 
{
    uint32_t        magic;
    uint32_t        n_files;
    uint32_t        n_dir;
    uint32_t        headers;
    uint32_t        OpenHeader;

    uint32_t        dhdrOffset;

    AqInfo_t        info;

    AqLocation_t    location;
    AqLocation_t    parentDir;
    AqLocation_t    dhdr;
    AqLocation_t    FirstDirHeader;
    AqLocation_t    LastDirHeader;
}AqDirectory_t;

typedef struct AqFile 
{
    uint32_t        magic;
    uint32_t        headers;

    uint32_t        dhdrOffset;
    
    AqInfo_t        info;

    uint32_t        lastEntryOff;
    AqLocation_t    lastEntryHdr;

    AqLocation_t    location;
    AqLocation_t    parentDir;
    AqLocation_t    dhdr;
    AqLocation_t    FirstFileHeader;
    AqLocation_t    LastFileHeader;
}AqFile_t;

typedef struct AqDirHeader 
{
    uint32_t        magic;
    uint32_t        usedEntrys;
    uint32_t        maxEntrys;
    uint32_t        id;

    AqLocation_t    location;
    AqLocation_t    parentDir;
    AqLocation_t    NextDirHeader;

    AqDhdrEntry_t   Entrys[0];
}AqDhdr_t;

typedef struct AqFileHeader
{
    uint32_t        magic;
    uint32_t        usedEntrys;
    uint32_t        maxEntrys;
    uint32_t        id;

    AqLocation_t    location;
    AqLocation_t    parentFile;
    AqLocation_t    NextFileHeader;

    AqFhdrEntry_t   Entrys[0];
}AqFhdr_t;

typedef struct AqHandle 
{
    uint32_t        type;
    char            name[AQ_MAX_NAME_LENGTH];   
    char*           path;
    uintptr_t       descriptor;
    uint32_t        existance;
}AqHandle_t;

typedef struct AqMainHeader
{
    uint32_t        magic;
    char            name[10];
    uint32_t        version;

    uint32_t        atreeSize;
    uint32_t        metaSpread;

    AqLocation_t    location;
    AqLocation_t    metaTable;
    AqLocation_t    rootDir;
}AqMainHeader_t;

typedef struct AqAllocEntry 
{
    AqLocation_t        start;
    uint32_t            size;
}AqAllocEntry_t;

typedef struct AqAllocTable 
{
    AqAllocEntry_t entries[AQ_ALLOCTABLE_MAX_ENTRIES];
}AqAllocTable_t;

typedef struct AqMetaEntry
{
    AqLocation_t        loc;
    uint32_t            greatest;
    uint32_t            empty;
}AqMetaEntry_t;

typedef struct AqAllocMeta
{
    uint32_t            n_entries; 
    uint32_t            n_available; 
    AqLocation_t        location;
    AqMetaEntry_t       entries[];
}AqAllocMeta_t;

typedef struct AqVolatileFile
{
    uint32_t        magic;
    uint32_t        sectors;
    uint32_t        bytesPerSector;
    uint32_t        size;
    AqDirectory_t*       current_dir;
    AqDirectory_t*       root_dir;
    AqAllocMeta_t*       metaTable_ptr;
    AqMainHeader_t*      MainHeader_ptr;
}AqVolatileFile_t;

AqVolatileFile_t* AqfsMainFile;

typedef struct AqPartitionMeta
{
    uint8_t           boot_indicator;
//  uint8_t           starting_head; //255
//  uint8_t           starting_sector; //63
//  uint8_t           starting_cylinder; //1023
    uint32_t           start     :   24;
    uint8_t           sys_id; //Use C2 or ED
    uint32_t           end       :   24;
//  uint8_t           ending_end;
//  uint8_t           ending_sector;
//  uint8_t           ending_cylinder;
    uint32_t          relative_sector;
    uint32_t          total_sectors;
}AqPartitionMeta_t;

typedef struct AqMBR_Meta
{
	char                name[10];  //Storage media Name     0x1b4
    AqPartitionMeta_t	partitions[4];
    uint8_t             bootBytes[2];
}AqMBR_Meta_t;

uint32_t AQ_DHEADER_MAX_ENTRYS  =  (AQ_DHEADER_SIZE - sizeof(AqDhdr_t))/sizeof(AqDhdrEntry_t);    

AqDirectory_t* Aq_CurrentDir;

int Aqfs2_InitStage1();
int Aqfs2_InitStage2();
int Aqfs2_Checkfs();
int Aqfs2_Partitioner(int partitionId, int start, int size);
int Aqfs2_burn(int partitionId);

int Aq_CreateNew_File(char* name, uint32_t perm);
int Aq_CreateNew_Directory(char* name, uint32_t perm);
int Aq_ListEntrys(char* path);
int Aq_ListEntrys_direct(AqDirectory_t* dir);
AqHandle_t* Aq_Load(char* path);
AqHandle_t* Aq_LoadFile(char* path);

int Aq_Delete_File(char* path);
int Aq_Delete_Dir(char* path);

int Aq_ReadFile(uint32_t* buffer, uint32_t offset, uint32_t size, AqHandle_t* handle);
int Aq_WriteFile_Append(uint32_t* buffer, uint32_t size, AqHandle_t* handle);
int Aq_WriteFile(uint32_t offset, uint32_t *buffer, uint32_t size, AqHandle_t* handle, uint32_t overwrite);

#endif