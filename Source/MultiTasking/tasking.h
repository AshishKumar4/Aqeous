#ifndef TASK_h
#define TASK_h

#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "stdint.h"
#include "fs.h"

#define QUEUE_START 209715200
#define LAST_QUEUE 209797120
#define TOTAL_QUEUES 20

uint32_t* volatile top_queue=(uint32_t*)LAST_QUEUE;
uint32_t volatile reached_bottom=0,bottom_task=1;
int volatile cli_already = 0;
bool volatile multitasking_ON = false;

// This structure defines a 'task' - a process.
typedef struct __attribute__((packed)) _task //DO NOT CHANGE ANYTHING UNLESS YOU HAVE A REASON; Make changes in tasking.asm too then.
{
    uint32_t esp;
    uint32_t pId;                // Process ID.
    uint32_t* process; //Parent Process Address
    //uint32_t kernel_stack;
    uint32_t priority;
    uint32_t active;
    char* name; //Parent Process name
}task_t;

task_t* Idle_task, *Shell_task, *Spurious_task;


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

Process_t *kernel_proc, *Shell_proc;

Process_t* create_process(char* name, uint32_t* code, uint32_t priority, Process_t* parent);  /// Create a New Task for a given Process

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

void Spurious_task_func();

void Priority_promoter(task_t* task);

void test_process();

#endif
