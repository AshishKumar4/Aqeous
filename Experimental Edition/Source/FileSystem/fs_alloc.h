#ifndef FS_ALLOC_h
#define FS_ALLOC_h

#include "fs.h"

uint32_t bytes;

uint32_t off;

uint32_t start_off;

void fs_alloc_init();

uint64_t fs_alloc(uint32_t blocks);

uint64_t sec_alloc(uint32_t blocks);

void del_blocks(uint64_t location, uint64_t size);

#endif
