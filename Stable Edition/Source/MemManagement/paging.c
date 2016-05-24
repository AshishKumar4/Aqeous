#include <paging.h>
#include <vmem.h>
#include <console.h>
#include <mem.h>

inline void switch_directory(pdirectory *dir)
{
    //asm volatile ("movl %%eax, %%cr3" :: "a" (&dir->m_entries[0])); // load PDPT into CR3
    asm volatile("mov %0, %%cr3":: "r"((uint32_t)dir):"memory");
}

inline pdirectory* pgdir_maker()
{
    Switch_to_system_dir();
    pdirectory* dir=(pdirectory*)pmalloc(2);
    memset((void*)dir, 0, 4096);
    PgDirs[PgDs].pgdir=dir;
    PgDirs[PgDs].ID=PgDs+1;
    ++PgDs;
    for(int i=0; i<1024; i++) //Make all page tables in advance to increase performance
    {
      dir->m_entries[i] = (table_t)pmalloc(2);
      memset((void*)dir->m_entries[i], 0, 0x1000);
      table_t* entry = &dir->m_entries[i];
      pd_entry_add_attrib (entry, I86_PDE_PRESENT);
      pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
    }
    Switch_back_from_System();
    return dir;
}

inline pdirectory* system_dir_maker()
{
    pdirectory* dir=(pdirectory*)(4194304);
    PgDirs[PgDs].pgdir=dir;
    PgDirs[PgDs].ID=PgDs+1;
    ++PgDs;
    for(int i=0;i<1024;i++)
    {
      dir->m_entries[i] = (table_t)(((i+1)*4096)+4194304);
      table_t* entry = &dir->m_entries[i];
      pd_entry_add_attrib (entry, I86_PDE_PRESENT);
      pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
      //pd_entry_set_frame (entry, (uint32_t)dir->m_entries[i]);
    }
    return dir;
}

inline void Kernel_Mapper(pdirectory* dir) ///To Map the Kernel in a given pdirectory
{
    map(0, 8*1024*1024, dir);
  //  map(50331648, 12*1024*1024, dir);
  //  map(200*1024*1024, 300*1024*1024, dir);
    /**Originally kernel resides from 100th mb physical. Here we just map it to 3GB of the page dir**/
    //Map_non_identity(8*1024*1024, 0xC0000000, 91*1024*1024, dir);
    Map_non_identity((uint32_t)VGA_buffer, 0xCF000000, 8*1024*1024, dir);
    map(0xF0000000, 0xFFFFF000-0xF0000000, dir);
    map((uint32_t)VGA_buffer, 8192*1024, dir);
    //while(1);
}

inline void System_dir_setup() ///will use it to manage OS directly
{
    //Map_non_identity(0,0,4095*1024*1024,system_dir);
    for(uint32_t i=0,j=0;i<0xFFFFF000;i+=4096,j+=4096) //Make the pages and page tables for the whole kernel memory (higher Half)
    {
        //printf(" %x",i);
        page_t* page;
        page=get_page(j,0,system_dir); //kernel Pages;
        pt_entry_set_frame ( page, i);
        pt_entry_add_attrib ( page, I86_PTE_PRESENT);
        pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
        pt_entry_add_attrib ( page, I86_PTE_USER);
        pt_entry_del_attrib ( page, CUSTOM_PTE_AVAIL_1);
        pt_entry_del_attrib ( page, CUSTOM_PTE_AVAIL_2);
        //page++;
    }
}

inline void Switch_to_system_dir()
{
    asm volatile("mov %0, %%cr3":: "r"((uint32_t)system_dir):"memory");
    _prev_directory=_cur_directory;
    _cur_directory=system_dir;
}

inline void Switch_back_from_System()
{
    asm volatile("mov %0, %%cr3":: "r"((uint32_t)_prev_directory):"memory");
    _cur_directory=_prev_directory;
}

inline void Map_non_identity(uint32_t phys, uint32_t virt, uint32_t size, pdirectory* dir)
{
    if(pag)
    {
      Switch_to_system_dir();
    }
    for(uint32_t i=phys,j=virt;i<phys+size;i+=4096,j+=4096) //Make the pages and page tables for the whole kernel memory (higher Half)
    {
        //printf(" %x",i);
        page_t* page;
        page=get_page(j,1,dir); //kernel Pages;
        pt_entry_set_frame ( page, i);
        pt_entry_add_attrib ( page, I86_PTE_PRESENT);
        pt_entry_add_attrib ( page, I86_PTE_WRITABLE);
        pt_entry_add_attrib ( page, I86_PTE_USER);
        pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_1);
        pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_2);
        //page++;
    }
    if(pag)
      Switch_back_from_System();
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
		free(p);

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

int switch_pdirectory (pdirectory* dir)
{
	if (!dir)
		return 0;
	curr_pgdir.pgdir = dir;
  _cur_directory = dir;
  switch_directory(dir);
	return 1;
}

int Switch_Page_Directory(pdirectory* dir) ///to be used once our paging has been enabled.
{
	if (!dir)
		return 0;
  Switch_to_system_dir();
  switch_directory(dir);
  curr_pgdir.pgdir = dir;
  _cur_directory = dir;
	return 1;
}

pdirectory* get_directory ()
{
	return curr_pgdir.pgdir;
}

void flush_tlb_entry (uint32_t addr)
{
  asm volatile("cli");
  asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
  asm volatile("sti");
}

inline page_t* MapPage (void* phys, void* virt, pdirectory* dir)
{
   //! get page directory
   pdirectory* pageDirectory = dir;

   //! get page table
   table_t* e = &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ((uint32_t) virt) ];
   if (!*e)
   {
      //! page table not present, allocate it
      ptable* table = (ptable*) pmalloc(4096);
      if (!table)
         return 0;

      //! clear page table
      //memset (table, 0, sizeof(ptable));

      //! create a new entry
      table_t* entry = &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ( (uint32_t) virt) ];

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
   pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_1);
   pt_entry_add_attrib ( page, CUSTOM_PTE_AVAIL_2);
   pt_entry_add_attrib ( page, I86_PTE_PRESENT);
   return page;
}

void map(uint32_t phy,size_t size, pdirectory* dir)
{
    if(pag)
    {
      Switch_to_system_dir();
    }
    uint32_t j=phy;
    for (; j < phy+size;j+=0x1000)
    {
      MapPage((void*)j,(void*)j,dir);
    }
    if(pag)
    {
      Switch_back_from_System();
    }
}

inline page_t* Get_Page(uint32_t addr, int make, pdirectory* dir)
{
    Switch_to_system_dir();
    page_t* page=get_page(addr,make,dir);
    Switch_back_from_System();
    return page;
}

inline page_t* get_page(uint32_t addr,int make, pdirectory* dir)
{

      // Turn the address into an index.
      addr /= 0x1000;
      // Find the page table containing this address.
      uint32_t table_idx = addr / 1024;

      if (dir->m_entries[table_idx]) // If this table is already assigned
      {
          //printf("1 ");
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
          //pd_entry_set_frame (entry, (uint32_t)dir->m_entries[table_idx]);
        //  dir->tables[table_idx] = tmp | 0x7; // PRESENT, RW, US.
          return &table->m_entries[addr%1024];
      }
      else
      {
          printf(" Cant get the Page! ");
          return 0;
      }
}

inline table_t* get_table(uint32_t index,int make, pdirectory* dir)
{
      if (dir->m_entries[index]) // If this table is already assigned
      {
          //printf("1 ");
          table_t* entry = &dir->m_entries [index];
          return entry;
      }
      else if(make)
      {
          printf(" g%x",index);
          dir->m_entries[index] = (table_t)pmalloc(sizeof(ptable));
          memset((void*)dir->m_entries[index], 0, 0x1000);
          table_t* entry = &dir->m_entries [index];
          pd_entry_add_attrib (entry, I86_PDE_PRESENT);
          pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
          //pd_entry_set_frame (entry, (uint32_t)dir->m_entries[table_idx]);
        //  dir->tables[table_idx] = tmp | 0x7; // PRESENT, RW, US.
          return entry;
      }
      else
      {
          printf(" Cant get the table! ");
          return 0;
      }
}

void initialise_paging()
{
  printf("Making Default Page directories, This may take a while\n");
  system_dir = system_dir_maker();
  System_dir_setup();
  switch_pdirectory(system_dir);
  uint32_t cr0=0;
  asm volatile("mov %%cr0, %0": "=r"(cr0)::"memory");
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0):"memory");
  main_dir = pgdir_maker();
  user_dir = pgdir_maker();
  curr_pgdir = PgDirs[0];
  _cur_directory = main_dir;
}

void enable_paging()
{
   //! switch to our main page directory
   Kernel_Mapper(main_dir);
   Kernel_Mapper(user_dir);
   printf("\nSwitching Directory");
   switch_pdirectory (main_dir);
   //while(1);
   //BITMAP_LOCATION=3072*1024*1024;
   printf("\nDirectory Switched");
   pag=1;
   max_mem=0xFFFFFFFF;
   mb_temp=3062;
   printf("\nEnabled paging\n");
   //pmmngr_paging_enable (true);
}
