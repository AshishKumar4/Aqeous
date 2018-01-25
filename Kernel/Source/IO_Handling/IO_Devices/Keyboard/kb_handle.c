#include "kb_handle.h"
#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "queues.h"
#include "shell.h"
#include "Scheduler\Scheduler.h"

void kb_io_init()
{
  kb_Start_q = 0;
  kb_Last_q = 0;
}

void kb_getline(char* str, uint32_t length)
{
  SchedulerKits_t* kit = Get_Scheduler();
  PageDirectory_t* curr_dir = kit->current_pdir;
  switch_directory(system_dir);
  Serial_input(str, length, KB_INPUT_FLAG, kit->current_task, 0, (serials_i_struct_t **)&kb_Start_q, (serials_i_struct_t **)&kb_Last_q, (uint32_t*)&kb_q_elements);
  asm volatile("int $50;");
  switch_directory(curr_dir);
}

void kb_stdin_syscall(char* str, uint32_t length)
{
  SchedulerKits_t* kit = Get_Scheduler();
  //uint32_t tmp = ((task_t*)kit->current_task)->pgdir;
  ((task_t*)kit->current_task)->pgdir = (uint32_t)system_dir;
  Serial_input(str, length, KB_INPUT_FLAG, kit->current_task, 0, (serials_i_struct_t **)&kb_Start_q, (serials_i_struct_t **)&kb_Last_q, (uint32_t*)&kb_q_elements);
 /* asm volatile("int $50;");
  ((task_t*)kit->current_task)->pgdir = tmp;//*/
}
