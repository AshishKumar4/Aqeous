#include "tasking.h"
#include "task.h"
#include "process.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "Scheduler/Scheduler.h"

#include "IPCInterface/IPCInterface.h"

task_t* create_task(char* name, func_t func, uint32_t priority, uint32_t flags, Process_t* process)  /// Create a New Task for a given Process
{
	task_table_t* New_task_entry = (task_table_t*)mtalloc(8);
	task_t* New_task = &New_task_entry->task;
	uint32_t* stack = (uint32_t*)New_task;

	faster_memset((void*)stack,0,4096);

	New_task_entry->next=NULL;
//	New_task_entry->test = 2;
	//Process_t* process = process_ptr;
	if(process->pgdir != system_dir)
		map((uint32_t)New_task_entry,8192*2,(PageDirectory_t*)process->pgdir);

	if(!process->first_task_entry)
	{
		process->first_task_entry = New_task_entry;
		process->last_task_entry = New_task_entry;
		New_task_entry->back = New_task_entry;
	}
	else
	{
		process->last_task_entry->next = New_task_entry;
		New_task_entry->back = process->last_task_entry;
		process->last_task_entry = New_task_entry;
	}

	++process->total_tasks;
	++process->counter;

	strcpy(New_task->name, name);
//	New_task->name = name;
	New_task->pId = process->processID;
	New_task->priority = priority;
	New_task->process = (uint32_t*)process;
	New_task->pgdir = process->pgdir;
	New_task->task_id = process->counter;

	New_task->main_pgdir = process->pgdir;
	New_task->mem_used = 0;

	stack+=4095;

	uint32_t base=(uint32_t)stack;

	*--stack = flags; //EFLAGS
	*--stack = 0x8; //cs
	*--stack = (uint32_t)func; //eip

	*--stack = 0; // eax
	*--stack = 0; // ecx
	*--stack = 0; // edx;
	*--stack = 0; //ebx
	*--stack = (uint32_t)(base-16); //esp
	*--stack = (uint32_t)(base-16); //ebp
	*--stack = 0; //esi
	*--stack = 0; //edi

	New_task->esp = (uint32_t)stack;
	New_task->tokens = priority;
	New_task->func = (uint32_t)func;

	//TODO: Complete this function, check everything thrice

	return New_task;
}

void Task_Refresh(task_t* task, func_t func)  /// Refresh a task to give it a new memory region
{/*
	//printf("\n22333");
	SchedulerKits_t* kit = task->Scheduler;
	if(kit != Get_Scheduler())
	{
		IPCPacket_t p, *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_REFRESH;
		//pp->no_opt = 1;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;
		//pp->options[0] = (uint32_t)func;

		IPCdispatch(pp);

		task->delivery_flag = 0x9999;
		//_printf("\n11\n");

		while(!(task->delivery_flag == 2))
		{
			//_printf("A %d ", task->delivery_flag);
			asm volatile("int $51");
		}
		task->delivery_flag = 0;

		_printf("ASDSASDASD");
		return;
	}
	_printf("\n555");*/
	task_t* New_task = task;
	uint32_t* stack = (uint32_t*)New_task;
	//stack += 512;

	//faster_memset((void __attribute__((optimize("O0")))*)stack,0,3584);

	New_task->mem_used = 0;

	stack+=4095;//3583;

	uint32_t base=(uint32_t)stack;

	*--stack = 0x202; //EFLAGS
	*--stack = 0x8; //cs
	*--stack = (uint32_t)func; //eip

	*--stack = 0; // eax
	*--stack = 0; // ecx
	*--stack = 0; // edx;
	*--stack = 0; //ebx
	*--stack = (uint32_t)(base-16); //esp
	*--stack = (uint32_t)(base-16); //ebp
	*--stack = 0; //esi
	*--stack = 0; //edi

	New_task->esp = (uint32_t)stack;
/*
	printf("\nASXXXXXX");
	if(New_task->delivery_flag == 0x9999)
	{
			New_task->delivery_flag = 2;
			printf("\nASXXXXXX");
	}
*/
}

void __attribute__((optimize("O0"))) Activate_task(task_table_t* task_entry) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
	task_t* task = &task_entry->task;
	//TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE
	SchedulerKits_t* kit = FindLightestScheduler();

	if(kit != Get_Scheduler())
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_ACTIVATE;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//while(!pp->done) asm volatile("int $51");

		return;
	}

	task->Scheduler = (SchedulerKits_t*)kit;
	uint32_t* _q=kit->queue_start;
	//printf("\nAx%x ",_q);
	_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
	//printf(" Bx%x %x ",_q, *_q);
	++(*_q); ///Create a new entry, extend the queue
	//printf(" Cx%x ",*_q);

	///TODO: Implement what to do when tasks in a queue become more then 1024!!!

	uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
	//printf(" Dx%x ",q_entry);
	*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
	task->active = (uint32_t)q_entry;
	///TODO: The Other things to do while making a queue entry!!!
	if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
		kit->top_queue = _q; //Put it as the top most queue

//  if(reached_bottom)
	kit->reached_bottom = 0;
}

void __attribute__((optimize("O0"))) Activate_task_direct(task_t* task) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
	//TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

	//LOCK(TASK_LOCK_ATD);
	SchedulerKits_t* kit = FindLightestScheduler();

	if(kit != Get_Scheduler() && multitasking_ON)
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_ACTIVATE;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//UNLOCK(TASK_LOCK_ATD);
		//while(!pp->done) asm volatile("int $51");

		return;
	}

	task->Scheduler = (SchedulerKits_t*)kit;
	uint32_t* _q=kit->queue_start;
	//printf("\nAx%x ",_q);
	_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
	//printf(" Bx%x %x ",_q, *_q);
	++(*_q); ///Create a new entry, extend the queue
	//printf(" Cx%x ",*_q);

	///TODO: Implement what to do when tasks in a queue become more then 1024!!!

	uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
	//printf(" Dx%x ",q_entry);
	*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
	task->active = (uint32_t)q_entry;
	///TODO: The Other things to do while making a queue entry!!!
	if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
		kit->top_queue = _q; //Put it as the top most queue

//  if(reached_bottom)
	kit->reached_bottom = 0;
	task->tokens = task->priority;
	++kit->tasks;

	//UNLOCK(TASK_LOCK_ATD);
}

void __attribute__((optimize("O0"))) Activate_task_direct_SP(task_t* task, SchedulerKits_t* kit) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
	//TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

	if(kit != Get_Scheduler() && multitasking_ON)
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_ACTIVATE;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//while(!pp->done) asm volatile("int $51");

		return;
	}

	task->Scheduler = (SchedulerKits_t*)kit;
	uint32_t* _q=kit->queue_start;
	//printf("\nAx%x ",_q);
	_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
	//printf(" Bx%x %x ",_q, *_q);
	++(*_q); ///Create a new entry, extend the queue
	//printf(" Cx%x ",*_q);

	///TODO: Implement what to do when tasks in a queue become more then 1024!!!

	uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
	//printf(" Dx%x ",q_entry);
	*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
	task->active = (uint32_t)q_entry;
	///TODO: The Other things to do while making a queue entry!!!
	if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
		kit->top_queue = _q; //Put it as the top most queue

//  if(reached_bottom)
	kit->reached_bottom = 0;
	++kit->tasks;

}

void __attribute__((optimize("O0"))) Task_Swap(task_t* new, task_t* original)
{
	//printf("\n77");
	SchedulerKits_t* kit = original->Scheduler;
	if(kit != Get_Scheduler() && multitasking_ON)
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_SWAP;
		pp->no_opt = 1;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = new;
		pp->options[0] = (uint32_t*)original;

		pp->done = 0;
		IPCdispatch(pp);
			//printf("\n88");
		//while(!pp->done) asm volatile("int $51");

			//printf("\n99");
		return;
	}

	*(uint32_t*)(original->active) = (uint32_t)new;
	new->active = original->active;
	original->active = 0;

	new->Scheduler = kit;

	//printf("\nasdas11");
}

void __attribute__((optimize("O0"))) kill()	// Know on which core the kill function has been called. Then get the scheduler of the core and get the current_task
{
	LOCK(TASK_LOCK_KILL);
	asm volatile("cli");
	switch_directory(system_dir);
	SchedulerKits_t* kit = Get_Scheduler();//Get the Scheduler of the core on which this instruction is executed.
	uint32_t *place_holder = (uint32_t *)((task_t*)kit->current_task)->active;
	*place_holder = (uint32_t)kit->Spurious_task;

	Process_t* proc = ((task_t*)kit->current_task)->process;
	--proc->total_tasks;
	task_table_t* tbl = kit->current_task;
	tbl->back->next = tbl->next;
	tbl->next->back = tbl->back;

	memset_fast((void *)kit->current_task, 0, 16);
	//mtfree(kit->current_task, 2);
	kit->current_task = (uint32_t)Idle_task;
	--kit->tasks;
	kit->curr_dir = (uint32_t*)system_dir;
	UNLOCK(TASK_LOCK_KILL);
	asm volatile("int $51");
//	while(1);
}

void __attribute__((optimize("O0"))) _kill(task_t* task)	// Know on which core the kill function has been called. Then get the scheduler of the core and get the current_task
{
	SchedulerKits_t* kit = task->Scheduler;//Get the Scheduler of the core on which this instruction is executed.

	if(kit != Get_Scheduler())
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_KILL;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//while(!pp->done) asm volatile("int $51");

		return;
	}

	uint32_t *place_holder = (uint32_t *)task->active;
	*place_holder = (uint32_t)kit->Spurious_task;

	Process_t* proc = task->process;
	--proc->total_tasks;
	task_table_t* tbl = task;
	tbl->back->next = tbl->next;
	tbl->next->back = tbl->back;

	memset_fast((void *)task, 0, 16);
	mtfree(task, 2);
	--kit->tasks;

}
/*
void __attribute__((optimize("O0"))) Pause_ALL_CPUs()
{

}
*/
void __attribute__((optimize("O0"))) kill_with_func(func_t func)
{
	asm volatile("cli");
	switch_directory(system_dir);
	SchedulerKits_t* kit = Get_Scheduler();//Get the Scheduler of the core on which this instruction is executed.
	uint32_t *place_holder = (uint32_t *)((task_t*)kit->current_task)->active;
	*place_holder = (uint32_t)kit->Spurious_task;

	Process_t* proc = ((task_t*)kit->current_task)->process;
	--proc->total_tasks;
	task_table_t* tbl = kit->current_task;
	tbl->back->next = tbl->next;
	tbl->next->back = tbl->back;

	memset_fast((void *)kit->current_task, 0, 16);
	mtfree(kit->current_task, 2);
	kit->current_task = (uint32_t)Idle_task;
	--kit->tasks;
	kit->curr_dir = (uint32_t*)system_dir;
	func();
	asm volatile("int $51");
}

void __attribute__((optimize("O0"))) Priority_promoter(task_t* task)
{
	if(task->active)
	{
		//printf("\n999\n");

		SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;
		if(kit != Get_Scheduler())
		{
			IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
			pp->type = TASK_PRIORITY_REFRESH;
			pp->no_opt = 0;
			pp->source = Get_Scheduler();
			pp->destination = kit;
			pp->task = task;

			pp->done = 0;
			IPCdispatch(pp);
			//while(!pp->done) asm volatile("int $51");

			return;
		}
		uint32_t volatile *place_holder = (uint32_t*)task->active;
		*place_holder = (uint32_t)kit->Spurious_task; //Spurious task is a task which would kill itself to remove the void.

		uint32_t* _q=kit->queue_start;

		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		++(*_q); ///Create a new entry, extend the queue

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
		task->active = (uint32_t)q_entry;

		if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
			kit->top_queue = _q; //Put it as the top most queue

		//if(reached_bottom)
		kit->reached_bottom = 0;

		task->tokens = task->priority;
	}

}

void __attribute__((optimize("O0"))) Task_sleep(task_t* task)
{
	if(task->active)
	{
		SchedulerKits_t* kit = task->Scheduler;
		if(kit != Get_Scheduler())
		{
			IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
			pp->type = TASK_SLEEP;
			pp->no_opt = 0;
			pp->source = Get_Scheduler();
			pp->destination = kit;
			pp->task = task;

			pp->done = 0;
			IPCdispatch(pp);
			//while(!pp->done) asm volatile("int $51");
			return;
		}
		uint32_t volatile *place_holder = (uint32_t volatile *)task->active;
		*place_holder = (uint32_t)kit->Spurious_task;
		task->active = 0;
	}
}
/*
inline void Bottom_task_remover(task_t* task)
{
	SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;
	uint32_t* tmp = kit->bottom_queue + (*kit->bottom_queue);
	if(*tmp == (uint32_t)task)
	{
		--(*kit->bottom_queue);
	}
}
*/
void __attribute__((optimize("O0"))) Task_wakeup(task_t* task)
{
	if(!task->active)
	{
		//TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE
		SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;

		if(kit != Get_Scheduler())
		{
			IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
			pp->type = TASK_WAKEUP;
			pp->no_opt = 0;
			pp->source = Get_Scheduler();
			pp->destination = kit;
			pp->task = task;

			pp->done = 0;
			IPCdispatch(pp);
			//while(!pp->done) asm volatile("int $51");

			return;
		}

		uint32_t* _q=kit->queue_start;
		//printf("\nAx%x ",_q);
		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		//printf(" Bx%x %x ",_q, *_q);
		++(*_q); ///Create a new entry, extend the queue
		//printf(" Cx%x ",*_q);

		///TODO: Implement what to do when tasks in a queue become more then 1024!!!

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
		task->active = (uint32_t)q_entry;
		///TODO: The Other things to do while making a queue entry!!!
		if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
			kit->top_queue = _q; //Put it as the top most queue

	//	if(kit->reached_bottom)
			kit->reached_bottom = 0;

		task->tokens = task->priority;
	}
}

void __attribute__((optimize("O0"))) Task_wakeup_direct(task_t* task)
{
	if(!task->active)
	{
		//TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE
		SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;

		uint32_t* _q=kit->queue_start;
		//printf("\nAx%x ",_q);
		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		//printf(" Bx%x %x ",_q, *_q);
		++(*_q); ///Create a new entry, extend the queue
		//printf(" Cx%x ",*_q);

		///TODO: Implement what to do when tasks in a queue become more then 1024!!!

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
		task->active = (uint32_t)q_entry;
		///TODO: The Other things to do while making a queue entry!!!
		if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
			kit->top_queue = _q; //Put it as the top most queue

	//	if(kit->reached_bottom)
			kit->reached_bottom = 0;

		task->tokens = task->priority;
	}
}

void __attribute__((optimize("O0"))) Priority_changer(task_t* task, uint32_t new_priority)
{
	if(task->active)
	{
		SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;

		if(kit != Get_Scheduler())
		{
			IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
			pp->type = TASK_PRIORITY_CHANGE;
			pp->no_opt = 1;
			pp->source = Get_Scheduler();
			pp->destination = kit;
			pp->task = task;
			pp->options[0] = new_priority;

			pp->done = 0;

			IPCdispatch(pp);

			//while(!pp->done) asm volatile("int $51");

			return;
		}

		uint32_t volatile *place_holder = (uint32_t*)task->active;
		*place_holder = (uint32_t)kit->Spurious_task; //Spurious task is a task which would kill itself to remove the void.

		uint32_t* _q=kit->queue_start;
		task->priority = new_priority;

		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		++(*_q); ///Create a new entry, extend the queue

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
		task->active = (uint32_t)q_entry;

		if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
			kit->top_queue = _q; //Put it as the top most queue

		//if(reached_bottom)
		kit->reached_bottom = 0;

		task->tokens = task->priority;
	}

}

inline void cli()
{
	if(!cli_already)
	{
		asm volatile("cli");
	}
	++cli_already;
}

inline void sti()
{
	if(!cli_already)
	{
		asm volatile("sti");
	}
	--cli_already;
}

inline void sti_int50()

{
	if(!cli_already)
	{
		asm volatile("sti;\
		int $51");
	}
	--cli_already;
//  asm volatile("int $51");
}
