#ifndef PAGING_h
#define PAGING_h

#include<common.h>
#include <mem.h>
typedef u32int physical_addr;
enum PAGE_PTE_FLAGS {

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
   	I86_PTE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};

typedef uint32_t pt_entry;

extern void 		pt_entry_add_attrib (pt_entry* e, uint32_t attrib);
extern void 		pt_entry_del_attrib (pt_entry* e, uint32_t attrib);
extern void 		pt_entry_set_frame (pt_entry*, physical_addr);
extern bool 		pt_entry_is_present (pt_entry e);
extern bool 		pt_entry_is_writable (pt_entry e);
extern physical_addr	pt_entry_pfn (pt_entry e);

enum PAGE_PDE_FLAGS {

	I86_PDE_PRESENT			=	1,		//0000000000000000000000000000001
	I86_PDE_WRITABLE		=	2,		//0000000000000000000000000000010
	I86_PDE_USER			=	4,		//0000000000000000000000000000100
	I86_PDE_PWT			=	8,		//0000000000000000000000000001000
	I86_PDE_PCD			=	0x10,		//0000000000000000000000000010000
	I86_PDE_ACCESSED		=	0x20,		//0000000000000000000000000100000
	I86_PDE_DIRTY			=	0x40,		//0000000000000000000000001000000
	I86_PDE_4MB			=	0x80,		//0000000000000000000000010000000
	I86_PDE_CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
	I86_PDE_LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
   	I86_PDE_FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
};
//! get directory entry from directory table
//! virtual address
typedef uint32_t virtual_addr;

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


//! a page directery entry
typedef uint32_t pd_entry;

//! page table
typedef struct table {

	pt_entry m_entries[PAGES_PER_TABLE];
}ptable;

//! page directory
typedef struct directoy {

	pd_entry m_entries[PAGES_PER_DIR];
}pdirectory;

extern void		pd_entry_add_attrib (pd_entry* e, uint32_t attrib);
extern void		pd_entry_del_attrib (pd_entry* e, uint32_t attrib);
extern void		pd_entry_set_frame (pd_entry*, physical_addr);
extern bool		pd_entry_is_present (pd_entry e);
extern bool		pd_entry_is_user (pd_entry);
extern bool		pd_entry_is_4mb (pd_entry);
extern bool		pd_entry_is_writable (pd_entry e);
extern physical_addr	pd_entry_pfn (pd_entry e);
extern void		pd_entry_enable_global (pd_entry e);
//! maps phys to virtual address
extern void MmMapPage (void* phys, void* virt);

//! initialize the memory manager
extern void vmmngr_initialize ();

//! allocates a page in physical memory
extern bool vmmngr_alloc_page (pt_entry*);

//! frees a page in physical memory
extern void vmmngr_free_page (pt_entry* e);

//! switch to a new page directory
extern bool vmmngr_switch_pdirectory (pdirectory*);

//! get current page directory
extern pdirectory* vmmngr_get_directory ();

//! flushes a cached translation lookaside buffer (TLB) entry
extern void vmmngr_flush_tlb_entry (virtual_addr addr);

//! clears a page table
extern void vmmngr_ptable_clear (ptable* p);

//! convert virtual address to page table index
extern uint32_t vmmngr_ptable_virt_to_index (virtual_addr addr);

//! get page entry from page table
extern pt_entry* vmmngr_ptable_lookup_entry (ptable* p,virtual_addr addr);

//! convert virtual address to page directory index
extern uint32_t vmmngr_pdirectory_virt_to_index (virtual_addr addr);

//! clears a page directory table
extern void vmmngr_pdirectory_clear (pdirectory* dir);


//! current directory table
pdirectory*		_cur_directory=0;

//! current page directory base register
physical_addr	_cur_pdbr=0;


#endif // PAGING_h
