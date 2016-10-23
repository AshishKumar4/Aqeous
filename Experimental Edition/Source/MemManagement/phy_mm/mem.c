#include "phy_mm\mem.h"
#include "virt_mm\vmem.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
//#include "streams.h"
#include "CustomCSRB\csrb_custom.h"

#include "virt_mm\paging.h"

/*
	[0-4MB] = KERNEL 
	[4-8MB] = KERNEL MAIN PAGE DIRECTORY (sys_dir)
	[8-12MB] = FRAME STACK FOR PHYSICAL FRAME ALLOCATION
	[12-14MB] = FREE
	[14-16MB] = RESERVED 
	[>3GB] = RESERVED
*/

void memmap_generator()
{
	MemRegion_t* OS_MMap = phy_alloc4K();
	Fmemmap = OS_MMap;
	OS_MMap->startHi = 0;
	OS_MMap->sizeHi = 4*1024*1024;
	OS_MMap->type = 5;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 4*1024*1024;
	OS_MMap->sizeHi = 4*1024*1024;
	OS_MMap->type = 6;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 8*1024*1024;
	OS_MMap->sizeHi = 6*1024*1024;
	OS_MMap->type = 5;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 14*1024*1024;
	OS_MMap->sizeHi = 2*1024*1024;
	OS_MMap->type = 2;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 16*1024*1024;
	OS_MMap->sizeHi = 8*1024*1024;
	OS_MMap->type = 5;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 24*1024*1024;
	OS_MMap->sizeHi = 166*1024*1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 190*1024*1024;
	OS_MMap->sizeHi = 410*1024*1024;
	OS_MMap->type = 5;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 600*1024*1024;
	OS_MMap->sizeHi = 2472*1024*1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	printf("\nOS Specific Memory Regions Preallocated!");
}

void setup_frameStack()
{
	printf("\nSetting up Frame Stack!");

	//Setup the Stack frame at 10th MB!
	uint32_t* frame_stack_ptr = 10240*1024;
	uint32_t i = 786432; //3rd GB
	for(; i > 4096; --i) //more then 50th MB
	{
		*frame_stack_ptr++ = i;
	}
	frame_stack_end = frame_stack_ptr;
	--frame_stack_end;
}

inline uint32_t pop_frameStack()
{
	uint32_t fr = *frame_stack_end;
	--frame_stack_end;
	if(!fr) printf("\nNo memory left! %x",fr);
	return fr;
}

inline void push_frameStack(uint32_t fr)
{
	++frame_stack_end;
	*frame_stack_end = fr;
}

uint32_t phy_alloc4K()
{
	uint32_t addr;
	back:
	addr = pop_frameStack()*4096;
	if(*(uint32_t*)(get_pageEntry(addr)) & (uint32_t)CUSTOM_PTE_AVAIL_1)
	goto back;
	return addr;
}

void Setup_PhyMEM()     //Sets up the allocation buffers for kernel memory address space (System Directory)
{
	uint32_t new_buff = system_pdirCap->csrb_f; //Allocate 4kb space.    Free blocks
	CustomCSRB_M_header_t* nb_f = (CustomCSRB_M_header_t*)new_buff;
	nb_f->head = (CustomCSRB_M_header_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	new_buff = system_pdirCap->csrb_u; //Allocate 4kb space.     Used blocks
	CustomCSRB_M_header_t* nb_u = (CustomCSRB_M_header_t*)new_buff;     
	nb_u->head = (CustomCSRB_M_header_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	nb_u->other = nb_f;
	nb_f->other = nb_u;

	nb_f->entries = 0;
	nb_u->entries = 0;

	MemRegion_t* mm = Fmemmap;
	CustomCSRB_M_t* tmp_f = (CustomCSRB_M_t*)nb_f->head;
	CustomCSRB_M_t* tmp_u = (CustomCSRB_M_t*)nb_u->head;
	++mm;
	while(1)
	{
		if(!(mm->type-1))
		{
			tmp_f->addr = (uint32_t*)tmp_f;
			tmp_f->size = mm->sizeHi;
			tmp_f->begin = mm->startHi;
			//printf("\n%x %x %x",tmp_f->addr,tmp_f->size,tmp_f->begin);
			++tmp_f;
			++nb_f->entries;
		}
		else
		{
			tmp_u->addr = (uint32_t*)tmp_u;
			tmp_u->size = mm->sizeHi;
			tmp_u->begin = mm->startHi;
			//printf("\n%x %x %x %x",tmp_u->addr,tmp_u->size,tmp_u->begin, mmap_info->sizeHi);
			++tmp_u;
			++nb_u->entries;
		}
		++mm;
		if(mm->reservedt != 0xFFE42) break;
	}
	nb_f->tail = (uint32_t*)tmp_f; 
	tmp_f->addr = nb_f->head;   //Make last one to point to first one.
	printf("\nB%x", tmp_f->size);
	
	nb_u->tail = (uint32_t*)tmp_u; 
	tmp_u->addr = nb_u->head;   //Make last one to point to first one.
	//printf("\nA1");
	//while(1);
}

uint32_t pmem(uint32_t size)
{
	Pdir_Capsule_t* curr_cap = system_pdirCap;
	PageDirectory_t* dir = system_dir;
	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

	uint32_t* tail = csrb_f->tail;
	uint32_t* head = csrb_f->head;

	CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)curr_cap->csrb_u;

	CustomCSRB_M_t* tm = (CustomCSRB_M_t*)head;
	uint32_t baddr = 0;
	for(int i = 0; i<1024*1024; i++)
	{
		tm = (CustomCSRB_M_t*)tm->addr;
		//printf("A-%x ",tm->size);
		if(tm->size >= size)
		{
			tm->size -= size;
			if(!tm->size) 
			{
				tm->reserved = 1;
				--csrb_f->entries;
			}
			baddr = tm->begin;

			/****/  //TODO: Create an entry in used CSRB.

			CustomCSRB_M_t* tmp = (CustomCSRB_M_t*)csrb_u->tail;
			tmp->size = size;
			tmp->begin = baddr;
			tmp->addr = (uint32_t*)tmp;
			++tmp;
			++csrb_u->entries;
			if(!(((uint32_t)(tmp) + 16)%4096))
			{
				tmp->addr = phy_alloc4K();
				tmp = tmp->addr;
				++csrb_u->entries;
			}
			tmp->addr = csrb_u->head;
			csrb_u->tail = tmp;
			/****/

			tm->begin += size;
			break;
		}
		tm++;
	}
	//printf("\n\n");
	//Create necessary page tables and pages for the allocated memory.
	uint32_t pg_frame = baddr/4096;
	uint32_t pd_off = pg_frame/1024;
	uint32_t pt_off = pg_frame%1024;
	table_t* tentry = &dir->table_entry[pd_off];
	PageTable_t* pt = PAGE_GET_PHYSICAL_ADDRESS(tentry);
	//Create the page. 
	page_t* pg = &pt->page_entry[pt_off];
	for(int i =  pg_frame; i<(size/4096) + pg_frame; i++)
	{
		if(!(i%1024))   //Whenever crossing page table boundries, just switch to next page tables.
		{
			tentry = &dir->table_entry[i/1024];
			pt = PAGE_GET_PHYSICAL_ADDRESS(tentry);
			pg = &pt->page_entry[0];
		}
		*pg |= 1027 | CUSTOM_PTE_AVAIL_2;
		++pg;
	}
	//SwitchFrom_SysDir();
	return baddr;
	
}

void pfree(uint32_t address)
{
	Pdir_Capsule_t* curr_cap = system_pdirCap;
	PageDirectory_t* dir = system_dir;
	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

	uint32_t* tail_f = csrb_f->tail;
	uint32_t* head_f = csrb_f->head;

	CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)curr_cap->csrb_u;
	
	uint32_t* tail_u = csrb_u->tail;
	uint32_t* head_u = csrb_u->head;

	CustomCSRB_M_t* tmf = (CustomCSRB_M_t*)head_f;
	CustomCSRB_M_t* tmu = (CustomCSRB_M_t*)head_u;
	//printf("\n%x %x",csrb_u->entries,csrb_f->entries);
//	return;
	for(int i = 0; i < csrb_u->entries; i++)
	{
		tmu = tmu->addr;
		//printf("\tA%x Ax%x Sx%x ",address, tmu->begin, tmu->size);
		if(tmu->begin == address)
		{
			tmf = csrb_f->tail;
			tmf->addr = tmf;
			tmf->size = tmu->size;
			tmf->begin = tmu->begin;
			tmf->reserved = 0;
			++csrb_f->entries;
			++tmf;
			if(!(((uint32_t)(tmf) + 16)%4096))
			{
				tmf->addr = phy_alloc4K();
				tmf = tmf->addr;
				++csrb_f->entries;
			}
			tmf->addr = csrb_f->head;
			csrb_f->tail = tmf;
			memset(tmu->begin, 0, tmu->size);
			tmu->begin = 0;
			tmu->size = 0;
			tmu->reserved = 1;
			--csrb_u->entries;
			return;
		}
		++tmu;
	}
	return;
}


inline uint32_t pmalloc(uint32_t id)
{
    //return kmalloc_int(sz, 1, 0,2,1,4);
    uint32_t mem=phy_alloc4K();
  //  if(pag)
  //    MapPage((void*)mem,(void*)mem);
   return mem;
}

const uint32_t mtalc_start = 220*1024*1024;
const uint32_t mtalc_end = 500*1024*1024;

uint32_t mtalloc(uint32_t pages)
{
   uint32_t tmp = 0;
   PageTable_t* table;
   page_t* page;
   table_t* table_entry = &system_dir->table_entry[mtalc_start/(1024*4096)];
   for(int i=0; i<71 ; i++)
   {
      table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(table_entry);
      page=&table->page_entry[0];
      for(int j=0; j<1024 ; j++)
      {
         if(!(*page & CUSTOM_PTE_AVAIL_1))
         {
            ++tmp;
            if(pages == tmp)
            {
               pt_entry_add_attrib( page, CUSTOM_PTE_AVAIL_1);
               return (PAGE_GET_PHYSICAL_ADDRESS(page));
               //TODO: Return the address.
            }
         }
         else tmp = 0;
         ++page;
      }
      ++table_entry;
   }
   _printf("\nCould not find memory, sorry. Try kmalloc()");
   return kmalloc(pages*4096);
   return 0;
   //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
}

void mtfree(uint32_t addr, uint32_t size)
{
   PageTable_t* table_sys;
   page_t* page_sys;
   PageTable_t* table;
   page_t* page;
   addr /= 4096;
   table_sys=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->table_entry[mtalc_start/(1024)]);
   page_sys=&table_sys->page_entry[addr%1024];
   table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(&_cur_dir->table_entry[mtalc_start/(1024)]);
   page=&table->page_entry[addr%1024];
   for(;size;size--)
   {
      pt_entry_del_attrib( page_sys, CUSTOM_PTE_AVAIL_1);
      *page = 0;
      ++page_sys;
      ++page;
   }
}

uint32_t fsalloc(uint32_t sz)
{
  uint32_t addr=pmem(sz);
  //printf(" Ab%x",addr);
  return addr;
}
