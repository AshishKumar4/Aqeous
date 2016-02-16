#ifndef TASK_h
#define TASK_h

#include <common.h>
#include <mem.h>
#include <paging.h>

#define MAX_TASKS 100;
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

typedef struct
{
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
    uint32_t error_code;
}irqregs_t;

// This structure defines a 'task' - a process.
typedef struct task
 {
    uint32_t esp;
    int id;                // Process ID.
    char* name; // Process name
    registers_t regs;
    uint32_t StackTop,ds,es,fs,gs;
    char used;
    uint32_t ss; //stack segment registers
    pdirectory *pdir; // Page directory.
    struct task *next;     // The next task in a linked list.
 } task_t;

task_t* StartTask,*currentTask;

uint32_t old_esp,current_esp;

uint32_t NumberOfTasks;
// Initialises the tasking system.
void initTasking();

void createTask(task_t* task,void (*func)(), char *name, uint32_t flags, pdirectory *pagedir);

// Called by the timer hook, this changes the running process.
void switch_task();

// Forks the current process, spawning a new one with a different
// memory space.
int fork();

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, u32int size);

// Returns the pid of the current process.
int getpid();

typedef volatile struct __tss_struct
{
    unsigned short   link;
    unsigned short   link_h;

    unsigned long   esp0;
    unsigned short   ss0;
    unsigned short   ss0_h;

    unsigned long   esp1;
    unsigned short   ss1;
    unsigned short   ss1_h;

    unsigned long   esp2;
    unsigned short   ss2;
    unsigned short   ss2_h;

    unsigned long   cr3;
    unsigned long   eip;
    unsigned long   eflags;

    unsigned long   eax;
    unsigned long   ecx;
    unsigned long   edx;
    unsigned long    ebx;

    unsigned long   esp;
    unsigned long   ebp;

    unsigned long   esi;
    unsigned long   edi;

    unsigned short   es;
    unsigned short   es_h;

    unsigned short   cs;
    unsigned short   cs_h;

    unsigned short   ss;
    unsigned short   ss_h;

    unsigned short   ds;
    unsigned short   ds_h;

    unsigned short   fs;
    unsigned short   fs_h;

    unsigned short   gs;
    unsigned short   gs_h;

    unsigned short   ldt;
    unsigned short   ldt_h;

    unsigned short   trap;
    unsigned short   iomap;

} tss_struct_t;

#endif
