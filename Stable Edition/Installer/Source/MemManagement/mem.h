#ifndef MEM
#define MEM

#include <common.h>
#include <sys.h>
#include <string.h>
#include <stdio.h>

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define UHEAP_START         0x3000000
#define UHEAP_INITIAL_SIZE  0x7000000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

uint32_t vga;

uint32_t BlockFinder(uint32_t addr);

inline int clearBits(uint32_t map[],uint32_t sz);

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys, int purpose,int packed,int processId);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
uint32_t kmalloc_a(uint32_t sz);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to u32int!
**/
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. It must be page-aligned.
**/
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

/**
   General allocation function.
**/
uint32_t kmalloc(uint32_t sz);

/**
   General deallocation function.
**/
void free(uint32_t* ptr);

uint32_t malloc(uint32_t sz);

uint32_t pmalloc(uint32_t sz);


#endif // MEM
