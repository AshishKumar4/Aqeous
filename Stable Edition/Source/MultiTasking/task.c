#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"

task_t* create_task(char* name, void (*func)(), uint32_t priority, uint32_t flags, Process_t* process)  /// Create a New Task for a given Process
{
  Switch_to_system_dir();
  task_table_t* New_task_entry = (task_table_t*)tmalloc(8192);
  New_task_entry->test = 2;
  task_t* New_task = &New_task_entry->task;
  New_task_entry->next=NULL;

  map((uint32_t)New_task_entry,8192,(pdirectory*)process->pgdir);

  process->last_task_entry->next = New_task_entry;
  process->last_task_entry = New_task_entry;

  uint32_t* stack = (uint32_t*)New_task;//smalloc(8192); ///TODO:Make this stack take memory alloted to process itself!!!
  //New_task->stack = stack;
  memset((void*)stack,0,8192);

  New_task->name = name;
  New_task->pId = process->processID;
  New_task->priority = priority;
  New_task->process = (uint32_t*)process;

  stack+=2048;

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
//  New_task->kernel_stack = (uint32_t)tmalloc(4096);
  //New_task->cs = 0x8;
  //TODO: Complete this function, check everything thrice
  Switch_back_from_System();
  return New_task;
}

void Activate_task(task_table_t* task_entry) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
  Switch_to_system_dir(); ///Get into the Kernel!!!
  task_t* task = &task_entry->task;
  //TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

  uint32_t* _q=(uint32_t*)QUEUE_START;
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
  if(_q<top_queue)  //If the Top most queue earlier was below this queue,
    top_queue = _q; //Put it as the top most queue

  if(reached_bottom)
    reached_bottom = 0;
  Switch_back_from_System();
}

void Activate_task_direct(task_t* task) /// Put a given Task_entry into an appropriate queue for it to be executed.
{
  Switch_to_system_dir(); ///Get into the Kernel!!!
  //TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

  uint32_t* _q=(uint32_t*)QUEUE_START;
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
  if(_q<top_queue)  //If the Top most queue earlier was below this queue,
    top_queue = _q; //Put it as the top most queue

  if(reached_bottom)
    reached_bottom = 0;
  Switch_back_from_System(); ///Get back now
}

void kill()
{
  asm volatile("cli");
  Switch_to_system_dir();

  uint32_t* volatile place_holder = ((task_t*)current_task)->active;
  *place_holder = 0;

//  current_task = Idle_task;

  uint32_t volatile temp = (uint32_t)place_holder/4096;
  uint32_t* volatile _q = temp*4096;
  --(*_q);

  Switch_back_from_System();
  //switch_pdirectory(system_dir);
  asm volatile("sti;\
  int $50");
  while(1);
}

void Priority_promoter(task_t* task)
{
  Switch_to_system_dir();
  uint32_t* place_holder = (uint32_t*)task->active;
  *place_holder = Spurious_task; //Spurious task is a task which would kill itself to remove the void.

  uint32_t* _q=(uint32_t*)QUEUE_START;

  _q+=(1024*(TOTAL_QUEUES - task->priority)); ///Get into the queue required
  ++(*_q); ///Create a new entry, extend the queue

  uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
  //printf(" Dx%x ",q_entry);
  *q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
  task->active = (uint32_t)q_entry;

  if(_q<top_queue)  //If the Top most queue earlier was below this queue,
    top_queue = _q; //Put it as the top most queue

  if(reached_bottom)
    reached_bottom = 0;

  Switch_back_from_System();
}

void Task_wakeup(task_t* task)
{
  Switch_to_system_dir(); ///Get into the Kernel!!!
  //TODO: Complete the algorithm to put a task into appropriate queue based on its priority, to the END of the QUEUE

  uint32_t* _q=(uint32_t*)QUEUE_START;
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
  if(_q<top_queue)  //If the Top most queue earlier was below this queue,
    top_queue = _q; //Put it as the top most queue

  if(reached_bottom)
    reached_bottom = 0;
  Switch_back_from_System(); ///Get back now
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
    int $50");
  }
  --cli_already;
//  asm volatile("int $50");
}
