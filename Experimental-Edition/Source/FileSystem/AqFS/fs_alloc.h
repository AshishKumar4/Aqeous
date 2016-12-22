/*** AqFS File System, Aqeous OS's default FS ***/

#ifndef FS_ALLOC_h
#define FS_ALLOC_h

#include "fs.h"

volatile uint32_t bytes;

volatile uint32_t off;

volatile uint32_t start_off;

void fs_alloc_init();

uint64_t fs_alloc(uint32_t blocks);

uint64_t sec_alloc(uint32_t sectors);

void del_blocks(uint64_t location, uint64_t size);

#endif
