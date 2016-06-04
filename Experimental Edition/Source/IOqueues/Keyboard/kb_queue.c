#include "kb_queue.h"
#include "tasking.h"
#include "mem.h"
#include "stdio.h"
#include "queues.h"
#include "shell.h"

void kb_io_init()
{
  Start_q = 0;
  Last_q = 0;
}

void kb_getline(char* str, uint32_t length)
{
  asm volatile("cli");
  kb_queue_t* new_entry = kb_Qalloc();
  Last_q->next = new_entry;
  Last_q = new_entry;
  Last_q->next = 0;

  if(!Start_q)
  {
    Start_q = Last_q;
    Shell_sleep();
  }

  ++q_elements;

  new_entry->task = current_task;
  new_entry->buffer = (uint32_t*)str;
  new_entry->str_ln = length;

  //TODO: Remove the Current task from the Scheduing Queue!!!

  uint32_t* place_holder = ((task_t*)current_task)->active;
  *place_holder = Spurious_task; //Spurious task is a task which would kill itself to remove the void.


  //TODO: Create a better way of doing this shit instead of this way!!!

  asm volatile("sti;\
  int $50;");

  return;
}
