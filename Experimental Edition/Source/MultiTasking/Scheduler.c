#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "timer.h"
#include "paging.h"
#include "stdint.h"
#include "APIC.h"
#include "PIC.h"
/*
void Scheduler_init()
{
}
*/

//TODO: Extend Base Queue size so that it can store 20480 tasks.

void __attribute__((optimize("O0"))) Scheduler() ///The Actual Engine :D
{
  uint32_t* _q=top_queue; ///Always start searching from the top most available queue

  if(reached_bottom)  ///Already in the lowest queue? As here, tasks are of low priority, we dont need it to be fast :D
  {
    uint32_t* tmp;
    if(bottom_task<(*_q))
      ++bottom_task;
    else bottom_task=1;
    back:
    tmp=_q+bottom_task; ///round robin manner
    if(*tmp)
    {
      current_task=*tmp;
      ((task_t*)current_task)->active = (uint32_t)tmp;
    }
    else
    {
      ++bottom_task;
      goto back;
    }
  }
  else if(*_q == 1) ///Only one task in the queue??
  {
    uint32_t *first_curr=_q + 1;
    --(*_q);
    _q+=1024; ///1024*4 increment
    ++(*_q);
    uint32_t *last_lower=_q + (*_q); ///Last queue element of lower queue
    *last_lower=*first_curr;
    *first_curr=0; //deinitialize it
    current_task=*last_lower;
    top_queue=_q;
    if((uint32_t)_q==LAST_QUEUE) reached_bottom=1;
    ((task_t*)current_task)->active = (uint32_t)last_lower;
  }
  else ///General
  {
    uint32_t *first_curr=_q + 1,*last_curr=_q + (*_q); ///First and Last queue elements of current queue
    --(*_q);
    _q+=1024; ///1024*4 increment
    ++(*_q);
    uint32_t *last_lower=_q + (*_q); ///Last queue element of lower queue
    *last_lower=*first_curr;
    *first_curr=*last_curr; //swap values
    *last_curr=0; //deinitialize it
    current_task=*last_lower;
    ((task_t*)current_task)->active = (uint32_t)last_lower;
  }
  time_slice = ((uint32_t)top_queue & 0xfffff)>>5;
}

/*********************************** SCHEDULER ASSISTANCE SYSTEM ***********************************/

void SAS_init()
{
  asm volatile("cli");
  SAS_booster = create_task("SAS_task_booster", SAS_task_booster, 1, 0x202, SAS_proc);
  Activate_task_direct(SAS_booster);
  SAS_eraser = create_task("SAS_void_eraser", SAS_void_eraser, 1, 0x202, SAS_proc);
  Activate_task_direct(SAS_eraser);
  SAS_booster->special = 1;
  SAS_eraser->special = 1;
  kill();

  asm volatile("sti;\
  int $50");
}

void __attribute__((optimize("O0"))) SAS_task_booster()
{
  while(1)
  {
    if(sas_cleaning_complete)
    {
      asm volatile("cli");
      //printf("\n Ax");
      //_printf("\n%s", ((task_t*)highest_task_entry)->name);
      if(highest_task_entry && ((task_t*)highest_task_entry)->active)
      {
        task_t* task = (task_t*)highest_task_entry;
        task->tokens = task->priority;
      //  _printf(" %s", task->name);

        uint32_t* place_holder = (uint32_t*)task->active;
        *place_holder = (uint32_t)Spurious_task; //Spurious task is a task which would kill itself to remove the void.

        uint32_t* _q=(uint32_t*)LAST_QUEUE;

        _q-=2048; ///Get into the queue required
        ++(*_q); ///Create a new entry, extend the queue

        uint32_t* q_entry = _q + (*_q); ///Get the pointer to the new entry
        //printf(" Dx%x ",q_entry);
        *q_entry = (uint32_t)task; ///Fill in the Entry with the address of the new Task!!!
        task->active = (uint32_t)q_entry;

        if(_q<top_queue)  //If the Top most queue earlier was below this queue,
          top_queue = _q; //Put it as the top most queue

      //if(reached_bottom)
        reached_bottom = 0;
      }
      sas_cleaning_complete = 0;
    }
    asm volatile("int $50");
  }
}

void __attribute__((optimize("O0"))) SAS_void_eraser()
{
  uint32_t tasks_searched = 1;
  while(1)
  {
    if(!sas_cleaning_complete)
    {
      asm volatile("cli");
      if(*bottom_queue >= tasks_searched)
      {
      //  _printf("\n%x",*bottom_queue);
        uint32_t *tmp = bottom_queue + tasks_searched;
        if(!*tmp || *tmp == (uint32_t)Spurious_task)
        {
          uint32_t* last_task = bottom_queue + (*bottom_queue);
          *tmp = *last_task;
          *last_task = 0;
          ((task_t*)*tmp)->active = (uint32_t)tmp;
          --(*bottom_queue);
        }
        else if(!((task_t*)*tmp)->special)
        {
          if(((task_t*)*tmp)->tokens > last_highest_priority)
          {
            highest_task_entry = (uint32_t*)*tmp;
            last_highest_priority = ((task_t*)*tmp)->tokens;
          }
          ((task_t*)*tmp)->tokens += 1;
          //_printf("\n%s",((task_t*)*tmp)->name);
        }
        ++tasks_searched;
      }
      else
      {
        //sas_boosting_complete = 0;
        sas_cleaning_complete = 1;
        tasks_searched = 1;
        last_highest_priority = 0;
      }
    }
    asm volatile("int $50");
  }
}

//TODO: CREATE AN ADVANCED INTEGRATED SYSTEM WHICH WOULD CONTROL THE WHOLE MULTITASKING SYSTEM AND THE SCHEDULER, The Timer, the Interrupts etc Literally!. It should act as the BOSS of the Scheduler!!!
