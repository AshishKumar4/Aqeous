#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "CustomCSRB/csrb_custom.h"

#include "virt_mm/paging.h"

void Setup_VMEM(Pdir_Capsule_t* dcap)     //Sets up the allocation buffers for kernel memory address space (System Directory)
{
	uint32_t new_buff = (uint32_t)dcap->csrb_f; //Allocate 4kb space.    Free blocks
	CustomCSRB_M_header_t* nb_f = (CustomCSRB_M_header_t*)new_buff;
	nb_f->head = (uint32_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	new_buff = (uint32_t)dcap->csrb_u; //Allocate 4kb space.     Used blocks
	CustomCSRB_M_header_t* nb_u = (CustomCSRB_M_header_t*)new_buff;
	nb_u->head = (uint32_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	nb_u->changed = 0;
	nb_f->changed = 0;

	//MemRegion_t* mm = mmap_info;
	CustomCSRB_M_t* tmp_f = (CustomCSRB_M_t*)nb_f->head;
	CustomCSRB_M_t* tmp_u = (CustomCSRB_M_t*)nb_u->head;
	for(int i = 0; mmap_info; i++)
	{
		if(mmap_info->type < 7)
		{
			tmp_f->addr = (uint32_t*)tmp_f;
			tmp_f->size = mmap_info->sizeHi;
			tmp_f->begin = mmap_info->startHi;
			printf("\nAddr: %x Size: %x", tmp_f->begin, tmp_f->size);
			++tmp_f;
			++nb_f->entries;
		}
		else
		{
			tmp_u->addr = (uint32_t*)tmp_u;
			tmp_u->size = mmap_info->sizeHi;
			tmp_u->begin = mmap_info->startHi;
			++tmp_u;
			++nb_u->entries;
		}
		++mmap_info;
		if(mmap_info->reservedt != 0xFFE42) break;
	}
	nb_f->tail = (uint32_t*)tmp_f;
	tmp_f->addr = nb_f->head;   //Make last one to point to first one.

	nb_u->tail = (uint32_t*)tmp_u;
	tmp_u->addr = nb_u->head;   //Make last one to point to first one.*/
}

uint32_t vmem(uint32_t size)
{
	Pdir_Capsule_t* curr_cap = Get_Scheduler()->curr_dir;
	PageDirectory_t* dir = &curr_cap->pdir;
	//SwitchTo_SysDir();
	switch_directory(system_dir);

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

	//uint32_t* tail = csrb_f->tail;
	uint32_t* head = csrb_f->head;

	CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)curr_cap->csrb_u;

	CustomCSRB_M_t* tm = (CustomCSRB_M_t*)head;
	uint32_t baddr = 0;
	while(1)
	{
		tm = (CustomCSRB_M_t*)tm->addr;
		if(tm->size >= size)
		{
			tm->size -= size;
			baddr = (uint32_t)tm->begin;

			/****/  //TODO: Create an entry in used CSRB.
			CustomCSRB_M_t* tmp = (CustomCSRB_M_t*)csrb_u->tail;
			tmp->size = size;
			tmp->begin = baddr;
			tmp->addr = (uint32_t*)tmp;
			++tmp;
			++csrb_u->entries;
			if(!(((uint32_t)(tmp) + 16)%4096))
			{
				tmp->addr = (uint32_t*)phy_alloc4K();
				tmp = (CustomCSRB_M_t*)tmp->addr;
				++csrb_u->entries;
			}
			tmp->addr = csrb_u->head;
			/****/

			tm->begin += size;
			break;
		}
		tm++;
	}

	//Create necessary page tables and pages for the allocated memory.
	uint32_t pg_frame = baddr/4096;
	uint32_t pd_off = pg_frame/1024;
	uint32_t pt_off = pg_frame%1024;
	if(!dir->table_entry[pd_off])
	{
		//printf("A");
		//Create the table, and then the page.
		table_t* entry = &dir->table_entry[pd_off];
		*entry = phy_alloc4K();
		pd_entry_add_attrib (entry, I86_PDE_PRESENT);
		pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
		pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_1);
		pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_2);
	}
	else
	{
		//printf("B");
		PageTable_t* pt = (PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(&dir->table_entry[pd_off]);
		//Create the page.
		page_t* pg = &pt->page_entry[pt_off];
		uint32_t i;
		for(i =  pg_frame; i<(size/4096) + pg_frame; i++)
		{
			if(!(i%1024))   //Whenever crossing page table boundries, just switch to next page tables.
			{
				table_t* entry = &dir->table_entry[i/1024];
				if(!entry)
				{
					//printf(" b2");
					*entry = phy_alloc4K();
					pd_entry_add_attrib (entry, I86_PDE_PRESENT);
					pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
					pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_1);
					pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_2);
				}
				pt = (PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(entry);
				pg = &pt->page_entry[0];
			}
			//printf(" b1");
			*pg = 1027 | CUSTOM_PTE_AVAIL_2 | phy_alloc4K();
			++pg;
		}
	}
	if(size%4096)
	{
		//printf("C");
		PageTable_t* pt = (PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(&dir->table_entry[pd_off]);
		//Create the page.
		page_t* pg = &pt->page_entry[pt_off];
		if(!*pg)
		{
			//printf(" c1");
			uint32_t phy_mem = phy_alloc4K();
			*pg = 1027 | CUSTOM_PTE_AVAIL_1 | phy_mem;
		}
	//	printf(" pp%d %d", pg, *pg);

		//Make appropriate memory strips.
	}
/*
	if(!(baddr%4096))
	{
		uint32_t* pgs =
		for(int i = 0; i < )
	}
*/
	switch_directory(dir);
	//SwitchFrom_SysDir();
	return baddr;
}

void vfree(void* addr)
{
	uint32_t address = (uint32_t)addr;
	Pdir_Capsule_t* curr_cap = system_pdirCap;
	//PageDirectory_t* dir = system_dir;
	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

	//uint32_t* tail_f = csrb_f->tail;
	uint32_t* head_f = csrb_f->head;

	CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)curr_cap->csrb_u;

	//uint32_t* tail_u = csrb_u->tail;
	uint32_t* head_u = csrb_u->head;

	CustomCSRB_M_t* tmf = (CustomCSRB_M_t*)head_f;
	CustomCSRB_M_t* tmu = (CustomCSRB_M_t*)head_u;
	//printf("\n%x %x",csrb_u->entries,csrb_f->entries);
//	return;
	for(uint32_t i = 0; i < csrb_u->entries; i++)
	{
		tmu = (CustomCSRB_M_t*)tmu->addr;
		//printf("\tA%x Ax%x Sx%x ",address, tmu->begin, tmu->size);
		if(tmu->begin == address)
		{
			tmf = (CustomCSRB_M_t*)csrb_f->tail;
			tmf->addr = (uint32_t*)tmf;
			tmf->size = tmu->size;
			tmf->begin = tmu->begin;
			tmf->reserved = 0;
			++csrb_f->entries;
			++tmf;
			if(!(((uint32_t)(tmf) + 16)%4096))
			{
				tmf->addr = (uint32_t*)phy_alloc4K();
				CustomCSRB_M_t* tmp2 =tmf;
				tmf = (CustomCSRB_M_t*)tmf->addr;
				tmf->addr = (uint32_t*)tmp2;
				++tmf;
				++csrb_u->entries;
			}
			tmf->addr = csrb_f->head;
			csrb_f->tail = (uint32_t*)tmf;
			memset((void*)tmu->begin, 0, tmu->size);
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
