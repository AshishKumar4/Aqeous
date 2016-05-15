#ifndef PROCESS_h
#define PROCESS_h

#include <tasking.h>
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "fs.h"

typedef struct __attribute__((packed)) task_table
{
    task_t task;
    uint32_t test;
    struct task_table* next;
}task_table_t;

typedef struct __attribute__((packed)) Process_File_wrapper
{
    File_t* pFile;
    struct Process_File_wrapper* next;
}Process_File_wrapper_t;

typedef struct __attribute__((packed)) Process
{
    uint32_t pgdir; // Page directory.
    //uint32_t esp;
    uint32_t *code;
    //uint32_t *stack; //user stack
    uint32_t *kernel_stack; //kernel_stack
    char name[24]; // Process name
    uint32_t processID;
    uint32_t priority;
    uint32_t active;
    task_table_t* first_task_entry; ///First Made Task
    task_table_t* last_task_entry; ///Last Made Task
    uint32_t total_tasks;
    Process_File_wrapper_t* First_file;
    struct Process* parent;
    //uint32_t ss; //stack segment registers
}Process_t;

Process_t* create_process(char* name, uint32_t* code, uint32_t priority, Process_t* parent);  /// Create a New Task for a given Process

#endif
