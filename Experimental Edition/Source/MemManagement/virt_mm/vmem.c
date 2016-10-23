#include "phy_mm\mem.h"
#include "virt_mm\vmem.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "CustomCSRB\csrb_custom.h"

#include "virt_mm\paging.h"

void Setup_VMEM(Pdir_Capsule_t* dcap)     //Sets up the allocation buffers for kernel memory address space (System Directory)
{
	uint32_t new_buff = dcap->csrb_f; //Allocate 4kb space.    Free blocks
	CustomCSRB_M_header_t* nb_f = (CustomCSRB_M_header_t*)new_buff;
	nb_f->head = (CustomCSRB_M_header_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	new_buff = dcap->csrb_u; //Allocate 4kb space.     Used blocks
	CustomCSRB_M_header_t* nb_u = (CustomCSRB_M_header_t*)new_buff;     
	nb_u->head = (CustomCSRB_M_header_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	nb_u->other = nb_f;
	nb_f->other = nb_u;

	MemRegion_t* mm = mmap_info;
	CustomCSRB_M_t* tmp_f = (CustomCSRB_M_t*)nb_f->head;
	CustomCSRB_M_t* tmp_u = (CustomCSRB_M_t*)nb_u->head;
	for(int i = 0; mmap_info; i++)
	{
		if(!(mmap_info->type-1))
		{
			tmp_f->addr = (uint32_t*)tmp_f;
			tmp_f->size = mmap_info->sizeHi;
			tmp_f->begin = mmap_info->startHi;
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
	}
	nb_f->tail = (uint32_t*)tmp_f; 
	tmp_f->addr = nb_f->head;   //Make last one to point to first one.
	
	nb_u->tail = (uint32_t*)tmp_u; 
	tmp_u->addr = nb_u->head;   //Make last one to point to first one.
}

uint32_t vmem(uint32_t size)
{
	Pdir_Capsule_t* curr_cap = _cur_pdirCap;
	PageDirectory_t* dir = &curr_cap->pdir;
	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

	uint32_t* tail = csrb_f->tail;
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
		PageTable_t* pt = PAGE_GET_PHYSICAL_ADDRESS(&dir->table_entry[pd_off]);
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
					*entry = phy_alloc4K();
					pd_entry_add_attrib (entry, I86_PDE_PRESENT);
					pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
					pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_1);
					pd_entry_del_attrib (entry, CUSTOM_PDE_AVAIL_2);
				}
				pt = PAGE_GET_PHYSICAL_ADDRESS(entry);
				pg = &pt->page_entry[0];
			}
			*pg = 1027 | CUSTOM_PTE_AVAIL_2 | phy_alloc4K();
			++pg;
		}
	}
	if(size%4096)
	{
		PageTable_t* pt = PAGE_GET_PHYSICAL_ADDRESS(&dir->table_entry[pd_off]);
		//Create the page. 
		page_t* pg = &pt->page_entry[pt_off];
		if(!pg)
		{
			uint32_t phy_mem = phy_alloc4K();
			*pg = 1027 | CUSTOM_PTE_AVAIL_1 | phy_mem;
		}
		
		//Make appropriate memory strips.
	}
	//SwitchFrom_SysDir();
	return baddr;
}

int vfree(uint32_t address)
{

}