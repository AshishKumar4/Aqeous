#ifndef TASK_h
#define TASK_h

#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "stdint.h"

#define QUEUE_START 209715200
#define LAST_QUEUE 209797120
#define TOTAL_QUEUES 20

uint32_t* top_queue=(uint32_t*)LAST_QUEUE;
uint32_t reached_bottom=0,bottom_task=1;

// This structure defines a 'task' - a process.
typedef struct __attribute__((packed)) _task //DO NOT CHANGE ANYTHING UNLESS YOU HAVE A REASON; Make changes in tasking.asm too then.
{
    uint32_t esp;
    uint32_t pId;                // Process ID.
    uint32_t process; //Parent Process Address
    uint32_t kernel_stack;
    uint16_t priority;
    uint16_t active;
    char* name; //Parent Process name
}task_t;

task_t* Idle_task;

#include <process.h>

//task_t* current_task,*old_task;

extern uint32_t current_task;
extern uint32_t old_task;
extern uint32_t old_process;
extern uint32_t new_process;
extern uint32_t current_esp;
extern uint32_t old_esp;
extern void Scheduler_init();

//void Activate_task(task_table_t* task_entry);
void Activate_task_direct(task_t* task);
task_t* create_task(char* name, void (*func)(), uint32_t priority, uint32_t flags, Process_t* process);

#endif
