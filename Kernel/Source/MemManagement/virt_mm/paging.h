#ifndef PAGING_H
#define PAGING_H

//! i86 architecture defines 1024 entries per table--do not change
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR	1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

//! page table represents 4mb address space
#define PTABLE_ADDR_SPACE_SIZE 0x400000

//! directory table represents 4gb address space
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

//! page sizes are 4k
#define PAGE_SIZE 4096

typedef uint_fast32_t page_t;
typedef uint_fast32_t table_t;

typedef struct __attribute__((packed)) PageDirectory
{
    table_t table_entry[PAGES_PER_DIR];
}PageDirectory_t;

PageDirectory_t* system_dir;
PageDirectory_t* _cur_dir,* _prev_dir, *TemplatePgDir;

typedef struct __attribute__((packed)) PageTable
{
    page_t page_entry[PAGES_PER_TABLE];
}PageTable_t;

PageTable_t* TemplatePgTbl_Array;

uint32_t cumulative_Knlpgdsz = 0;

typedef struct __attribute__((packed)) Pdir_Capsule
{
    PageDirectory_t pdir;
    uint32_t csrb_f[1024];
    uint32_t csrb_u[1024];
}Pdir_Capsule_t;

Pdir_Capsule_t* system_pdirCap;
Pdir_Capsule_t* _cur_pdirCap, *_prev_pdirCap;

enum PAGE_PTE_FLAGS //Pages
{
	I86_PTE_NOT_PRESENT		=	0,		    //0000000000000000000000000000000
	I86_PTE_PRESENT			=	1,  		//0000000000000000000000000000001
	I86_PTE_WRITABLE		=	2,  		//0000000000000000000000000000010
	I86_PTE_USER			=	4,  		//0000000000000000000000000000100
	I86_PTE_WRITETHOUGH		=	8,  		//0000000000000000000000000001000
	I86_PTE_NOT_CACHEABLE	=	0x10,		//0000000000000000000000000010000
	I86_PTE_ACCESSED		=	0x20,		//0000000000000000000000000100000
	I86_PTE_DIRTY			=	0x40,		//0000000000000000000000001000000
	I86_PTE_PAT		    	=	0x80,		//0000000000000000000000010000000
	I86_PTE_CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
	I86_PTE_LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
	CUSTOM_PTE_AVAIL_1		=	0x400,		//0000000000000000000010000000000
	CUSTOM_PTE_AVAIL_2		=	0x800,		//0000000000000000000100000000000
    I86_PTE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};

enum PAGE_PDE_FLAGS
{
	I86_PDE_NOT_PRESENT		=	0,	    	//0000000000000000000000000000000
	I86_PDE_PRESENT			=	1,	    	//0000000000000000000000000000001
	I86_PDE_WRITABLE		=	2,  		//0000000000000000000000000000010
	I86_PDE_USER			=	4,		    //0000000000000000000000000000100
	I86_PDE_PWT		    	=	8,	    	//0000000000000000000000000001000
	I86_PDE_PCD		    	=	0x10,		//0000000000000000000000000010000
	I86_PDE_ACCESSED		=	0x20,		//0000000000000000000000000100000
	I86_PDE_DIRTY			=	0x40,		//0000000000000000000000001000000
	I86_PDE_4MB			    =	0,		    //0000000000000000000000010000000
	I86_PDE_CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
	I86_PDE_LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
	CUSTOM_PDE_AVAIL_1		=	0x400,		//0000000000000000000010000000000
	CUSTOM_PDE_AVAIL_2		=	0x800,		//0000000000000000000100000000000
   	I86_PDE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};

void SystemDir_Mapper();

void Setup_SystemDir();

Pdir_Capsule_t* pgdir_maker();

/****/
uint32_t kernelPgDir_Builder(PageDirectory_t* dir);
void MapKernelPages(PageDirectory_t* dir);
void kernelPgDir_MapMaker(PageDirectory_t* dir);
inline void Kernel_Mapper(PageDirectory_t* dir);

inline void Map_non_identity(uint32_t phys, uint32_t virt, uint32_t size, PageDirectory_t* dir);

inline void Unmap_non_identity(uint32_t phys, uint32_t virt, uint32_t size, PageDirectory_t* dir);

inline void Create_PTable(uint32_t phy, PageTable_t* tbl);

void flush_tlb_entry (uint32_t addr);

inline page_t* MapPage (void* phys, void* virt, PageDirectory_t* dir);

void map(uint32_t phy,size_t size, PageDirectory_t* dir);
void map_readOnly(uint32_t phy,size_t size, PageDirectory_t* dir);
void map_kernelOnly(uint32_t phy,size_t size, PageDirectory_t* dir);

page_t* get_page(uint32_t addr,int make, PageDirectory_t* dir);
/****/

void SwitchTo_SysDir();

void SwitchFrom_SysDir();

void switch_directory(PageDirectory_t *dir);

inline void switch_pCap(Pdir_Capsule_t* pcap);

inline page_t* get_pageEntry(uint32_t addr);

inline table_t* get_tableEntry(uint32_t addr);

inline PageTable_t* get_table(uint32_t addr);

uint32_t get_phyAddr(uint32_t addr, PageDirectory_t* dir);

void Setup_Paging();


inline uint32_t pt_get_frame(page_t* e)
{
	return (*e & I86_PTE_FRAME);
}

inline void pt_entry_add_attrib (page_t* e, uint32_t attrib)
{
	*e |= attrib;
}

inline void pt_entry_del_attrib (page_t* e, uint32_t attrib)
{
	*e &= ~attrib;
}

inline void pt_entry_set_frame (page_t* e, uint32_t addr)
{
	*e = (*e & ~I86_PTE_FRAME) | addr;
}

inline int pt_entry_is_present (page_t e)
{
	return e & I86_PTE_PRESENT;
}

inline int pt_entry_is_writable (page_t e)
{
	return e & I86_PTE_WRITABLE;
}

inline uint32_t pt_entry_pfn (page_t e)
{
	return e & I86_PTE_FRAME;
}

inline int pt_entry_is_avail_1 (page_t e)
{
	return e & CUSTOM_PTE_AVAIL_1;
}

inline int pt_entry_is_avail_2 (page_t e)
{
	return e & CUSTOM_PTE_AVAIL_2;
}

inline void pd_entry_add_attrib (table_t* e, uint32_t attrib)
{
	*e |= attrib;
}

inline void pd_entry_del_attrib (table_t* e, uint32_t attrib)
{
	*e &= ~attrib;
}

inline void pd_entry_set_frame (table_t* e, uint32_t addr)
{
	*e = (*e & ~I86_PDE_FRAME) | addr;
}

inline int pd_entry_is_present (table_t e)
{
	return e & I86_PDE_PRESENT;
}

inline bool pd_entry_is_writable (table_t e)
{
	return e & I86_PDE_WRITABLE;
}

inline uint32_t pd_entry_pfn (table_t e)
{
	return e & I86_PDE_FRAME;
}

inline int pd_entry_is_user (table_t e)
{
	return e & I86_PDE_USER;
}

inline int pd_entry_is_4mb (table_t e)
{
	return e & I86_PDE_4MB;
}

inline int pd_entry_is_avail_1 (table_t e)
{
	return e & CUSTOM_PDE_AVAIL_1;
}

inline int pd_entry_is_avail_2 (table_t e)
{
	return e & CUSTOM_PDE_AVAIL_2;
}

#endif
