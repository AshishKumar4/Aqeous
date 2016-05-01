#ifndef TASK_h
#define TASK_h

#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "stdint.h"

#define QUEUE_START 50331648
#define LAST_QUEUE 50413568
#define TOTAL_QUEUES 20

uint_fast32_t* top_queue=QUEUE_START,reached_bottom=0,bottom_task=1;

// This structure defines a 'task' - a process.
struct __attribute__((packed)) task_t
{
    uint_fast32_t eip;
    uint_fast32_t cs;
    uint_fast32_t eflags;
    uint_fast32_t esp;
    uint32_t id;                // Process ID.
    uint32_t* process; //Parent Process Address
    uint16_t priority;
    uint16_t active;
    char* name; //Parent Process name
    //struct task *next;     // The next task in a linked list.
};

typedef struct __attribute__((packed)) task_table
{
    struct task_t task;
    struct task_table* next;
}task_table_t;


//struct task_t* current_task,*old_task;

extern uint32_t current_task;
extern uint32_t old_task;
extern uint32_t old_process;
extern uint32_t new_process;

void Scheduler();

#endif
