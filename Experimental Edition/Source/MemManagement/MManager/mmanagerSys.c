/************ Memory Management And Defragmentation System ************/
#include "MManager/mmanagerSys.h"
#include "phy_mm/mem.h"
#include "CustomCSRB/csrb_custom.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"

#include "tasking.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "shell.h"

void Setup_MMADS()
{
	MMADS_Process = (uint32_t*)create_process("MMADS", 0, 1, kernel_proc);
	MMADS_csrbTweakerThread = (uint32_t*)create_task("MMADS_csrbTweaker",MMADS_csrbTweaker, 0, 0x202, (Process_t*)MMADS_Process);
	Activate_task_direct((task_t*)MMADS_csrbTweakerThread);
	mmads_stack_start = (uint32_t*)(12*1024*1024); //12th MB
	mmads_stack_end = mmads_stack_start;
}

void __attribute__((optimize("O0"))) MMADS_csrbTweaker()
{
	CustomCSRB_M_t* tt;
	CustomCSRB_M_t* tl;
	Process_t* process;
	while(1)
	{
		if(mmads_stack_size)
		{
			//printf("\nMMADS");
			--mmads_stack_end;
			--mmads_stack_size;
			process = (Process_t*)mmads_stack_end;
			Pdir_Capsule_t* pdcap = (Pdir_Capsule_t*)process->pgdir;
			CustomCSRB_M_header_t* csrb_u = (CustomCSRB_M_header_t*)pdcap->csrb_u;
			CustomCSRB_M_t* tmp = (CustomCSRB_M_t*)csrb_u->head;
			for(;csrb_u->changed;)
			{
				tmp = (CustomCSRB_M_t*)tmp->addr;
				if(tmp->reserved)
				{
					//Pop element from tail and put it here!
					tt = (CustomCSRB_M_t*)csrb_u->tail;
					if(!(((uint32_t)(tt))%4128))	//If tail is on new page
					{
						tl = tt;
						--tt;
						tt = (CustomCSRB_M_t*)tt->addr;
						--tt;
						csrb_u->tail = (uint32_t*)tt;
						tmp->begin = tt->begin;
						tmp->size = tt->size;
						tmp->reserved = 0;
						tt->addr = csrb_u->head;
						push_frameStack(ROUNDDOWN(((uint32_t)tl),4096));
					}
					else
					{
						--tt;
						csrb_u->tail = (uint32_t*)tt;
						tmp->begin = tt->begin;
						tmp->size = tt->size;
						tmp->reserved = 0;
						tt->addr = csrb_u->head;
					}
					--csrb_u->changed;
				}
				++tmp;
			}
			CustomCSRB_M_header_t* csrb_f = (CustomCSRB_M_header_t*)pdcap->csrb_f;
			tmp = (CustomCSRB_M_t*)csrb_f->head;
			for(;csrb_f->changed;)
			{
				tmp = (CustomCSRB_M_t*)tmp->addr;
				if(tmp->reserved)
				{
					tt = (CustomCSRB_M_t*)csrb_f->tail;
					if(!(((uint32_t)(tt))%4168))	//If tail is on new page
					{
						tl = tt;
						--tt;
						tt = (CustomCSRB_M_t*)tt->addr;
						--tt;
						csrb_f->tail = (uint32_t*)tt;
						tmp->begin = tt->begin;
						tmp->size = tt->size;
						tmp->reserved = 0;
						tt->addr = csrb_f->head;
						push_frameStack(ROUNDDOWN(((uint32_t)tl),4096));
					}
					else
					{
						--tt;
						csrb_f->tail = (uint32_t*)tt;
						tmp->begin = tt->begin;
						tmp->size = tt->size;
						tmp->reserved = 0;
						tt->addr = csrb_f->head;
					}
					--csrb_f->changed;
				}
				++tmp;
			}
		}
		asm volatile("int $50");
	}
}
