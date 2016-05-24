#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "timer.h"
#include "paging.h"
#include "stdint.h"
/*
void Scheduler_init()
{
}
*/

void Scheduler()
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
      current_task=*tmp;
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
}

/*********************************** SCHEDULER ASSISTANCE SYSTEM ***********************************/

void Task_Promoter() ///Select a task from the end of the base queue, pop it, and promote it to higher queue based on its priority.
{
  if(reached_bottom)  ///Already in the lowest queue? As here, tasks are of low priority, we dont need it to be fast :D
  {
  //  uint32_t* tmp;
    uint32_t* _q = LAST_QUEUE;
    if(bottom_task == *_q)
      bottom_task = 1;
    uint32_t* base_last = _q + (*_q);
    --(*_q);
    if(!*_q) reached_bottom = 0;
    _q=(uint32_t*)QUEUE_START;
    _q+=(1024*(TOTAL_QUEUES - ((task_t*)*base_last)->priority)); ///Get into the queue required
    ++(*_q);
    uint32_t* upper_last = _q + (*_q);
    *upper_last = *base_last;
    *base_last = 0;
    if(_q < top_queue) top_queue = _q;
  }
}
