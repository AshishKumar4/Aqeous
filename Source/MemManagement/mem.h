#ifndef MEM
#define MEM

#include <common.h>
#include <sys.h>
#include <string.h>
#include <stdio.h>
#include <tasking.h>
#include <process.h>
#include <vmem.h>

//#define KHEAP_START         0xC0000000
//#define KHEAP_INITIAL_SIZE  0x100000
uint32_t BITMAP_LOCATION=8388608;

//#define UHEAP_START         0x3000000
//#define UHEAP_INITIAL_SIZE  0x7000000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

uint32_t vga;

inline uint32_t Phy_alloc(uint32_t purpose); ///Gives frames for pages

inline void PhyMap_unSet(uint32_t addr);

uint32_t kmalloc_int(uint32_t sz, int align);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
uint32_t kmalloc_a(uint32_t sz);

/**
   General allocation function.
**/
uint32_t kmalloc(uint32_t sz);
uint32_t* tmalloc(uint32_t sz);
uint32_t* smalloc(uint32_t sz);
inline uint32_t Phy_alloc_pg(uint32_t processID);

/**
   General deallocation function.
**/
void kfree(uint32_t* ptr);

inline uint32_t pmalloc(uint32_t id);

uint32_t fsalloc(uint32_t sz);


#endif // MEM
