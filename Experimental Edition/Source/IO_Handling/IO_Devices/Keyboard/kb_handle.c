#include "kb_handle.h"
#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "queues.h"
#include "shell.h"

void kb_io_init()
{
  kb_Start_q = 0;
  kb_Last_q = 0;
}

void kb_getline(char* str, uint32_t length)
{
//  asm volatile("cli");
/*
  kb_queue_t* new_entry = (kb_queue_t*)kb_Qalloc();

  Shell_sleep();
  if(!q_elements)
  {
    kb_Start_q = new_entry;
    kb_Last_q = kb_Start_q;
    kb_Start_q->next = 0;
  }
  else
  {kb_kb_Start_q
    kb_Last_q->next = new_entry;
    kb_Last_q = new_entry;
    kb_Last_q->next = 0;
  }

  ++q_elements;

  new_entry->task = (task_t*)current_task;
  new_entry->buffer = (uint32_t*)str;
  new_entry->str_ln = length;

  uint32_t* place_holder = (uint32_t*)((task_t*)current_task)->active;
  *place_holder = (uint32_t)Spurious_task; //Spurious task is a task which would kill itself to remove the void.
  ((task_t*)current_task)->active = 0;*/
  Serial_input(str, length, KB_INPUT_FLAG, current_task, 1, (serials_i_struct_t **)&kb_Start_q, (serials_i_struct_t **)&kb_Last_q);
  asm volatile("sti;\
  int $50;");
}
