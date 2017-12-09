#ifndef SCHEDULER_h
#define SCHEDULER_h

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

void Init_Scheduler();

extern void switcher_ksp_t();
extern void switcher_end_t();
extern void cw_ksp_ss_t();
extern void cw_p2_ndc_c_t();

uint32_t* MotherSpace;

typedef struct __attribute__((packed)) SchedulerKits
{
  uint32_t identity;
  uint32_t tasks;

  uint32_t* queue_start;
  uint32_t* queue_last;
  uint32_t* top_queue;
  uint32_t reached_bottom;
  uint32_t bottom_task;

  uint32_t* bottom_queue;

  uint32_t* scheduler;
  uint32_t* switcher;
  uint32_t* Scheduler_init;

  uint32_t* syscall_vector;

  uint32_t current_task;
  uint32_t old_task;
  uint32_t timeslice;
 // uint32_t* curr_dir;
  uint32_t* current_pdir;

  uint32_t* Spurious_task;
  uint32_t* stack;
  uint32_t* stack_top;
  uint32_t  tmp_threadStack;

  uint32_t Core_Main_Lock;

  uintptr_t IPCRecord_addr;
  uintptr_t gdt;
  uintptr_t idt;
}SchedulerKits_t;

SchedulerKits_t* KitList;

//TODO: Extend Base Queue size so that it can store 20480 tasks.

SchedulerKits_t* FindLightestScheduler();

SchedulerKits_t* Get_Scheduler();

extern void Scheduler();

extern void Scheduler_end();

extern void Scheduler_t();

extern void Scheduler_end_t();


/*********************************** SCHEDULER ASSISTANCE SYSTEM ***********************************/

void SAS_init();

void SAS_task_booster_t();

void SAS_task_booster_end();

void SAS_void_eraser_t();

void SAS_end_t();

#endif
