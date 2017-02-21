#ifndef MEM_H
#define MEM_H

#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
//#include "streams.h"
#include "CustomCSRB/csrb_custom.h"

#include "virt_mm/paging.h"

#define FRAME_STACK_START 10240*1024 //8th M

uint32_t vga;
uint32_t maxmem=0;
uint32_t memAvailable=0;
uint16_t blockID=10;
uint32_t max_mem = 0;

typedef struct memory_region
{
	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	reservedt;
	uint32_t	type;
}MemRegion_t;

MemRegion_t *mmap_info, *Fmemmap;

//! different memory regions (in memory_region.type)
char* strMemoryTypes[] = {

	"Available",				//memory_region.type==0
	"Reserved",					//memory_region.type==1
	"ACPI Reclaim",				//memory_region.type==2
	"ACPI NVS Memory"			//memory_region.type==3
	"Kernel Reserved"			//memory_region.type==4
	"Kernel Page Directory"		//memory_region.type==4
};

uint32_t* frame_stack_end = 0;

void memmap_generator();

void setup_frameStack();

uint32_t pop_frameStack();

void push_frameStack(uint32_t fr);

uint32_t phy_alloc4K();

void Setup_PhyMEM();

void* pmem(uint32_t size);

void pfree(void* addr);

inline uint32_t pmalloc();

uint32_t mtalloc(uint32_t pages);

void mtfree(uint32_t addr, uint32_t size);

uint32_t fsalloc(uint32_t sz);

#endif
