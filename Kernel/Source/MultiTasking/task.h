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

#include "MultiTasking/Scheduler/scheduler.h"

// This structure defines a 'task' - a process.
typedef struct __attribute__((packed)) _task //DO NOT CHANGE ANYTHING UNLESS YOU HAVE A REASON; Make changes in tasking.asm too then.
{
    uint32_t esp;                   //0
    uint32_t pId;                // Process ID. 4
    uint32_t* process; //Parent Process Address     8
    uint32_t pgdir;             //12
    //uint32_t kernel_stack;
    uint32_t priority;          //16
    uint32_t tokens;            //20
    uint32_t active;            //24
    uint32_t ThreadTable_entry;         //28
    uint32_t magic;
    uint32_t special;
    uint32_t RtFlags;
    uint32_t mem_used;
    uint32_t task_id;
    uint32_t func;
    SchedulerKits_t* Scheduler;
    uint32_t delivery_flag; //For checking if any applied function on the task is executed or not
    char name[50]; //Parent Process name
}task_t;

task_t *Kernel_task;
task_t *Idle_task, *Shell_task, *Shell_Istream_task, *Shell_Ostream_task;

enum RUNTIME_FLAGS 
{
    THREAD_RT_FLG_ShellSleep            =           0b0000000000000000000000000000001,
};

typedef struct __attribute__((packed)) task_table
{
    task_t task;
//    uint32_t test;
    struct task_table* next;
    struct task_table* back;
}task_table_t;

DECLARE_LOCK(TASK_LOCK_KILL);
DECLARE_LOCK(TASK_LOCK_ATD);

task_t* task_GetSelf();

void Activate_task(task_table_t* task_entry);

void Activate_task_direct(task_t* task);

void Activate_task_direct_SP(task_t* task, SchedulerKits_t* kit);
void Activate_task_strict_SP(task_t* task, SchedulerKits_t* kit);

void Task_Remover(uint32_t* place_holder, SchedulerKits_t* kit);

void kill();

void _kill(task_t* task);

void Task_sleep(task_t* task);

void Task_wakeup(task_t* task);

void Task_wakeup_direct(task_t* task);

void Task_Swap(task_t* new, task_t* original);

#include "MultiTasking/process.h"

task_t* create_task(char* name, void (*func)(), uint32_t priority, uint32_t flags, Process_t* process);

void Task_Refresh(task_t* task, func_t func);

void Priority_changer(task_t* task, uint32_t new_priority);

void Priority_promoter(task_t* task);

void test_ab(uint32_t s, uint32_t d);

//void SAS_Catalouge_handler();

void SAS_task_booster();

void SAS_init();

inline void Bottom_task_remover(task_t* task);

void SAS_void_eraser();

#endif
