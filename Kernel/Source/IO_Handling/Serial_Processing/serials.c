
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "Shell/shell.h"
#include "stdio.h"
#include "MultiTasking/tasking.h"
#include "std_iohandling.h"
#include "serials.h"

void Serial_input(char* str, uint32_t length, uint32_t INPUT_FLAG, uint32_t task, uint32_t shell_kill, serials_i_struct_t** q_start, serials_i_struct_t** q_end, uint32_t* queue_counter)
{
  std_io_struct_t* new_std_entry = (std_io_struct_t*)std_Qalloc();
  new_std_entry->TAN = STD_INPUT_FLAG | INPUT_FLAG;
  new_std_entry->task = (task_t*)task;
  serials_i_struct_t* new_entry = (serials_i_struct_t*)new_std_entry->type_io_struct;

  if(shell_kill)
    Shell_sleep();
  if(!(*queue_counter))
  {
    *q_start = new_entry;
    *q_end = *q_start;
    (*q_start)->next = 0;
  }
  else
  {
    (*q_end)->next = new_entry;
    *q_end = new_entry;
    (*q_end)->next = 0;
  }

  ++(*queue_counter);

  new_entry->task = (task_t*)task;
  new_entry->buffer = (uint32_t*)str;
  new_entry->size = length;

  uint32_t* place_holder = (uint32_t*)((task_t*)task)->active;
  Task_Remover(place_holder, ((task_t*)task)->Scheduler);
 // *place_holder = (uint32_t)Get_Scheduler()->Spurious_task; //Spurious task is a task which would kill itself to remove the void.
  ((task_t*)task)->active = 0;
}

void Serial_output(char* str, uint32_t length, uint32_t OUTPUT_FLAG, uint32_t task, uint32_t shell_kill, serials_o_struct_t** q_start, serials_o_struct_t** q_end, uint32_t* queue_counter)
{
  std_io_struct_t* new_std_entry = (std_io_struct_t*)std_Qalloc();
  new_std_entry->TAN = STD_OUTPUT_FLAG | OUTPUT_FLAG;
  new_std_entry->task = (task_t*)task;
  serials_o_struct_t* new_entry = (serials_o_struct_t*)new_std_entry->type_io_struct;

  if(shell_kill)
    Shell_sleep();
  if(!(*queue_counter))
  {
    *q_start = new_entry;
    *q_end = *q_start;
    (*q_start)->next = 0;
  }
  else
  {
    (*q_end)->next = new_entry;
    *q_end = new_entry;
    (*q_end)->next = 0;
  }

  ++(*queue_counter);

  new_entry->task = (task_t*)task;
  new_entry->buffer = (uint32_t*)str;
  new_entry->size = length;

  uint32_t* place_holder = (uint32_t*)((task_t*)task)->active;
  Task_Remover(place_holder, ((task_t*)task)->Scheduler);
//  *place_holder = (uint32_t)Get_Scheduler()->Spurious_task; //Spurious task is a task which would kill itself to remove the void.
  ((task_t*)task)->active = 0;
}
