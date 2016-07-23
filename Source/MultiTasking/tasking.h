#ifndef TASK_h
#define TASK_h

#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "stdint.h"
#include "fs.h"

#define QUEUE_START 33554432//209715200
#define LAST_QUEUE 33636352//209797120
#define TOTAL_QUEUES 20
const uint32_t TIME_MASK=0x000ff000;

uint32_t* volatile top_queue=(uint32_t*)LAST_QUEUE;
volatile uint32_t reached_bottom=0,bottom_task=1;
int volatile cli_already = 0;
volatile int multitasking_ON = 0;

// This structure defines a 'task' - a process.
typedef struct _task //DO NOT CHANGE ANYTHING UNLESS YOU HAVE A REASON; Make changes in tasking.asm too then.
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
    char* name; //Parent Process name
}task_t;

task_t *Kernel_task;
task_t *Idle_task, *Shell_task, *Spurious_task, *SAS_booster, *SAS_eraser, *Shell_Istream_task, *Shell_Ostream_task;


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

Process_t *kernel_proc, *Shell_proc, *SAS_proc;

uint32_t *bottom_queue = (uint32_t *)LAST_QUEUE;
volatile uint32_t sas_bottom_task = 1;

volatile uint32_t last_highest_priority = 0;
volatile uint32_t sas_cleaning_complete = 1;
volatile uint32_t* highest_task_entry = 0;


Process_t* create_process(char* name, uint32_t* code, uint32_t priority, Process_t* parent);  /// Create a New Task for a given Process

//task_t* current_task,*old_task;

extern uint32_t current_task;
extern uint32_t old_task;
extern uint32_t old_process;
extern uint32_t new_process;
extern uint32_t current_esp;
extern uint32_t old_esp;
extern uint32_t time_slice;
extern void Scheduler_init();

void Activate_task(task_table_t* task_entry);

void Activate_task_direct(task_t* task);

void kill();

void Task_sleep(task_t* task);

void Task_wakeup(task_t* task);

task_t* create_task(char* name, void (*func)(), uint32_t priority, uint32_t flags, Process_t* process);

void Spurious_task_func();

void Priority_promoter(task_t* task);

//void SAS_Catalouge_handler();

void SAS_task_booster();

void SAS_init();

inline void Bottom_task_remover(task_t* task);

void SAS_void_eraser();

#endif
