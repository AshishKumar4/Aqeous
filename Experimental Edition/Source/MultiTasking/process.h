#ifndef PROCESS_h
#define PROCESS_h

#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"

typedef struct __attribute__((packed)) Process
{
    uint_fast32_t *pgdir; // Page directory.
    uint_fast32_t esp;
    uint_fast32_t *code;
    uint32_t *stack; //user stack
    uint32_t *kernel_stack; //kernel_stack
    char name[24]; // Process name
    uint32_t processID;
    uint32_t priority;
    uint32_t active;
    task_table_t* first_task_entry; ///First Made Task
    task_table_t* last_task_entry; ///Last Made Task
    uint32_t total_tasks;
    struct Process* parent;
    //uint32_t ss; //stack segment registers
}Process_t;

#endif
