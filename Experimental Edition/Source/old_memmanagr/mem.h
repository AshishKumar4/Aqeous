#ifndef MEM_H
#define MEM_H
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <common.h>
//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8

//! block size (4k)
#define PMMNGR_BLOCK_SIZE	4096

//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE

u32int GetMemMap(u32int flag,u32int mmap);

void Init_mem();

//! size of physical memory
static	uint32_t	memory_size=0;

//! number of blocks currently in use
static	uint32_t	used_blocks=0;

//! maximum number of available memory blocks
static	uint32_t	max_blocks=0;

//! memory map bit array. Each bit represents a memory block
static	uint32_t*	memory_map= 0;

void kfree(void* p, size_t size);

void*	kmalloc (size_t size);

#endif // MEM_H
