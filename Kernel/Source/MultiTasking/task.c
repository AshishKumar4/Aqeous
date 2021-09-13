#include "MultiTasking/tasking.h"
#include "MultiTasking/task.h"
#include "MultiTasking/process.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "MultiTasking/Scheduler/scheduler.h"
#include "MultiTasking/ThreadTable.h"

#include "IPCInterface/IPCInterface.h"

task_t* create_task(char* name, func_t func, uint32_t priority, uint32_t flags, Process_t* process)  /// Create a New Task for a given Process
{
	task_table_t* New_task_entry = (task_table_t*)pmem_4k(2);
//	printf("[%x:%s]", New_task_entry, name);
	task_t* New_task = &New_task_entry->task;
	uint32_t* stack = (uint32_t*)New_task;

	memset_fast((void*)stack,0,4096);

//	New_task_entry->test = 2;
	//Process_t* process = process_ptr;
	if(process->pgdir != (uint32_t)system_dir)
		map_kernelOnly((uint32_t)New_task_entry,4096*2,(PageDirectory_t*)process->pgdir);

	if(!process->total_tasks)
	{
		process->first_task_entry = New_task_entry;
		process->last_task_entry = New_task_entry;
		New_task_entry->back = New_task_entry;
		//New_task_entry->next = process->first_task_entry;
	}
	else
	{
		process->last_task_entry->next = New_task_entry;
		New_task_entry->back = process->last_task_entry;
		process->last_task_entry = New_task_entry;
	}
	New_task_entry->next = process->first_task_entry;

	++process->total_tasks;
	++process->counter;

	strcpy(New_task->name, name);
//	New_task->name = name;
	New_task->pId = process->processID;
	New_task->priority = priority;
	New_task->process = (uint32_t*)process;
	New_task->pgdir = process->pgdir;
	New_task->task_id = process->counter;

	//New_task->main_pgdir = process->pgdir;
	New_task->mem_used = 0;

	uint32_t ring_level = process->privileges & 0xff;
	uint32_t cs = cs_TallyTbl[ring_level];
	uint32_t ds = ds_TallyTbl[ring_level];

	stack += 1024;
	//stack += 512;

	uint32_t base=(uint32_t)stack;

	if(ring_level)
	{
		*--stack = ds;		// ss
		*--stack = (uint32_t)(base);
	//	printf("\n{Stack: %d %x}", stack, stack);
	}

	*--stack = flags; //EFLAGS
	*--stack = cs;//0x8; //cs
	*--stack = (uint32_t)func; //eip

//	*--stack = ds;		// ds, es, fs, gs

	*--stack = 0x4284; // eax
	*--stack = 0; // ecx
	*--stack = 0; // edx;
	*--stack = 0; //ebx
	*--stack = (uint32_t)(base-16); //esp
	*--stack = (uint32_t)(base-16); //ebp
	*--stack = 0; //esi
	*--stack = 0; //edi

	*--stack = ds;		// ds, es, fs, gs

	New_task->esp = (uint32_t)stack;
	New_task->tokens = priority;
	New_task->func = (uint32_t)func;
	New_task->magic = 0x42841999;

	//TODO: Complete this function, check everything thrice

	return New_task;
}

void Task_Refresh(task_t* task, func_t func)  /// Refresh a task to give it a new memory region
{
	/*SchedulerKits_t* kit = (SchedulerKits_t*)task->Scheduler;
	if(kit != Get_Scheduler())
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_REFRESH;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//while(!pp->done) asm volatile("int $51");

		return;
	}*/
	task_t* New_task = task;
	uint32_t* stack = (uint32_t*)New_task;
	//stack += 512;

	//faster_memset((void __attribute__((optimize("O0")))*)stack,0,3584);

	New_task->mem_used = 0;

	uint32_t ring_level = ((Process_t*)New_task->process)->privileges & 0xff;
	uint32_t cs = cs_TallyTbl[ring_level];
	uint32_t ds = ds_TallyTbl[ring_level];


	stack += 1024;
	stack += 512;

	uint32_t base=(uint32_t)stack;

	if(ring_level)
	{
		*--stack = ds;		// ss
		*--stack = (uint32_t)(base-24);
	}

	*--stack = 0x202; //EFLAGS
	*--stack = cs;//0x8; //cs
	*--stack = (uint32_t)func; //eip

	*--stack = 0; // eax
	*--stack = 0; // ecx
	*--stack = 0; // edx;
	*--stack = 0; //ebx
	*--stack = (uint32_t)(base-16); //esp
	*--stack = (uint32_t)(base-16); //ebp
	*--stack = 0; //esi
	*--stack = 0; //edi

	*--stack = ds;		// ds, es, fs, gs

	New_task->esp = (uint32_t)stack;
	New_task->magic = 0x42841999;
/*
	printf("\nASXXXXXX");
	if(New_task->delivery_flag == 0x9999)
	{
			New_task->delivery_flag = 2;
			printf("\nASXXXXXX");
	}
*/
}

task_t* __attribute__((optimize("O0"))) task_GetSelf()
{
	return (task_t*)Get_Scheduler()->current_task;
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

	ThreadTableEntry_t* tht = ThreadTable_alloc();
	tht->task = task;
	tht->pgdir = (PageDirectory_t*)task->pgdir;
	task->ThreadTable_entry = (uint32_t)tht;
	*q_entry = (uint32_t)tht; ///Fill in the Entry with the address of the new Task!!!

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

	ThreadTableEntry_t* tht = ThreadTable_alloc();
	tht->task = task;
	tht->pgdir = (PageDirectory_t*)task->pgdir;
	task->ThreadTable_entry = (uint32_t)tht;
	*q_entry = (uint32_t)tht; ///Fill in the Entry with the address of the new Task!!!

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
	//printf("\n{%d}", kit->identity);
	if(kit != Get_Scheduler() && multitasking_ON)
	{
		IPCPacket_t *pp = kmalloc(sizeof(IPCPacket_t));
		pp->type = TASK_ACTIVATE;
		pp->no_opt = 0;
		pp->source = Get_Scheduler();
		pp->destination = kit;
		//printf("\n{%d}", pp->destination->identity);
		pp->task = task;

		pp->done = 0;
		IPCdispatch(pp);
		//while(!pp->done) asm volatile("int $51");

		return;
	}//*/

	task->Scheduler = (SchedulerKits_t*)kit;
	uint32_t* _q=kit->queue_start;
	//printf("\nAx%x ",_q);
	_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
	//printf(" Bx%x %x ",_q, *_q);
	++(*_q); ///Create a new entry, extend the queue
	//printf(" Cx%x ",*_q);

	///TODO: Implement what to do when tasks in a queue become more then 1024!!!

	uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry

	ThreadTableEntry_t* tht = ThreadTable_alloc();
	tht->task = task;
	tht->pgdir = (PageDirectory_t*)task->pgdir;
	//printf("\n<[%x]",tht->pgdir);
	task->ThreadTable_entry = (uint32_t)tht;
	*q_entry = (uint32_t)tht; ///Fill in the Entry with the address of the new Task!!!

	task->active = (uint32_t)q_entry;
	///TODO: The Other things to do while making a queue entry!!!
	if(_q<kit->top_queue)  //If the Top most queue earlier was below this queue,
		kit->top_queue = _q; //Put it as the top most queue

//  if(reached_bottom)
	kit->reached_bottom = 0;
	++kit->tasks;

}


void __attribute__((optimize("O0"))) Activate_task_strict_SP(task_t* task, SchedulerKits_t* kit) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
	task->Scheduler = (SchedulerKits_t*)kit;
	uint32_t* _q=kit->queue_start;
	//printf("\nAx%x ",_q);
	_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
	//printf(" Bx%x %x ",_q, *_q);
	++(*_q); ///Create a new entry, extend the queue
	//printf(" Cx%x ",*_q);

	///TODO: Implement what to do when tasks in a queue become more then 1024!!!

	uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry

	ThreadTableEntry_t* tht = ThreadTable_alloc();
	tht->task = task;
	tht->pgdir = (PageDirectory_t*)task->pgdir;
	//printf("\n<[%x]",tht->pgdir);
	task->ThreadTable_entry = (uint32_t)tht;
	*q_entry = (uint32_t)tht; ///Fill in the Entry with the address of the new Task!!!

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
		pp->options[0] = (uint32_t)original;

		pp->done = 0;
		IPCdispatch(pp);
			//printf("\n88");
		//while(!pp->done) asm volatile("int $51");

			//printf("\n99");
		return;
	}

	*(uint32_t*)(original->active) = (uint32_t)new->ThreadTable_entry;
	new->active = original->active;
	original->active = 0;

	new->Scheduler = kit;

	//printf("\nasdas11");
}

void __attribute__((optimize("O0"))) Task_Remover(uint32_t* place_holder, SchedulerKits_t* kit)		// Remove the task from scheduler queues
{
	*place_holder = 0;
	uint32_t* qptr = (uint32_t*)(((uint32_t)place_holder) & 0xfffff000);		// Get the page aligned queue start location
	if(qptr != kit->queue_last)
	{
		--(*qptr);		// Remove one entry from the queue
		uint32_t* tmpqptr = qptr;
		while((*tmpqptr) == 0)		// Until we find a queue with tasks, keep going to lower queues
		{
			tmpqptr += 1024;
		}
		//asm volatile("hlt");
		// We must certainly have a queue with tasks now. So set the Top Queue and other parameters of the scheduler
		kit->top_queue = tmpqptr;
		if(tmpqptr == kit->queue_last)
			kit->reached_bottom = 1;
	}
	else  
	{
	//	*place_holder = (uint32_t)kit->Spurious_task;
		uint32_t* last = qptr + (*qptr);
		--(*qptr);		// Remove one entry from the queue
		*place_holder = *last;
	}
}

void __attribute__((optimize("O0"))) kill()	// Know on which core the kill function has been called. Then get the scheduler of the core and get the current_task
{
	LOCK(TASK_LOCK_KILL);
	asm volatile("cli");
	switch_directory(system_dir);
	SchedulerKits_t* kit = Get_Scheduler();//Get the Scheduler of the core on which this instruction is executed.

	uint32_t *place_holder = (uint32_t *)((task_t*)kit->current_task)->active;
//	*place_holder = (uint32_t)kit->Spurious_task;

	Task_Remover(place_holder, kit);

	Process_t* proc = (Process_t*)(((task_t*)kit->current_task)->process);
	--proc->total_tasks;
	task_table_t* tbl = (task_table_t*)kit->current_task;

	if(tbl == proc->first_task_entry)
	{
		proc->first_task_entry = tbl->next;
	}
	else if(tbl == proc->last_task_entry)
	{
		proc->last_task_entry = tbl->back;
	}

	if(tbl->back && tbl->next)
	{
		tbl->back->next = tbl->next;
		tbl->next->back = tbl->back;
	}

	if(((task_t*)kit->current_task)->RtFlags & THREAD_RT_FLG_ShellSleep)
	{
		Shell_Softwakeup();
	}

	ThreadTable_free((ThreadTableEntry_t*)((task_t*)kit->current_task)->ThreadTable_entry);

	memset_fast((void *)kit->current_task, 0, sizeof(task_t));
	//mtfree(kit->current_task, 2);
	kit->current_task = (uint32_t)Idle_task;
	--kit->tasks;
	kit->current_pdir = (uint32_t*)system_dir;
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

	if(task->active)
	{
		uint32_t *place_holder = (uint32_t *)task->active;
	//	*place_holder = (uint32_t)kit->Spurious_task;

		Task_Remover(place_holder, kit);
		//printf("<---->");
	}
	else
	{
		//asm volatile("mov $0x1234, %eax; hlt");
	}

	Process_t* proc = (Process_t*)(task->process);
	--proc->total_tasks;
	task_table_t* tbl = (task_table_t*)task;

	if(tbl == proc->first_task_entry)
	{
		proc->first_task_entry = tbl->next;
	}
	else if(tbl == proc->last_task_entry)
	{
		proc->last_task_entry = tbl->back;
	}

	if(tbl->back && tbl->next)
	{
		tbl->back->next = tbl->next;
		tbl->next->back = tbl->back;
	}
	
	if(task->RtFlags & THREAD_RT_FLG_ShellSleep)
	{
		Shell_Softwakeup();
	}
	ThreadTable_free((ThreadTableEntry_t*)task->ThreadTable_entry);
	memset_fast((void *)task, 0, 16);
	//mtfree(task, 2);
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
	//*place_holder = (uint32_t)kit->Spurious_task;

	Task_Remover(place_holder, kit);

	Process_t* proc =(Process_t*)(((task_t*)kit->current_task)->process);
	--proc->total_tasks;
	task_table_t* tbl =(task_table_t*)(kit->current_task);
	
	if(tbl == proc->first_task_entry)
	{
		proc->first_task_entry = tbl->next;
	}
	else if(tbl == proc->last_task_entry)
	{
		proc->last_task_entry = tbl->back;
	}

	if(((task_t*)kit->current_task)->RtFlags & THREAD_RT_FLG_ShellSleep)
	{
		Shell_Softwakeup();
	}

	ThreadTable_free((ThreadTableEntry_t*)((task_t*)kit->current_task)->ThreadTable_entry);

	memset_fast((void *)kit->current_task, 0, 16);
	mtfree(kit->current_task, 2);
	kit->current_task = (uint32_t)Idle_task;
	--kit->tasks;
	kit->current_pdir = (uint32_t*)system_dir;
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
		uint32_t *place_holder = (uint32_t*)task->active;
	//	*place_holder = (uint32_t)kit->Spurious_task; //Spurious task is a task which would kill itself to remove the void.

		Task_Remover(place_holder, kit);

		uint32_t* _q=kit->queue_start;

		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		++(*_q); ///Create a new entry, extend the queue

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)(task->ThreadTable_entry); ///Fill in the Entry with the address of the new Task!!!
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
		uint32_t *place_holder = (uint32_t *)task->active;
	//	*place_holder = (uint32_t)kit->Spurious_task;

		Task_Remover(place_holder, kit);
		task->active = 0;
	}
}

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
		*q_entry = (uint32_t)(task->ThreadTable_entry); ///Fill in the Entry with the address of the new Task!!!
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
		*q_entry = (uint32_t)(task->ThreadTable_entry); ///Fill in the Entry with the address of the new Task!!!
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

		uint32_t *place_holder = (uint32_t*)task->active;
	//	*place_holder = (uint32_t)kit->Spurious_task; //Spurious task is a task which would kill itself to remove the void.

		Task_Remover(place_holder, kit);	

		uint32_t* _q=kit->queue_start;
		task->priority = new_priority;

		_q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
		++(*_q); ///Create a new entry, extend the queue

		uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
		//printf(" Dx%x ",q_entry);
		*q_entry = (uint32_t)(task->ThreadTable_entry); ///Fill in the Entry with the address of the new Task!!!
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
