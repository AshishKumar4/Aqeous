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
  Switch_to_system_dir();
  kb_queue_t* new_entry = kb_Qalloc();

  if(!q_elements)
  {
    Start_q = new_entry;
    Last_q = Start_q;
    Start_q->next = 0;
    Shell_sleep();
  }
  else
  {
    Last_q->next = new_entry;
    Last_q = new_entry;
    Last_q->next = 0;
  }

  ++q_elements;

  new_entry->task = current_task;
  new_entry->buffer = (uint32_t*)str;
  new_entry->str_ln = length;

  //TODO: Remove the Current task from the Scheduing Queue!!!

  uint32_t* place_holder = ((task_t*)current_task)->active;
  *place_holder = Spurious_task; //Spurious task is a task which would kill itself to remove the void.
  Switch_back_from_System();
  asm volatile("sti;\
  int $50;");
}
