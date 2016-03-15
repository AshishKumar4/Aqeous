
#include <paging.h>
#include <mem.h>

void switch_directory(pdirectory *dir)
{
    //asm volatile ("movl %%eax, %%cr3" :: "a" (&dir->m_entries[0])); // load PDPT into CR3
    asm volatile("mov %0, %%cr3":: "r"((uint32_t)dir));
    u32int cr0=0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

int alloc_page (page_t* e)
{
	//! allocate a free physical frame
	void* p = (void*)pmalloc(4096);
	if (!p)
		return 0;

	//! map it to the page
	pt_entry_set_frame (e, (uint32_t)p);
	pt_entry_add_attrib (e, I86_PTE_PRESENT);

	return 1;
}

void free_page (page_t* e)
{
	void* p = (void*)pt_entry_pfn (*e);
	if (p)
		free (p);

	pt_entry_del_attrib (e, I86_PTE_PRESENT);
}

inline page_t* ptable_lookup_entry (ptable* p,uint32_t addr)
{
	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}

inline page_t* pdirectory_lookup_entry (pdirectory* p, uint32_t addr)
{
	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}
//! current directory table (global)
pdirectory*		_cur_directory=0;

inline int switch_pdirectory (pdirectory* dir)
{
	if (!dir)
		return 0;
	_cur_directory = dir;
  switch_directory(dir);
	return 1;
}

pdirectory* get_directory ()
{
	return main_dir;
}

void flush_tlb_entry (uint32_t addr)
{
  asm volatile("cli");
  asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
  asm volatile("sti");
}

page_t* MapPage (void* phys, void* virt)
{
   //! get page directory
   pdirectory* pageDirectory = main_dir;

   //! get page table
   table_t* e = &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ((uint32_t) virt) ];
   if ( (*e & I86_PTE_PRESENT) != I86_PTE_PRESENT)
   {
      //! page table not present, allocate it
      ptable* table = (ptable*) pmalloc(4096);
      if (!table)
         return 0;

      //! clear page table
      //memset (table, 0, sizeof(ptable));

      //! create a new entry
      table_t* entry =
         &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ( (uint32_t) virt) ];

      //! map in the table (Can also just do *entry |= 3) to enable these bits
      pd_entry_add_attrib (entry, I86_PDE_PRESENT);
      pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
      pd_entry_set_frame (entry, (uint32_t)table);
   }

   //! get table
   ptable* table = (ptable*) PAGE_GET_PHYSICAL_ADDRESS ( e );

   //! get page
   page_t* page = &table->m_entries [ PAGE_TABLE_INDEX ( (uint32_t) virt) ];

   //! map it in (Can also do (*page |= 3 to enable..)
   pt_entry_set_frame ( page, (uint32_t) phys);
   pt_entry_add_attrib ( page, I86_PTE_PRESENT);
   return page;
}

void map(uint32_t phy,size_t size)
{
    uint32_t j=phy;
    for (; j < phy+size;j+=0x1000)
    {
      MapPage((void*)j,(void*)j);
    }
}


page_t* get_page(uint32_t addr,int make, pdirectory* dir)
{

      // Turn the address into an index.
      addr /= 0x1000;
      // Find the page table containing this address.
      uint32_t table_idx = addr / 1024;

      if (dir->m_entries[table_idx]) // If this table is already assigned
      {
          table_t* entry = &dir->m_entries [table_idx];
          ptable* table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(entry);
          return &table->m_entries[addr%1024];
      }
      else if(make)
      {
          dir->m_entries[table_idx] = (table_t)pmalloc(sizeof(ptable));
          memset((void*)dir->m_entries[table_idx], 0, 0x1000);
          table_t* entry = &dir->m_entries [table_idx];
          ptable* table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(entry);
          pd_entry_add_attrib (entry, I86_PDE_PRESENT);
          pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
          pd_entry_set_frame (entry, (uint32_t)dir->m_entries[table_idx]);
          //dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
          return &table->m_entries[addr%1024];
      }
      else
      {
          printf(" Cant get the Page! ");
          return 0;
      }
}

void initialise_paging()
{
  main_dir = (pdirectory*) pmalloc (sizeof(pdirectory));
  //! clear directory table and set it as current
  memset (main_dir, 0, sizeof (pdirectory));
}

void enable_paging()
{

      printf("Allocating Pages and Page tables, This may take a while\n");
      MemMap_t* tempBlock3=Mblock;
      for(uint32_t i=0;i<(1024*1024*300);i+=4096) //Make the pages and page tables for the whole kernel memory (100mb)
      {
          page_t* page;
          page=get_page(i,1,main_dir); //kernel Pages;
          pt_entry_set_frame ( page, i);
          pt_entry_add_attrib ( page, I86_PTE_PRESENT);
          pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
          pt_entry_add_attrib ( page, I86_PTE_USER);
          tempBlock3->page=page;
          tempBlock3++;
          //page++;
      }
      for(uint32_t i=300*1024*1024;i<(1024*maxmem);i+=4096) //For the rest reserved memory!
      {
          if(tempBlock3->used==1) // memory is reserved, identity map it, i dont want any issues!
          {
            page_t* page;
            page=get_page(i,1,main_dir); //kernel Pages;
            pt_entry_set_frame ( page, i);
            pt_entry_add_attrib ( page, I86_PTE_PRESENT);
            pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
            pt_entry_add_attrib ( page, I86_PTE_USER);
            tempBlock3->page=page;
          }
          ++tempBlock3;
          //page++;
      }
      for(uint32_t i=maxmem/4;i<(1024*1024);i++) //For the rest reserved memory!
      {
          page_t* page;
          page=get_page(i*4096,1,main_dir); //kernel Pages;
          pt_entry_set_frame ( page, i*409);
          pt_entry_add_attrib ( page, I86_PTE_PRESENT);
          pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
          pt_entry_add_attrib ( page, I86_PTE_USER);
          //page++;
      }
   //! switch to our page directory
   switch_pdirectory (main_dir);

   register_interrupt_handler(14, page_fault);
   pag=1;
   printf("\nEnabled paging\n");
   //pmmngr_paging_enable (true);
}


void page_fault(registers_t regs)
{
  asm volatile("cli");
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    console_writestring("\nPage fault! ( ");
    if (present)
    {
      console_writestring("present, Allocating page for it ");
      MapPage((void*)faulting_address,(void*)faulting_address);
    }
    if (rw)
    {
        console_writestring("read-only ");
    }
    if (us) {console_writestring("user-mode ");}
    if (reserved) {console_writestring("reserved ");}

    if (id) {console_writestring("id "); console_write_dec(id);}
    console_writestring(") at 0x");
    console_write_dec(faulting_address);
    console_writestring(" - EIP: ");
    console_write_dec(regs.eip);
    console_writestring("\n");
    asm volatile("         \
     cli;                 \
     mov %0, %%ecx;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     sti;                 \
     jmp *%%ecx           "
                 : : "r"(regs.eip), "r"(regs.esp), "r"(regs.ebp));
    asm volatile("sti");
    asm volatile("iret");
   // PANIC("Page fault");
}
