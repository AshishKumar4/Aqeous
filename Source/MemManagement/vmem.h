#ifndef VMEM_h
#define VMEM_h

#include <paging.h>
#include <mem.h>

int pag=0;
int status=0;
uint32_t maxmem=0;
uint32_t memAvailable=0;
uint16_t blockID=10;

typedef struct __attribute__ ((packed)) __attribute__ ((aligned (8))) PageFrameMap
{
    uint32_t used;
    uint32_t frame;
}FrameMap_t;

FrameMap_t* StartFrame;

typedef struct __attribute__ ((packed)) __attribute__ ((aligned (32))) MemMap
{
    uint32_t id;
    uint32_t map[4];
    uint32_t addr;
    uint16_t used;
    uint16_t reserved;
    page_t* page;
}MemMap_t;

MemMap_t *Mblock,*lastBlock,*lastBlockAddr,mblock,*Kblock;

typedef struct memory_region
{
	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	reservedt;
	uint32_t	type;
}MemRegion_t;

MemRegion_t *mmap_info;

//! different memory regions (in memory_region.type)
char* strMemoryTypes[] = {

	"Available",			//memory_region.type==0
	"Reserved",			//memory_region.type==1
	"ACPI Reclaim",		//memory_region.type==2
	"ACPI NVS Memory"		//memory_region.type==3
};

uint32_t VMem_Alloc(uint32_t sz, int align, uint32_t *phys,int purpose,int packed,int processId);
inline uint32_t BlockFinder(uint32_t addr); /// returns the corresponding memory block for a given physical address

#endif // VMEM_h
