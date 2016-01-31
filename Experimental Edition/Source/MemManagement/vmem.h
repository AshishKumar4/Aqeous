#ifndef VMEM_h
#define VMEM_h

int pag=0;
int status=0;
uint32_t maxmem=0;

typedef struct MemMap
{
    uint16_t used;
    uint32_t map;
    uint32_t addr;
    uint32_t* page;
    uint32_t* link;
}MemMap_t;

MemMap_t *Mblock,*tempBlock1,*tempBlock2,*lastBlock,*tempBlock3,*lastBlockAddr,mblock,*Kblock; //6mb of memory map as linked lists of Blocks

struct Multiboot_mmap
{
    uint64_t base;
    uint64_t length;
    uint8_t type;
};

struct memory_region {

	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	reservedt;
	uint32_t	type;
}*mmap_info;

//! different memory regions (in memory_region.type)
char* strMemoryTypes[] = {

	{"Available"},			//memory_region.type==0
	{"Reserved"},			//memory_region.type==1
	{"ACPI Reclaim"},		//memory_region.type==2
	{"ACPI NVS Memory"}		//memory_region.type==3
};


#endif // VMEM_h
