#ifndef TASK_h
#define TASK_h

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdint.h"
#include "fs.h"

#include "Scheduler/Scheduler.h"

// This structure defines a 'task' - a process.
typedef struct __attribute__((packed)) _task //DO NOT CHANGE ANYTHING UNLESS YOU HAVE A REASON; Make changes in tasking.asm too then.
{
    uint32_t esp;
    uint32_t pId;                // Process ID.
    uint32_t* process; //Parent Process Address
    uint32_t pgdir;
    //uint32_t kernel_stack;
    uint32_t priority;
    uint32_t tokens;
    uint32_t active;
    uint32_t special;
    uint32_t main_pgdir;
    uint32_t mem_used;
    uint32_t task_id;
    uint32_t* Scheduler;
    char name[50]; //Parent Process name
}task_t;

task_t *Kernel_task;
task_t *Idle_task, *Shell_task, *Shell_Istream_task, *Shell_Ostream_task;

typedef struct __attribute__((packed)) task_table
{
    task_t task;
//    uint32_t test;
    struct task_table* next;
    struct task_table* back;
}task_table_t;

void Activate_task(task_table_t* task_entry);

void Activate_task_direct(task_t* task);

void Activate_task_direct_SP(task_t* task, SchedulerKits_t* kit);

void kill();

void _kill(task_t* task);

void Task_sleep(task_t* task);

void Task_wakeup(task_t* task);

#include "process.h"

task_t* create_task(char* name, void (*func)(), uint32_t priority, uint32_t flags, Process_t* process);

void Priority_changer(task_t* task, uint32_t new_priority);

void Spurious_task_func();

void Priority_promoter(task_t* task);

void test_ab(uint32_t s, uint32_t d);

//void SAS_Catalouge_handler();

void SAS_task_booster();

void SAS_init();

inline void Bottom_task_remover(task_t* task);

void SAS_void_eraser();

#endif
