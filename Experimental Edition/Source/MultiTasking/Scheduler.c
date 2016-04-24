#include "task.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "stdint.h"

void Scheduler_init()
{
}

void Scheduler()
{
  uint_fast32_t* _q=top_queue; ///Always start searching from the top most available queue
  if(reached_bottom)  ///Already in the lowest queue?
  {
    uint_fast32_t* tmp=_q+reached_bottom; ///round robin manner
    current_task=*tmp;
    if(reached_bottom<1023)
      ++reached_bottom;
    else reached_bottom=1;
  }
  else if(*_q==1) ///Only one task in the queue??
  {
    uint_fast32_t *first_curr=_q + 1;
    --*_q;
    _q+=1024; ///1024*4 increment
    ++*_q;
    uint_fast32_t *last_lower=_q + *_q; ///Last queue element of lower queue
    *last_lower=*first_curr;
    *first_curr=0; //deinitialize it
    current_task=*last_lower;
    top_queue=_q;
    if(_q==LAST_QUEUE) reached_bottom=1;
  }
  else ///General
  {
    uint_fast32_t *first_curr=_q + 1,*last_curr=_q + *_q; ///First and Last queue elements of current queue
    --*_q;
    _q+=1024; ///1024*4 increment
    ++*_q;
    uint_fast32_t *last_lower=_q + *_q; ///Last queue element of lower queue
    *last_lower=*first_curr;
    *first_curr=*last_curr; //swap values
    *last_curr=0; //deinitialize it
    current_task=*last_lower;
    //top_queue=_q;
  }
}
//TODO: REMOVE SAVE_ESP 
void save_esp() ///USED BY switcher() function in tasking.asm
{
    old_task->esp = old_esp;
    new_esp = current_task->esp;
    new_eip = current_task->eip;
    new_cs = current_task->cs;
    new_eflags = current_task->eflags;
    new_seg = current_task->segment;
}
