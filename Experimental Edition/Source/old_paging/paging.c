#include <paging.h>
#include <mem.h>
#include <console.h>

void vmmngr_flush_tlb_entry (virtual_addr addr)
{
    virtual_addr *abc;
    abc=addr;
    asm volatile
    (
        "cli\n\t"
        "invlpg (%0)"::"r"(abc):"memory"
     );
     asm volatile("sti");
}

void pmmngr_paging_enable ()
{
	u32int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

int pmmngr_is_paging ()
 {
	uint32_t res=0;
    asm volatile
    (
        "mov %%cr0,%0":"=r"(res)
    );
    if(res&0x80000000)
	return 2;
    return 1;
}

void pmmngr_load_PDBR (physical_addr addr)
{
    asm volatile
    (
        "mov %0,%%cr3"::"r"(addr)
    );
}

physical_addr pmmngr_get_PDBR ()
{
    physical_addr addr;
    asm volatile
    (
        "mov %%cr3,%0":"=r"(addr)
    );
    return addr;
}

/**Page table entry functions**/
inline void pt_entry_add_attrib (pt_entry* e, uint32_t attrib) {
	*e |= attrib;
}

inline void pt_entry_del_attrib (pt_entry* e, uint32_t attrib) {
	*e &= ~attrib;
}

inline void pt_entry_set_frame (pt_entry* e, physical_addr addr) {
	*e = (*e & ~I86_PTE_FRAME) | addr;
}

inline bool pt_entry_is_present (pt_entry e) {
	return e & I86_PTE_PRESENT;
}

inline bool pt_entry_is_writable (pt_entry e) {
	return e & I86_PTE_WRITABLE;
}

inline physical_addr pt_entry_pfn (pt_entry e) {
	return e & I86_PTE_FRAME;
}

/**end**/

/**PAGE DIRECTORY ENTRY FUNCTIONS**/

inline void pd_entry_add_attrib (pd_entry* e, uint32_t attrib) {
	*e |= attrib;
}

inline void pd_entry_del_attrib (pd_entry* e, uint32_t attrib) {
	*e &= ~attrib;
}

inline void pd_entry_set_frame (pd_entry* e, physical_addr addr) {
	*e = (*e & ~I86_PDE_FRAME) | addr;
}

inline bool pd_entry_is_present (pd_entry e) {
	return e & I86_PDE_PRESENT;
}

inline bool pd_entry_is_writable (pd_entry e) {
	return e & I86_PDE_WRITABLE;
}

inline physical_addr pd_entry_pfn (pd_entry e) {
	return e & I86_PDE_FRAME;
}

inline bool pd_entry_is_user (pd_entry e) {
	return e & I86_PDE_USER;
}

inline bool pd_entry_is_4mb (pd_entry e) {
	return e & I86_PDE_4MB;
}

inline void pd_entry_enable_global (pd_entry e) {

}
/**end**/



void* vmmngr_alloc_page (pt_entry* e) {

	//! allocate a free physical frame
	void* p = kmalloc1 ();
	if (!p)
		return 0;

	//! map it to the page
	pt_entry_set_frame (e, (physical_addr)p);
	pt_entry_add_attrib (e, I86_PTE_PRESENT);

	return p;
}

void vmmngr_free_page (pt_entry* e)
 {

	void* p = (void*)pt_entry_pfn (*e);
	if (p)
		kfree1 (p);

	pt_entry_del_attrib (e, I86_PTE_PRESENT);
}

inline pt_entry* vmmngr_ptable_lookup_entry (ptable* p,virtual_addr addr)
{

	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}

inline pd_entry* vmmngr_pdirectory_lookup_entry (pdirectory* p, virtual_addr addr)
{

	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}


inline bool vmmngr_switch_pdirectory (pdirectory* dir)
{

	if (!dir)
		return false;

	_cur_directory = dir;
	pmmngr_load_PDBR (_cur_pdbr);
    asm volatile("mov %0, %%cr3":: "r"(_cur_pdbr));
	return true;
}

pdirectory* vmmngr_get_directory ()
 {

	return _cur_directory;
}


void vmmngr_map_page (void* phys, void* virt) {

   //! get page directory
   pdirectory* pageDirectory = vmmngr_get_directory ();

   //! get page table
   pd_entry* e = &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ((uint32_t) virt) ];
   if ( (*e & I86_PTE_PRESENT) != I86_PTE_PRESENT)
    {      //! page table not present, allocate it
      ptable* table = (ptable*) kmalloc1 ();
      if (!table)
         return;

      //! clear page table
      memset (table, 0, sizeof(ptable));

      //! create a new entry
      pd_entry* entry =
         &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ( (uint32_t) virt) ];

      //! map in the table (Can also just do *entry |= 3) to enable these bits
      pd_entry_add_attrib (entry, I86_PDE_PRESENT);
      pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
      pd_entry_set_frame (entry, (physical_addr)table);
   }
   //! get table
   ptable* table = (ptable*) PAGE_GET_PHYSICAL_ADDRESS ( e );

   //! get page
   pt_entry* page = &table->m_entries [ PAGE_TABLE_INDEX ( (uint32_t) virt) ];

   //! map it in (Can also do (*page |= 3 to enable..)
   pt_entry_set_frame ( page, (physical_addr) phys);
   pt_entry_add_attrib ( page, I86_PTE_PRESENT);
}
u32int* vesapage;
ptable *tablep;
void vmmngr_initialize () {

	//! allocate default page table
	ptable* table = (ptable*) kmalloc1 ();
	if (!table)
		return;

	//! allocates 3gb page table
	ptable* table2 = (ptable*) kmalloc1 ();
	if (!table2)
		return;

    ptable* table3 = (ptable*) kmalloc1 ();
    ptable* table4 = (ptable*) kmalloc1 ();

	//! clear page table
   memset (table, 0, sizeof (ptable)*2);
   	//! 1st few mb are idenitity mapped
	for (int i=0, frame=0x0, virt=0x00000000; i<1024; i++, frame+=4096, virt+=4096)
    {

 		//! create a new page
		pt_entry page=0;
		pt_entry_add_attrib (&page, I86_PTE_PRESENT);
 		pt_entry_set_frame (&page, frame);

		//! ...and add it to the page table
		table2->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
	}	//! map 1mb to 3gb (where we are at)
	for (int i=0, frame=0x100000, virt=0xc0000000; i<1024; i++, frame+=4096, virt+=4096) {

		//! create a new page
		pt_entry page=0;
		pt_entry_add_attrib (&page, I86_PTE_PRESENT);
		pt_entry_set_frame (&page, frame);

		//! ...and add it to the page table
		table->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
	}	//! create default directory table
	for (int i=0, frame=0x0+4096*1, virt=0x00000000+4096*1; i<1024; i++, frame+=4096, virt+=4096)
    {

 		//! create a new page
		pt_entry page=0;
		pt_entry_add_attrib (&page, I86_PTE_PRESENT);
 		pt_entry_set_frame (&page, frame);

		//! ...and add it to the page table
		table3->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
	}/*
	for (int i=0, frame=0x100000+4096*1, virt=0xc0000000+4096*1; i<1024; i++, frame+=4096, virt+=4096) {

		//! create a new page
		pt_entry page=0;
		pt_entry_add_attrib (&page, I86_PTE_PRESENT);
		pt_entry_set_frame (&page, frame);

		//! ...and add it to the page table
		table4->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
	}*/
	pdirectory*	dir = (pdirectory*) kmalloc (6);
	if (!dir)
		return;

	//! clear directory table and set it as current
	memset (dir, 0, sizeof (pdirectory));
		pd_entry* entry = &dir->m_entries [PAGE_DIRECTORY_INDEX (0xc0000000) ];
	pd_entry_add_attrib (entry, I86_PDE_PRESENT);
	pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
	pd_entry_set_frame (entry, (physical_addr)table);

	pd_entry* entry2 = &dir->m_entries [PAGE_DIRECTORY_INDEX (0x00000000) ];
	pd_entry_add_attrib (entry2, I86_PDE_PRESENT);
	pd_entry_add_attrib (entry2, I86_PDE_WRITABLE);
	pd_entry_set_frame (entry2, (physical_addr)table2);

	pd_entry* entry3 = &dir->m_entries [PAGE_DIRECTORY_INDEX (0x00000000+4096*1) ];
	pd_entry_add_attrib (entry3, I86_PDE_PRESENT);
	pd_entry_add_attrib (entry3, I86_PDE_WRITABLE);
	pd_entry_set_frame (entry3, (physical_addr)table3);
/*
	pd_entry* entry4 = &dir->m_entries [PAGE_DIRECTORY_INDEX (0xc0000000+4096*1) ];
	pd_entry_add_attrib (entry4, I86_PDE_PRESENT);
	pd_entry_add_attrib (entry4, I86_PDE_WRITABLE);
	pd_entry_set_frame (entry4, (physical_addr)table4);*/
	tablep=table3;
	//vesapage=entry3;
		//! store current PDBR
	_cur_pdbr = (physical_addr) &dir->m_entries;

	//! switch to our page directory
	vmmngr_switch_pdirectory (dir);

	//! enable paging
	pmmngr_paging_enable ();
}
u32int a=0;
void* vmalloc(size_t size)
{
    void* p=vmmngr_alloc_page(&tablep->m_entries[1+a]);
    for(int i=size;i>0;i--)
    {
        void* abc=vmmngr_alloc_page(&tablep->m_entries[i+a]);
    }
    a=a+size+1;
    //memset((void*)abc,0,size*4096);
    return p;
}
