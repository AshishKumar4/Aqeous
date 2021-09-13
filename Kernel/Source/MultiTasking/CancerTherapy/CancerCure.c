#include "MultiTasking/tasking.h"
#include "MultiTasking/task.h"
#include "MultiTasking/Scheduler/scheduler.h"
#include "stdio.h"
#include "stdlib.h"

#include "CancerCure.h"

extern uint32_t CancerCure_addr;

void CancerCure_init()
{
  CancerCure_addr = (uint32_t)CancerCure;
}

void CancerCure(uint32_t fault, uint32_t eip)
{
  asm volatile("cli");
  //task_t* fault_task = (task_t*)(Get_Scheduler()->current_task);
  printf("\nFault Detected!!! Fault number: %d, %d", fault, eip);
  Shell_Dbuff_sync();
  asm volatile("hlt");
  //printf("\tTask: ", fault_task->name);
  //Task_Refresh(fault_task, fault_task->func);
  //Get the Current Task.
  //Replace it with Spurious Task
  //Respawn the Current Task IF POSSIBLE
  asm volatile("int $50");
  //kill();
  //while(1);
}
