#ifndef VMEM_h
#define VMEM_h

#include <paging.h>

int pag=0;
int status=0;
uint32_t maxmem=0;

typedef struct MemMap
{
    uint16_t used;
    uint32_t map;
    uint32_t addr;
    page_t* page;
}MemMap_t;

MemMap_t *Mblock,*tempBlock1,*tempBlock2,*lastBlock,*tempBlock3,*lastBlockAddr,mblock,*Kblock;

typedef struct memory_region {

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


#endif // VMEM_h
