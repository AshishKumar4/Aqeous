#include "Console_handle.h"
#include "console.h"
#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "serials.h"
#include "std_iohandling.h"

void console_io_init()
{
  console_Start_q = 0;
  console_Last_q = 0;
}

void _stdprint(const char* str, uint32_t length)
{
  SchedulerKits_t* kit = Get_Scheduler();
  PageDirectory_t* curr_dir = kit->curr_dir;
  switch_directory(system_dir);
  Serial_output(str, length, CONSOLE_OUT_FLAG, kit->current_task, 0, (serials_o_struct_t **)&console_Start_q, (serials_o_struct_t **)&console_Last_q, &console_q_elements);
  asm volatile("int $50;");
  switch_directory(curr_dir);
}
