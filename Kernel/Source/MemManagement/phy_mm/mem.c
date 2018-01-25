#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
//#include "streams.h"
#include "CustomCSRB/csrb_custom.h"
#include "MManager/mmanagerSys.h"

#include "virt_mm/paging.h"
#include "Scheduler/Scheduler.h"
#include "tasking.h"

/*
	[0-4MB] = KERNEL
	[4-8MB] = KERNEL MAIN PAGE DIRECTORY (sys_dir)
	[8-12MB] = FRAME STACK FOR PHYSICAL FRAME ALLOCATION
	[12-14MB] = MMAD Structures
	[14-16MB] = RESERVED
	[16-24MB] =
	[24-600MB] = KERNEL Resources
	[700MB - 3GB] = USER Free Space
	[>3GB] = NOT AVAILABLE
	REST FREE
*/

void memmap_generator()
{
	MemRegion_t* OS_MMap = (MemRegion_t*)(16*1024*1024);
	Fmemmap = OS_MMap;
	OS_MMap->startHi = 0x0;
	OS_MMap->sizeHi = 0x400000;//4*1024*1024;
	OS_MMap->type = 7;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0x400000;// 4*1024*1024;
	OS_MMap->sizeHi = 0x400000;//4*1024*1024;
	OS_MMap->type = 8;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0x800000;//8*1024*1024;
	OS_MMap->sizeHi = 0x600000;//6*1024*1024;
	OS_MMap->type = 9;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0xE00000;//14*1024*1024;
	OS_MMap->sizeHi = 0x200000;//2*1024*1024;
	OS_MMap->type = 10;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0x1000000;//16*1024*1024;
	OS_MMap->sizeHi = 0x800000;//8*1024*1024;
	OS_MMap->type = 9;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0x1800000;//24*1024*1024;
	OS_MMap->sizeHi = 0x600000;//6*1024*1024;
	OS_MMap->type = 11;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0x1E00000;//30*1024*1024;
	OS_MMap->sizeHi = 0x23A00000;//570*1024*1024;
	OS_MMap->type = 11;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;/*
	OS_MMap->startHi = 190*1024*1024;
	OS_MMap->sizeHi = 410*1024*1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;*/
	OS_MMap->startHi = 0x25800000;//600*1024*1024
	OS_MMap->sizeHi = 0x6300000;//(100*1024*1024);//(2*1024*1024*1024) - (600*1024*1024);
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;

	++OS_MMap;
	OS_MMap->startHi = 0x32000000;//(800*1024*1024);
	OS_MMap->sizeHi = 0x8E000000;//(2272*1024);//(3*1024*1024*1024) - (800*1024*1024);
	//OS_MMap->sizeHi *= 1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	/*
	++OS_MMap;
	OS_MMap->startHi = (3*1024*1024*1024) + (256*1024*1024);
	OS_MMap->sizeHi = 256*1024*1024;
	OS_MMap->type = 2;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = (3*1024*1024*1024) + (512*1024*1024);
	OS_MMap->sizeHi =(256*1024*1024);
	OS_MMap->type = 2;
	OS_MMap->reservedt = 0xFFE42;*/

	/*************

	OS_MMap->startHi = 0x1E00000;//30*1024*1024;
	OS_MMap->sizeHi = 0xAA00000;//170*1024*1025;			//0x23A00000;//570*1024*1024;
	OS_MMap->type = 11;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 190*1024*1024;
	OS_MMap->sizeHi = 410*1024*1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	++OS_MMap;
	OS_MMap->startHi = 0xC800000;//200*1024*1024			//0x2BC00000;//(700*1024*1024);
	OS_MMap->sizeHi = 0x25800000;//600*1024*1024			//0x6300000;//(100*1024*1024);//(2*1024*1024*1024) - (600*1024*1024);
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;

	++OS_MMap;
	OS_MMap->startHi = 0x32000000;//(800*1024*1024);
	OS_MMap->sizeHi = 0x8E000000;//(2272*1024);//(3*1024*1024*1024) - (800*1024*1024);
	//OS_MMap->sizeHi *= 1024;
	OS_MMap->type = 1;
	OS_MMap->reservedt = 0xFFE42;
	*************/

	printf("\nOS Specific Memory Regions Preallocated!");
}

void __attribute__((optimize("O2"))) setup_frameStack()
{
	printf("\nSetting up Frame Stack!");
	uint32_t* frame_stack_ptr = (uint32_t*)0xA00000, *frame_stack_start = (uint32_t*)0xA00000;
	MemRegion_t* mm = Fmemmap;
	++mm;
	while(1)
	{
		if(mm->type == 1 || mm->type == 12)
		{
			for(uint32_t i = mm->startHi; i < mm->startHi + mm->sizeHi; i+=4096)
			{
				*frame_stack_ptr = i/4096;
				++frame_stack_ptr;
			}
		}
		++mm;
		if(mm && mm->reservedt != 0xFFE42) break;
	}
	frame_stack_end = frame_stack_ptr;
	--frame_stack_end;
	uint32_t size = frame_stack_end - frame_stack_start;
	printf("\nSize of Frame Stack = %d", size);
	for(uint32_t i = 0; i < size/2; i++)
	{
		uint32_t c = frame_stack_start[i];
		frame_stack_start[i] = frame_stack_start[size-i-1];
		frame_stack_start[size-i-1] = c;
	}
	//for(int i = 0; i < 500; i++)
	//	printf("%x \t", phy_alloc4K());
}

uint32_t pop_frameStack()
{
	uint32_t fr;
	back:
	fr = *frame_stack_end;
	--frame_stack_end;
	//printf("--A--");
	if(!fr)
	{
		printf("\nNo memory left! %x",fr);
		//while(1);
		asm volatile("hlt");
		goto back;
	}
	return fr;
}

void push_frameStack(uint32_t fr)
{
	++frame_stack_end;
	*frame_stack_end = fr;
}

uint32_t phy_alloc4K()
{
	uint32_t addr;
	addr = pop_frameStack()*4096;
	/*
	if((*(uint32_t*)(get_pageEntry(addr))) & (uint32_t)CUSTOM_PTE_AVAIL_1)	goto back;
	*((uint32_t*)(get_pageEntry(addr))) |= CUSTOM_PTE_AVAIL_1 | CUSTOM_PTE_AVAIL_2;*/
	return addr;
}

void __attribute__((optimize("O2"))) Setup_PhyMEM()     //Sets up the allocation buffers for kernel memory address space (System Directory)
{
	uint32_t new_buff = (uint32_t)system_pdirCap->csrb_f; //Allocate 4kb space.    Free blocks
	CustomCSRB_M_header_t* nb_f = (CustomCSRB_M_header_t*)new_buff;
	nb_f->head = (uint32_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	new_buff = (uint32_t)system_pdirCap->csrb_u; //Allocate 4kb space.     Used blocks
	CustomCSRB_M_header_t* nb_u = (CustomCSRB_M_header_t*)new_buff;
	nb_u->head = (uint32_t*)(new_buff + sizeof(CustomCSRB_M_header_t));

	nb_u->changed = 0;
	nb_f->changed = 0;

	nb_f->entries = 0;
	nb_u->entries = 0;

	MemRegion_t* mm = Fmemmap;
	//mmap_info = mm;
	CustomCSRB_M_t* tmp_f = (CustomCSRB_M_t*)nb_f->head;
	CustomCSRB_M_t* tmp_u = (CustomCSRB_M_t*)nb_u->head;
	++mm;

	while(1)
	{
		if(mm->type == 11)
		{
			tmp_f->addr = (uint32_t*)tmp_f;
			tmp_f->size = mm->sizeHi;
			tmp_f->begin = mm->startHi;
			tmp_f->reserved = 0;
		//	printf("\n%x %x %x",tmp_f->addr,tmp_f->size,tmp_f->begin);
			++tmp_f;
			++nb_f->entries;
		}
		else
		{
			tmp_u->addr = (uint32_t*)tmp_u;
			tmp_u->size = mm->sizeHi;
			tmp_u->begin = mm->startHi;
			tmp_f->reserved = 0;
			//printf("\n%x %x %x %x",tmp_u->addr,tmp_u->size,tmp_u->begin, mmap_info->sizeHi);
			++tmp_u;
			++nb_u->entries;
		}
		++mm;
		if(mm->reservedt != 0xFFE42) break;
	}

	nb_f->tail = (uint32_t*)tmp_f;
	tmp_f->addr = nb_f->head;   //Make last one to point to first one.
	//printf("\nB%x", tmp_f->size);

	nb_u->tail = (uint32_t*)tmp_u;
	tmp_u->addr = nb_u->head;   //Make last one to point to first one.
	//printf("\nA1");
	//while(1);
}

extern uint32_t test_flag_mm;

void* __attribute__((optimize("O2"))) pmem(uint32_t size)
{
	LOCK(MEM_LOCK);

	Pdir_Capsule_t* curr_cap = system_pdirCap;
	//PageDirectory_t* dir = system_dir;

	size = ROUNDUP(size,4);
	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

//	uint32_t* tail = csrb_f->tail;
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
			if((((uint32_t)(tmp) + 16)%4096) <= 8)
			{
	//			printf("\n\n\n\t\t%gNEW CSRB MADE%g",3,9);
				tmp->addr = (uint32_t*)phy_alloc4K();
				CustomCSRB_M_t* tmp2 = tmp;
				tmp = (CustomCSRB_M_t*)tmp->addr;
				tmp->addr = (uint32_t*)tmp2;
				++tmp;
				++csrb_u->entries;
			}
			tmp->addr = csrb_u->head;
			csrb_u->tail = (uint32_t*)tmp;
			/****/

			tm->begin += size;
			break;
		}
		tm++;
	}
	if(multitasking_ON)
	{
		SchedulerKits_t* kit = Get_Scheduler();
		mmads_stack_end = ((task_t*)(kit->current_task))->process;
		++mmads_stack_end;
		++mmads_stack_size;
	}
	UNLOCK(MEM_LOCK);
	//SwitchFrom_SysDir();
	//printf("=>{%x}<=", baddr);
	return (void*)baddr;

}

void* __attribute__((optimize("O2"))) pmem_4k(uint32_t pages)
{	
	//return mtalloc(pages);
	LOCK(MEM_LOCK);
	Pdir_Capsule_t* curr_cap = system_pdirCap;
	//PageDirectory_t* dir = system_dir;

	uint32_t size = pages*4096;

	//SwitchTo_SysDir();

	CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)curr_cap->csrb_f;   // csrb FREE structure is stored in the next page after the page directory.

//	uint32_t* tail = csrb_f->tail;
	uint32_t* head = csrb_f->head;

	CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)curr_cap->csrb_u;

	CustomCSRB_M_t* tm = (CustomCSRB_M_t*)head;
	uint32_t baddr = 0;

	for(int i = 0; i < 1024*1024; i++)
	{
		tm = (CustomCSRB_M_t*)tm->addr;	// Traverse CSRB

		baddr = ROUNDUP(tm->begin, 4096);

		if(baddr + size < tm->begin + tm->size + 4096)	// If the allocation can happen in the block
		{
			//uint32_t tts = tm->size;
			uint32_t tba = tm->begin;
		//	printf("\n=>tts %d, tba %d", tts, tba);
			tm->begin = baddr + size;
			tm->size -= tm->begin - tba;

			/* Put the preceeding broken block ahead in the end of free csrb */
			if(baddr != tba)
			{
				CustomCSRB_M_t* tmp = (CustomCSRB_M_t*)csrb_f->tail;
				tmp->size = baddr - tba;
				tmp->begin = tba;
				tmp->addr = (uint32_t*)tmp;
				++tmp;
				++csrb_f->entries;

				if((((uint32_t)(tmp) + 16)%4096) <= 8)
				{
					//printf("\n\n\n\t\t%gNEW CSRB MADE%g",3,9);
					tmp->addr = (uint32_t*)phy_alloc4K();
					CustomCSRB_M_t* tmp2 = tmp;
					tmp = (CustomCSRB_M_t*)tmp->addr;
					tmp->addr = (uint32_t*)tmp2;
					++tmp;
					++csrb_f->entries;
				}
				tmp->addr = csrb_f->head;
				csrb_f->tail = (uint32_t*)tmp;
				//tm->size = baddr - tm->begin;
			}
			/* Put the preceeding broken block ahead in the end of free csrb */

			CustomCSRB_M_t* tmp = (CustomCSRB_M_t*)csrb_u->tail;
			tmp->size = size;
			tmp->begin = baddr;
			tmp->addr = (uint32_t*)tmp;
			++tmp;
			++csrb_u->entries;
			if((((uint32_t)(tmp) + 16)%4096) <= 8)
			{
	//			printf("\n\n\n\t\t%gNEW CSRB MADE%g",3,9);
				tmp->addr = (uint32_t*)phy_alloc4K();
				CustomCSRB_M_t* tmp2 = tmp;
				tmp = (CustomCSRB_M_t*)tmp->addr;
				tmp->addr = (uint32_t*)tmp2;
				++tmp;
				++csrb_u->entries;
			}
			tmp->addr = csrb_u->head;
			csrb_u->tail = (uint32_t*)tmp;

			break;
		}
		tm++;
	}
	//printf("\n>-%x-<", baddr);
	//SwitchFrom_SysDir();
	if(multitasking_ON)
	{
		SchedulerKits_t* kit = Get_Scheduler();
		mmads_stack_end = ((task_t*)(kit->current_task))->process;
		++mmads_stack_end;
		++mmads_stack_size;
	}
	UNLOCK(MEM_LOCK);
	return (void*)baddr;
}

void* __attribute__((optimize("O2"))) pmem_realloc(void* ptr, uint32_t sz)
{
	void* new_ptr = NULL;
	new_ptr = pmem(sz);
	uint32_t tsz = sz;
	LOCK(MEM_LOCK);
	uint32_t address = (uint32_t)ptr;
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
	//printf("\n<%d>", sz);
	for(uint32_t i = 0; i < csrb_u->entries*2; i++)
	{
		tmu = (CustomCSRB_M_t*)tmu->addr;
		//printf("%gAx%g",10, 0);
	//	printf("%gA%xBx%x %g ",10, i, csrb_u->entries,0);
		if(tmu->begin <= address && tmu->size + tmu->begin > address)
		{
			memcpy((void*)new_ptr, (void*)tmu->begin, MIN(tmu->size, tsz));
			//printf("\n {%d, %d}", tmu->size, tsz);

			tmf = (CustomCSRB_M_t*)csrb_f->tail;
			tmf->addr = (uint32_t*)tmf;
			tmf->size = tmu->size;
			tmf->begin = tmu->begin;
			tmf->reserved = 0;
			++csrb_f->entries;
			++tmf;
			if((((uint32_t)(tmf) + 16)%4096) <= 8)
			{
		//		printf("\n\n\n\t\t%gNEW CSRB SWITCHED%g",3,0);
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
	//		printf("\n\n\n\t\t%gAx tmu->begin: %x tmu->size: %x%g",10,tmu->begin, tmu->size, 0);
			tmu->begin = 0;
			tmu->size = 0;
			tmu->reserved = 1;
			--csrb_u->entries;
			UNLOCK(MEM_LOCK);
			return (void*)new_ptr;
		}
		++tmu;
	}

	UNLOCK(MEM_LOCK);
	new_ptr = pmem(sz);
	UNLOCK(MEM_LOCK);
	return (void*)new_ptr;
}

void __attribute__((optimize("O2"))) pfree(void* addr)
{
	return;
	LOCK(MEM_LOCK);
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
	for(uint32_t i = 0; i < csrb_u->entries*2; i++)
	{
		tmu = (CustomCSRB_M_t*)tmu->addr;
		//printf("%gAx%g",10, 0);
	//	printf("%gA%xBx%x %g ",10, i, csrb_u->entries,0);
		if(tmu->begin <= address && tmu->size + tmu->begin > address)
		{
			tmf = (CustomCSRB_M_t*)csrb_f->tail;
			tmf->addr = (uint32_t*)tmf;
			tmf->size = tmu->size;
			tmf->begin = tmu->begin;
			tmf->reserved = 0;
			++csrb_f->entries;
			++tmf;
			if((((uint32_t)(tmf) + 16)%4096) <= 8)
			{
		//		printf("\n\n\n\t\t%gNEW CSRB SWITCHED%g",3,0);
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
	//		printf("\n\n\n\t\t%gAx tmu->begin: %x tmu->size: %x%g",10,tmu->begin, tmu->size, 0);
			tmu->begin = 0;
			tmu->size = 0;
			tmu->reserved = 1;
			--csrb_u->entries;
			UNLOCK(MEM_LOCK);
			return;
		}
		++tmu;
	}

	UNLOCK(MEM_LOCK);
	return;
}

inline uint32_t pmalloc()
{
    //return kmalloc_int(sz, 1, 0,2,1,4);
    uint32_t mem=phy_alloc4K();
  //  if(pag)
  //    MapPage((void*)mem,(void*)mem);
   return mem;
}

uint32_t __attribute__((optimize("O2"))) mtalloc(uint32_t pages)
{
   return (uint32_t)pmem_4k(pages);
	return (uint32_t)kmalloc(pages*4096);
   //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
}
/*/

const uint32_t mtalc_start = 500*1024*1024;
const uint32_t mtalc_end = 700*1024*1024;

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
   printf("\nCould not find memory, sorry. Try kmalloc()");
	 while(1);
   return (uint32_t)pmem_4k(pages);//(uint32_t)kmalloc(pages*4096);
   //_printf("\n%x %x", PAGE_GET_PHYSICAL_ADDRESS(page), mtalc_start);
}//*/
void __attribute__((optimize("O2"))) mtfree(uint32_t addr, uint32_t size)
{/*
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
   }/*/
	size = 0;
   	printf("[%d]", size);
	pfree((void*)addr);//*/
}

uint32_t fsalloc(uint32_t sz)
{
  uint32_t addr=(uint32_t)pmem(sz);
  //printf(" Ab%x",addr);
  return addr;
}

