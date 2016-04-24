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

uint_fast32_t* top_queue=QUEUE_START,reached_bottom=0;

// This structure defines a 'task' - a process.
struct __attribute__((packed)) task_t
{
    uint32_t id;                // Process ID.
    uint32_t priority;
    uint32_t active;
    uint_fast32_t segment;
    char* name; // Process name
    uint_fast32_t eip,esp;
    uint_fast32_t eflags,cs;
    //uint32_t ss; //stack segment registers
    uint_fast32_t *pgdir; // Page directory.
    uint32_t *stack; //stack
    //struct task *next;     // The next task in a linked list.
};

typedef struct __attribute__((packed)) task_table
{
    struct task_t task;
    struct task_table* next;
}task_table_t;

typedef struct __attribute__((packed)) Process
{
    uint32_t processID;
    uint32_t priority;
    uint32_t active;
    task_table_t* first_task_entry; ///First Made Task
    task_table_t* last_task_entry; ///Last Made Task
    uint32_t total_tasks;
    uint_fast32_t *pgdir;
}Process_t;

struct task_t* current_task,*old_task;

void Scheduler();

extern uint32_t new_eip;
extern uint32_t new_cs;
extern uint32_t new_eflags;
extern uint32_t old_eip;
extern uint32_t old_cs;
extern uint32_t old_eflags;
extern uint32_t old_esp;
extern uint32_t new_esp;
extern uint32_t new_seg;

#endif
