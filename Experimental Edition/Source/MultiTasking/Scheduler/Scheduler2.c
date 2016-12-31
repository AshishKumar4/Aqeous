#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "phy_mm/mem.h"
#include "timer.h"
#include "stdint.h"
#include "APIC.h"
#include "PIC.h"
#include "Scheduler/Scheduler.h"
#include "descriptors.h"
/*
Symbol Map:
    _cur_dir        ->        0x4284ACD1
    system_dir      ->        0x4284ACD2
    current_task    ->        0x4284ACD3
    old_task        ->        0x4284ACD4
    Scheduler       ->        0x4284ACD5
    time_slice      ->        0x4284ACD6
*/
/*
void Init_Scheduler()
{
  //Create copies of Scheduler function and switcher functions based on number of CPU cores.

  uint32_t sz_Scheduler = Scheduler_end - Scheduler;
  uint32_t sz_switcher = switcher_end - switcher_ksp_t;
  uint32_t sz_tot = sz_Scheduler + sz_switcher;
  uint32_t* nspace,*nptr32,*schdulr;
  uint8_t *nptr1, *nptr2;

  MotherSpace = phy_alloc4K();
  SchedulerKits_t* kits = MotherSpace;


  for(int i = 0; i < total_CPU_Cores; i++)
  {
    printf("\nCore #%x Initialized",i+1);
    nspace = phy_alloc4K();
    kits->identity = i;
    kits->tasks = 0;

    nptr1 = nspace;
    memcpy(nptr1, switcher_ksp_t, sz_switcher);
    kits->switcher = nptr1;

    nptr1 += sz_switcher;
    memcpy(nptr1, Scheduler, sz_Scheduler);
    schdulr = nptr1;
    kits->scheduler = nptr1;

    nptr1 += sz_Scheduler;
    nptr1 = ROUNDUP(nptr1,4) + 32;
    nptr32 = nptr1;

    ByteSequence_Replace(0x4284ACD1, 4, &_cur_dir, 4, kits->switcher, kits->switcher + sz_switcher); //_cur_dir
    *nptr32 = system_dir;
    kits->curr_dir = nptr32;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD2, 4, &system_dir, 4, kits->switcher, kits->switcher + sz_switcher); //system_dir
    *nptr32 = system_dir;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD3, 4, &current_task, 4, kits->switcher, kits->switcher + sz_switcher); //current_task
    *nptr32 = 0;
    kits->current_task = nptr32;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD4, 4, &old_task, 4, kits->switcher, kits->switcher + sz_switcher); //old_task
    *nptr32 = 0;
    kits->old_task = nptr32;
    ++nptr32;
    *nptr32 = Scheduler;
    ByteSequence_Replace(0x4284ACD5, 4, nptr32, 4, kits->switcher, kits->switcher + sz_switcher); //Scheduler
    ++nptr32;
    ByteSequence_Replace(0x4284ACD6, 4, &time_slice, 4, kits->switcher, kits->switcher + sz_switcher); //time_slice
    *nptr32 = 0;
    kits->timeslice = nptr32;
    ++nptr32;

    ByteSequence_Replace(0x4284ACD7, 4, *nptr32, 4, kits->switcher, kits->switcher + sz_switcher); //current_task
    *nptr32 = cw_ksp_ss_t;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD8, 4, *nptr32, 4, kits->switcher, kits->switcher + sz_switcher); //current_task
    *nptr32 = cw_p2_ndc_c_t;
    ++nptr32;

    kits->queue_start = kmalloc(4096*20);
    kits->queue_last = (uint32_t*)(((uint32_t)kits->queue_start) + (4096*20));
    kits->top_queue = kits->queue_last;
    kits->reached_bottom = 0;

    idtSetEntry(51, (uint32_t)&kits->switcher, 0x08, makeFlagByte(1, KERNEL_MODE));
    idtSetEntry(50, (uint32_t)&kits->switcher, 0x08, makeFlagByte(1, KERNEL_MODE));

    lidt((void *)&idt_ptr);
    ++kits;
  }
}*/
void Init_Scheduler()
{
  //Create copies of Scheduler function and switcher functions based on number of CPU cores.

  uint32_t sz_Scheduler = Scheduler_end - Scheduler;
  uint32_t sz_switcher = switcher_end - switcher_ksp_t;
  uint32_t sz_tot = sz_Scheduler + sz_switcher;
  uint32_t* nspace,*nptr32,*schdulr;
  uint8_t *nptr1, *nptr2;

  MotherSpace = phy_alloc4K();
  SchedulerKits_t* kits = MotherSpace;


  for(int i = 0; i < total_CPU_Cores; i++)
  {
    printf("\nCore #%x Initialized",i+1);
    nspace = phy_alloc4K();
    kits->identity = i;
    kits->tasks = 0;

    nptr1 = nspace;
    memcpy(nptr1, switcher_ksp_t, sz_switcher);
    kits->switcher = nptr1;

    nptr1 += sz_switcher;
    memcpy(nptr1, Scheduler, sz_Scheduler);
    schdulr = nptr1;
    kits->scheduler = nptr1;

    nptr1 += sz_Scheduler;
    nptr1 = ROUNDUP(nptr1,4) + 32;
    nptr32 = nptr1;

    ByteSequence_Replace(0x4284ACD1, 4, &_cur_dir, 4, switcher_ksp_t, switcher_end); //_cur_dir
    *nptr32 = system_dir;
    kits->curr_dir = nptr32;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD2, 4, &system_dir, 4, switcher_ksp_t, switcher_end); //system_dir
    *nptr32 = system_dir;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD3, 4, &current_task, 4, switcher_ksp_t, switcher_end); //current_task
    *nptr32 = 0;
    kits->current_task = nptr32;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD4, 4, &old_task, 4, switcher_ksp_t, switcher_end); //old_task
    *nptr32 = 0;
    kits->old_task = nptr32;
    ++nptr32;
    *nptr32 = Scheduler;
    ByteSequence_Replace(0x4284ACD5, 4, nptr32, 4, switcher_ksp_t, switcher_end); //Scheduler
    ++nptr32;
    ByteSequence_Replace(0x4284ACD6, 4, &time_slice, 4, switcher_ksp_t, switcher_end); //time_slice
    *nptr32 = 0;
    kits->timeslice = nptr32;
    ++nptr32;
/*
    ByteSequence_Replace(0x4284ACD7, 4, *nptr32, 4, switcher_ksp_t, switcher_end); //current_task
    *nptr32 = cw_ksp_ss_t;
    ++nptr32;
    ByteSequence_Replace(0x4284ACD8, 4, *nptr32, 4, switcher_ksp_t, switcher_end); //current_task
    *nptr32 = cw_p2_ndc_c_t;
    ++nptr32;
*/
    kits->queue_start = kmalloc(4096*20);
    kits->queue_last = (uint32_t*)(((uint32_t)kits->queue_start) + (4096*20));
    kits->top_queue = kits->queue_last;
    kits->reached_bottom = 0;

    idtSetEntry(51, (uint32_t)&kits->switcher, 0x08, makeFlagByte(1, KERNEL_MODE));
    idtSetEntry(50, (uint32_t)&kits->switcher, 0x08, makeFlagByte(1, KERNEL_MODE));

    lidt((void *)&idt_ptr);
    ++kits;
  }
}

SchedulerKits_t* FindLightestScheduler()
{
  SchedulerKits_t* tmp = MotherSpace;
  SchedulerKits_t* tmp2 = tmp;
  for(int i = 0; i < total_CPU_Cores; i++)
  {
    if(tmp2->tasks < tmp->tasks) tmp2 = tmp;
  }
  return tmp2;
}

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

void Scheduler_end()
{

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
