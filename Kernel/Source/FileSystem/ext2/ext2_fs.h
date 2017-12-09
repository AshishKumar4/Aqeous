
#ifndef FLOPPY_FS
#define FLOPPY_FS

// *          FLOPPY DISK LAYOUT SCHEME
// * Block    Offset    Length            Description
// * byte     0         512               bytes boot record (if present)
// * byte     512       512               bytes additional boot record data (if present)
// *
// * byte     1024      1024              bytes superblock
// * block    2         1 block           block group descriptor table
// * block    3         1 block           block bitmap
// * block    4         1 block           inode bitmap
// * block    5         23 blocks         inode table
// * block    28        1412 blocks       data blocks

#define M_EXT2 0xEF53
#define DISK_SIZE 4*1024*1024
#define _DiskStorage 1

///Defaults
#define EXT2_MAGIC           0xEF53
#define EXT2_BLOCK_SZ        1024              //a default block size of 1024 bytes
#define EXT2_MAX_BLK_GRP_SZ  8 * EXT2_BLOCK_SZ //maximum blocks in a block group, (8192) with block sizes of 1024
#define EXT2_MAX_INODES_GRP  8 * EXT2_BLOCK_SZ //maximum indoes in a block group, (8192) with block sizes of 1024

//the amount of inodes per block group on a floppy
#define EXT2_I_PER_GRP_FLPPY 184          //derived from 23 blocks for inode table (23 * EXT2_BLOCK_SZ) / (sizeof(ext2_inode_t) [128 bytes])

//the amount of inodes per block group on a filesystem with atlast one full size block group (EXT2_MAX_BLK_GRP_SZ)
#define EXT2_I_PER_GRP       214          //derived from 23 blocks for inode table (23 * EXT2_BLOCK_SZ) / (sizeof(ext2_inode_t) [128 bytes])

///OS creators
#define EXT2_OS_LINUX        0    //Linux
#define EXT2_OS_HURD         1    //GNU HURD
#define EXT2_OS_MASIX        2    //MASIX
#define EXT2_OS_FREEBSD      3    //FreeBSD
#define EXT2_OS_LITES        4    //Lites
#define EXT2_OS_JSOS         5    //AqeousOS

///Compresion algorithms
#define EXT2_NO_ALG          0x0  //Binary value of 0x00000000
#define EXT2_LZV1_ALG        0x1  //Binary value of 0x00000001
#define EXT2_LZRW3A_ALG      0x2  //Binary value of 0x00000002
#define EXT2_GZIP_ALG        0x4  //Binary value of 0x00000004
#define EXT2_BZIP2_ALG       0x8  //Binary value of 0x00000008
#define EXT2_LZO_ALG         0x10 //Binary value of 0x00000010

///Superblock related preprocessors
#define EXT2_SBLOCK_OFF      1024 //Superblock is 1024 bytes offset, for partition table
#define EXT2_MAX_MNT_COUNT   20
#define EXT2_CHECK_INTERVAL  3600 //check the file system every hour
#define EXT2_GOOD_OLD_REV    0    //Revision 0
#define EXT2_DYNAMIC_REV     1    //Revision 1 with variable inode sizes, extended attributes, etc.

///File system errors
#define EXT2_VALID_FS        0    //the state of the file system, is ok
#define EXT2_ERROR_FS        1    //the state of the file system, an error has occured (commonly not unmounted properly)

///types of file system errors
#define EXT2_ERRORS_CONTINUE 1    //continue as if nothing has happened
#define EXT2_ERRORS_RO       2    //remount file system as read-only
#define EXT2_ERRORS_PANIC    3    //major error, cause a kernel panic

///Constants relative to the data blocks
//direct blocks
#define EXT2_NDIR_BLOCKS     12                      //direct blocks

//singly indirect blocks
#define EXT2_IND_BLOCK       EXT2_NDIR_BLOCKS        //singly indirect block (12)
#define EXT2_NIND_BLOCK      256                     //singly indirect blocks (calculated by EXT2_BLOCK_SZ / sizeof(uint32_t))

//doubly indirect blocks
#define EXT2_DIND_BLOCK      (EXT2_IND_BLOCK + 1)    //doubly indirect block (13)
#define EXT2_NDIND_BLOCK     (256 * 256)             //doubly indirect blocks (calculated by squaring EXT2_NIND_BLOCK)

//triply indirect blocks
#define EXT2_TIND_BLOCK      (EXT2_DIND_BLOCK + 1)   //triply indirect block (14)
#define EXT2_NTIND_BLOCK     (256 * 256 * 256)       //triply indirect blocks (calculated by tripling EXT2_NIND_BLOCK)

//number of blocks in the inode typedef
#define EXT2_N_BLOCKS        (EXT2_TIND_BLOCK + 1)   //total number of block (15)

///Inode mode values
enum EXT2_IMODE
{
  //-- file type --
  EXT2_I_SOCK = 0xC000,   //socket
  EXT2_I_SLNK = 0xA00,    //symbolic link
  EXT2_I_FILE = 0x800,    //regular file
  EXT2_I_BLK = 0x600,     //block device
  EXT2_I_DIR = 0x400,     //directory
  EXT2_I_CHR = 0x200,     //character device
  EXT2_I_FIFO = 0x100,    //fifo

  //-- process execution user/group override --
  EXT2_I_UID = 0x080,     //Set process User ID
  EXT2_I_GID = 0x040,     //Set process Group ID
  EXT2_I_SBIT = 0x020,    //sticky bit

  //-- access rights --
  EXT2_I_RUSR = 0x0100,   //user read
  EXT2_I_WUSR = 0x0080,   //user write
  EXT2_I_XUSR = 0x0040,   //user execute
  EXT2_I_RGRP = 0x0020,   //group read
  EXT2_I_WGRP = 0x0010,   //group write
  EXT2_I_XGRP = 0x0008,   //group execute
  EXT2_I_ROTH = 0x0004,   //others read
  EXT2_I_WOTH = 0x0002,   //others write
  EXT2_I_XOTH = 0x0001    //others execute
};

///Root privilege access right values, all of the rights combined
enum EXT2_ROOT_ACCESS_RIGHTS
{
  EXT2_RROOT = EXT2_I_RUSR | EXT2_I_RGRP | EXT2_I_ROTH,
  EXT2_WROOT = EXT2_I_WUSR | EXT2_I_WGRP | EXT2_I_WOTH,
  EXT2_XROOT = EXT2_I_XUSR | EXT2_I_XGRP | EXT2_I_XOTH
};

///Dirent type values
enum EXT2_DIRENT
{
  EXT2_UNKOWN,
  EXT2_FILE,
  EXT2_DIR,
  EXT2_CHARD_DEV,
  EXT2_BLOCK_DEV,
  EXT2_FIFO,
  EXT2_SOCKET,
  EXT2_SYMLINK,
  EXT2_HARDLINK,
  EXT2_MOUNTPOINT
};

typedef struct ext2_superblock
{
  uint32_t total_inodes;
  uint32_t total_blocks;
  uint32_t reserved_blocks;
  uint32_t free_blocks;
  uint32_t free_inodes;
  uint32_t first_data_block;
  uint32_t block_size;
  uint32_t fragment_size;
  uint32_t blocks_per_group;
  uint32_t fragments_per_group;
  uint32_t inodes_per_group;
  uint32_t mtime;
  uint32_t wtime;
  uint16_t mnt_count;
  uint16_t max_mnt_count;
  uint16_t magic;
  uint16_t fs_state;
  uint16_t error_handling;
  uint16_t minor_revision_level;
  uint32_t lastcheck;
  uint32_t checkinterval;
  uint32_t creator_os;
  uint32_t revision_level;
  uint16_t uid_reserved;
  uint16_t gid_reserved;
  uint32_t first_inode;
  uint16_t inode_struct_max;
  uint16_t block_group_number;
  uint32_t feature_compatibility;
  uint32_t feature_incompat;
  uint32_t feature_ro_compat;
  uint16_t unique_id[8];
  char volume_name[16];
  char last_mounted_on[64];
  uint32_t compression_info;/*
  uint8_t prealloc_blocks;
  uint8_t prealloc_dir_blocks;
  uint16_t reserved_gdt_blocks;
  uint8_t journal_uuid[16];
  uint32_t journal_inum;
  uint32_t journal_dev;
  uint32_t last_orphan;
  uint32_t hash_seed[4];
  uint8_t def_hash_version;
  uint8_t jnl_backup_type;
  uint16_t group_desc_size;
  uint32_t default_mount_opts;
  uint32_t first_meta_bg;
  uint32_t mkfs_time;
  uint32_t jnl_blocks[17];*/
}ext2_superblock_t;
/*
typedef struct ext2_superblock
{
  uint32_t total_inodes;
  uint32_t total_blocks;
  uint32_t reserved_blocks;
  uint32_t total_unblocks;
  uint32_t total_uninodes;
  uint32_t first_data_block;
  uint32_t block_size;
  uint32_t fragment_size;
  uint32_t blocks_per_group;
  uint32_t fragments_per_group;
  uint32_t inodes_per_group;
  uint32_t mtime; //mount time
  uint32_t wtime; //write time
  uint16_t mnt_count;
  uint16_t max_mnt_count; //max amount of mounts before a check
  uint16_t magic;
  uint16_t fs_state;
  uint16_t error_handling;
  uint16_t minor_revision_level;
  uint32_t lastcheck; //the last time the file system was checked
  uint32_t checkinterval; //maximum amount of time the file system is allowed before being checked
  uint32_t creator_os;
  uint32_t revision_level;
  uint32_t first_inode;
  uint16_t inode_struct_max;
  uint16_t block_group_number;
  uint32_t feature_compatibility;
  uint32_t feature_incompat;
  uint32_t feature_ro_compat;
  uint32_t unique_id[4];
  char volume_name[16];
  char last_mounted_on[64];
  uint32_t compression_info;
}ext2_superblock_t;
//*/
typedef struct ext2_group_descriptor
{
  uint32_t block_bitmap;
  uint32_t inode_bitmap;
  uint32_t inode_table_id;
  uint16_t free_blocks;
  uint16_t free_inodes;
  uint16_t used_dirs;
  uint16_t pad;
  uint32_t inode_table_size;  //the amount of EXT2_BLOCK_SZ the inode table is
  uint32_t gdesc_location;
  uint32_t reserved;
}ext2_group_descriptor_t;

typedef struct ext2_inode
{
  uint8_t magic;
  uint32_t inode;
  uint16_t mode;
  uint32_t type;
  uint16_t uid;
  uint32_t size;
  uint32_t atime;
  uint32_t ctime;
  uint32_t mtime;
  uint32_t dtime;
  uint16_t gid;
  uint16_t nlinks;  //number of hard links
  uint16_t nblocks; //blocks of 512 bytes
  uint16_t flags;
  uint16_t osd1;
  uint32_t blocks[EXT2_N_BLOCKS];
  uint16_t version;
  uint16_t fire_acl;
  uint16_t dir_acl;
  uint16_t fragment_addr;
  uint16_t osd2[3];
  uint8_t reserved[13];
}ext2_inode_t;

struct ext2_dirent
{
  uint32_t ino;               //inode number. Required by POSIX.
  uint16_t rec_len;           //bytes from begining of this dirent to end of this dirent, size of file
  uint8_t name_len;           //the number of bytes of charachters in the name
  uint8_t file_type;          //a flag that states what type of file this dirent is (ie: a file, pipe, directory, etc.)
  char *name;           //filename, remember to kmalloc this to give it an address, or else it will page fault
};

typedef struct ext2_dirent ext2_dirent_t;

#define TO_uint32_t(bytes)                   ((bytes) / sizeof(uint32_t))

#define BLOCKS_TO_SECTORS(blocks)          (((blocks) * EXT2_BLOCK_SZ) / SECTOR_SIZE)
#define SECTORS_TO_BLOCKS(sectors)         (((sectors) * SECTOR_SIZE) / EXT2_BLOCK_SZ)

#define IS_CONS_BLOCKS(first, second)      ((((second) - (first)) / (EXT2_BLOCK_SZ / SECTOR_SIZE)) == 1 ? TRUE : FALSE)

//the global path for the current directory
char *ext2_path;
uint32_t ext2_current_dir_inode = 0;
ext2_inode_t *ext2_root;
char *ext2_root_name;

//caches
ext2_superblock_t *ext2_g_sblock = 0;
ext2_group_descriptor_t *ext2_g_gdesc = 0;
ext2_inode_t *ext2_g_inode_table = 0;
uint8_t *ext2_g_bb = 0;                                   //the block bitmap
uint8_t *ext2_g_ib = 0;                                    //the inode bitmap
//caches

//defaults (logged as user) for permisions files need to have in order to be accesed
uint32_t _Rlogged = EXT2_I_RUSR, _Wlogged = EXT2_I_WUSR, _Xlogged = EXT2_I_XUSR;

static ext2_inode_t *__create_root__(void);
static ext2_inode_t *__create_file__(uint32_t size);
static ext2_inode_t *__create_dir__(ext2_superblock_t *sblock, ext2_group_descriptor_t *gdesc);
static char *__get_name_of_file__(ext2_inode_t *directory, ext2_inode_t *file);

static struct ext2_dirent dirent;

enum __block_types__
{
	EXT2_DIRECT,
	EXT2_SINGLY,
	EXT2_DOUBLY,
	EXT2_TRIPLY
};

void* Ext2_currDir;//ptr_currentDir;

//adding a hardlink to a directory is exactly the same as adding a file to a directory, so I just make an alias
#define ext2_add_hardlink_to_dir(parent_dir, file, filename)    (ext2_add_file_to_dir(parent_dir, file, EXT2_HARDLINK, filename))

/*set the block group of a floppy*/
uint32_t ext2_set_block_group(uint32_t size);

/*set the default super block data*/
void ext2_sblock_set_data(ext2_superblock_t *data, uint32_t reserved_blocks, uint32_t sblock_location,
                          uint32_t error_handling, uint32_t sblock_group_num, char *partition_name);

/*set the default group descriptor data*/
void ext2_set_gdesc_table(ext2_group_descriptor_t *data);

/*place the meta data (sblock and gdesc) into the ram to be easily read*/
uint32_t ext2_read_meta_data(ext2_superblock_t **sblock, ext2_group_descriptor_t **gdesc);

/*create a file*/
ext2_inode_t *ext2_create_file(ext2_inode_t *parent_dir, char *name, uint32_t size);

/*create a directory*/
ext2_inode_t *ext2_create_dir(ext2_inode_t *parentNode, char *name);

/*formats the block bitmap by flipping the correct bits*/
uint32_t *ext2_format_block_bitmap(ext2_group_descriptor_t *gdesc, uint32_t blocks_used);

/*writes the locations of the blocks to the inode entries blocks data*/
uint32_t ext2_inode_entry_blocks(ext2_inode_t *node, ext2_group_descriptor_t *gdesc, uint32_t *block_locations, uint32_t blocks_used);

/*write an inode data to the inode table*/
uint32_t ext2_data_to_inode_table(ext2_inode_t *data, ext2_group_descriptor_t *gdesc, ext2_superblock_t *sblock);

/*creates a singly block*/
uint32_t ext2_singly_create(uint32_t *block_locations, uint32_t offset, uint32_t nblocks, ext2_group_descriptor_t *gdesc);

/*creates a doubly block*/
uint32_t ext2_doubly_create(uint32_t *block_locations, uint32_t offset, uint32_t nblocks, ext2_group_descriptor_t *gdesc);

/*adds a file to a directory*/
uint32_t ext2_add_file_to_dir(ext2_inode_t *parent_dir, ext2_inode_t *file, uint32_t file_type, char *name);

/*read this block in the block set of a file's inode*/
uint32_t ext2_block_of_set(ext2_inode_t *file, uint32_t block_number, uint32_t *block_output);

/*returns the inode data at a specific locaiton*/
uint32_t ext2_inode_from_inode_table(uint32_t inode_number, ext2_inode_t *output, ext2_group_descriptor_t *gdesc);

/*initialize the ext2 filesystem*/
uint32_t ext2_burn(uint32_t size, const char* device);
uint32_t ext2_initialize(uint32_t size, const char *device);

/*find a dirent by index from a directory*/
struct ext2_dirent *ext2_dirent_from_dir(ext2_inode_t *dir, uint32_t index);

/*gets a file's inode from a directory*/
ext2_inode_t *ext2_file_from_dir(ext2_inode_t *dir, char *name);

/*returns an inode with the inode table as input*/
ext2_inode_t *ext2_inode_from_offset(uint32_t inode_number);

/*returns the whole inode table data*/
ext2_inode_t *ext2_get_inode_table(ext2_group_descriptor_t *gdesc);

/*writes data to a specific block*/
uint32_t ext2_write_block_of_set(ext2_inode_t *file, uint32_t block_number, uint32_t *block_data, uint32_t size);

/*gets a dirent from a directory with input data*/
struct ext2_dirent *ext2_dirent_from_dir_data(ext2_inode_t *dir, uint32_t index, uint32_t *data);

/*finds an open inode*/
uint32_t ext2_find_open_inode(ext2_superblock_t *sblock, ext2_group_descriptor_t *gdesc);

/*returns a list of the blocks the file has*/
uint32_t *ext2_block_locs(ext2_inode_t *node);

/*reads a chunk of data in a file and returns it*/
uint32_t *ext2_chunk_data(uint32_t *blocks, uint32_t nblocks, uint32_t chunk, uint32_t *out_chunk_size);

/*returns the blocks in a singly block*/
uint32_t *ext2_get_singly(uint32_t location, uint32_t *nblocks);

/*returns the blocks in a doubly block*/
uint32_t *ext2_get_doubly(uint32_t location, uint32_t *nblocks);

/*returns the blocks in a triply block*/
uint32_t *ext2_get_triply(uint32_t location, uint32_t *nblocks);

/*expand a nodes size*/
uint32_t ext2_expand(ext2_inode_t *node, uint32_t increase_bytes);

/*remove a list of blocks from the block bitmap*/
uint32_t ext2_free_blocks(uint32_t *block_locs, uint32_t nblocks);

/*deletes a file/directory node*/
uint32_t ext2_delete(ext2_inode_t *parent_dir, ext2_inode_t *node);

/*removes an inode entry in the inode table and flips the bit in the inode bitmap*/
uint32_t ext2_remove_inode_entry(ext2_inode_t *node);

/*get the name of an ext2 directory node*/
char *ext2_get_name_of_dir(ext2_inode_t *directory);

/*removes a dirent from a directory*/
uint32_t ext2_remove_dirent(ext2_inode_t *directory, ext2_inode_t *node);

/*frees the data blocks of a node*/
uint32_t ext2_free_data_blocks(ext2_inode_t *directory, ext2_inode_t *node);

/*read from a node's data*/
uint32_t ext2_read(ext2_inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);

/*write to a node's data*/
uint32_t ext2_write(ext2_inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);

/*update a specific node in the inode table*/
uint32_t ext2_update_node_in_inode_table(ext2_inode_t *node);

#endif
