#ifndef PAGING_H
#define PAGING_H

#include <common.h>
#include <interrupts.h>

uint32_t mb_temp=0,max_mem;

enum PAGE_PTE_FLAGS //Pages
{
	I86_PTE_NOT_PRESENT			=	0,		//0000000000000000000000000000000
	I86_PTE_PRESENT			=	1,		//0000000000000000000000000000001
	I86_PTE_WRITABLE		=	2,		//0000000000000000000000000000010
	I86_PTE_USER			=	4,		//0000000000000000000000000000100
	I86_PTE_WRITETHOUGH		=	8,		//0000000000000000000000000001000
	I86_PTE_NOT_CACHEABLE		=	0x10,		//0000000000000000000000000010000
	I86_PTE_ACCESSED		=	0x20,		//0000000000000000000000000100000
	I86_PTE_DIRTY			=	0x40,		//0000000000000000000000001000000
	I86_PTE_PAT			=	0x80,		//0000000000000000000000010000000
	I86_PTE_CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
	I86_PTE_LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
	CUSTOM_PTE_AVAIL_1		=	0x400,		//0000000000000000000010000000000
	CUSTOM_PTE_AVAIL_2		=	0x800,		//0000000000000000000100000000000
   	I86_PTE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};

//! page table entry
typedef uint_fast32_t table_t;

enum PAGE_PDE_FLAGS
{
	I86_PDE_NOT_PRESENT			=	0,		//0000000000000000000000000000000
	I86_PDE_PRESENT			=	1,		//0000000000000000000000000000001
	I86_PDE_WRITABLE		=	2,		//0000000000000000000000000000010
	I86_PDE_USER			=	4,		//0000000000000000000000000000100
	I86_PDE_PWT			=	8,		//0000000000000000000000000001000
	I86_PDE_PCD			=	0x10,		//0000000000000000000000000010000
	I86_PDE_ACCESSED		=	0x20,		//0000000000000000000000000100000
	I86_PDE_DIRTY			=	0x40,		//0000000000000000000000001000000
	I86_PDE_4MB			=	0,		//0000000000000000000000010000000
	I86_PDE_CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
	I86_PDE_LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
	CUSTOM_PDE_AVAIL_1		=	0x400,		//0000000000000000000010000000000
	CUSTOM_PDE_AVAIL_2		=	0x800,		//0000000000000000000100000000000
   	I86_PDE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};

//! a page directery entry
typedef uint_fast32_t page_t;

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

//! page table
typedef struct ptable_t
{
	page_t m_entries[PAGES_PER_TABLE] __attribute__((aligned(0x1000)));
}ptable;

//! page directory
typedef struct pdirectory_t
{
	table_t m_entries[PAGES_PER_DIR] __attribute__((aligned(0x1000)));
}pdirectory;

pdirectory* main_dir,*system_dir,*user_dir;
//! current directory table (global)
pdirectory*		_cur_directory=0,*_prev_directory=0;

typedef struct  ///Array to keep the info of all the page directories made.
{
	pdirectory* pgdir; ///location of page directory in kernel
	uint16_t reserved;
	uint16_t ID; ///page ID
}PgDirs_t;

PgDirs_t PgDirs[100]; ///currently, only 100 max possible page directories.
PgDirs_t curr_pgdir;
uint8_t PgDs=0; ///Page Directories

inline page_t* Get_Page(uint32_t addr, int make, pdirectory* dir);
inline page_t* get_page(uint32_t addr,int make, pdirectory* dir);
inline void Map_non_identity(uint32_t virt, uint32_t phys, uint32_t size, pdirectory* dir);
inline void System_dir_setup();
inline void Switch_to_system_dir();
inline void Switch_back_from_System();
inline pdirectory* system_dir_maker();
int Switch_Page_Directory(pdirectory* dir);
void page_fault(registers_t regs);
inline page_t* MapPage(void* phys, void* virt, pdirectory* dir);
void map(uint32_t phy,size_t size, pdirectory* dir);
void free_page (page_t* e);
inline table_t* get_table(uint32_t index,int make, pdirectory* dir);

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
