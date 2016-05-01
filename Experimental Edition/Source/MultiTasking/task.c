#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"

void create_task(char* name, func_t func, uint32_t priority, uint32_t flags, Process_t* process)  /// Create a New Task for a given Process
{
  Switch_to_system_dir();
  task_table_t* New_task_entry = (task_table_t*)tmalloc(sizeof(task_table_t));
  struct task_t* New_task = &New_task_entry->task;
  New_task_entry->next=NULL;

  process->last_task_entry->next = New_task_entry;
  process->last_task_entry = New_task_entry;

  New_task->priority = priority;
  New_task->eip = (uint_fast32_t)func;
  uint32_t* stack = smalloc(4096); ///TODO:Make this stack take memory alloted to process itself!!!
  //New_task->stack = stack;
  New_task->name = name;
  New_task->id = process->processID;

  memset((void*)stack,0,4096);
  stack+=1023;

  uint32_t base=(uint32_t)stack;

  *--stack = flags; //EFLAGS
  *--stack = 0x8; //cs
  *--stack = func; //eip

  *--stack = 0; // eax
  *--stack = 0; // ecx
  *--stack = 0; // edx;
  *--stack = 0; //ebx
  *--stack = base; //esp
  *--stack = base; //ebp
  *--stack = 0; //esi
  *--stack = 0; //edi

  New_task->esp = (uint32_t)stack;
  New_task->cs = 0x8;
  //TODO: Complete this function, check everything thrice
  Switch_back_from_System();
}

void Activate_task(task_table_t* task_entry) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
  Switch_to_system_dir(); ///Get into the Kernel!!!
  struct task_t* task = &task_entry->task;
  //TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

  uint32_t* _q=QUEUE_START;
  _q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
  ++*_q; ///Create a new entry, extend the queue

  ///TODO: Implement what to do when tasks in a queue become more then 1024!!!

  uint32_t* q_entry = _q + *_q; ///Get the pointer to the new entry
  *q_entry = (uint32_t*)task; ///Fill in the Entry with the address of the new Task!!!
  ///TODO: The Other things to do while making a queue entry!!!
  if(_q<top_queue)  //If the Top most queue earlier was below this queue,
    top_queue = _q; //Put it as the top most queue
  Switch_back_from_System(); ///Get back now
}
