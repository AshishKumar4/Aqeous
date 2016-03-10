#ifndef TASK_h
#define TASK_h

#include <common.h>
#include <mem.h>
#include <sys.h>
#include <paging.h>

#define MAX_TASKS 10000

typedef struct
{
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

// This structure defines a 'task' - a process.
typedef struct __attribute__((packed)) task
 {
    uint8_t id;                // Process ID.
    uint16_t segment;
    char* name; // Process name
    uint32_t eip,esp;
    uint16_t eflags,cs;
    uint8_t priority :4;
    uint8_t active :4;
    //uint32_t ss; //stack segment registers
    pdirectory *pdir; // Page directory.
    struct task *next;     // The next task in a linked list.
 } task_t;

 task_t* iolocks[32];

typedef struct queues
{
     int istask;
     uint32_t usage,ticks;
     task_t* task;
     struct queues* next;
}queue_t;

uint8_t  timeslice[10];

uint32_t queue[10],last_element[10];

uint32_t timer_tick=5,rn=0,higher_tick=4,current_queue=0,max_priority=0,queue_serials[10];

task_t* StartTask,*currentTask;

uint32_t old_esp,current_esp;

uint32_t NumberOfTasks;

// Initialises the tasking system.
void initTasking();

void createTask(task_t* task,void (*func)(), char *name, uint32_t flags, uint32_t priority, pdirectory *pagedir);

// Called by the timer hook, this changes the running process.
void scheduler();

// Forks the current process, spawning a new one with a different
// memory space.

// Returns the pid of the current process.
int getpid();

void IOLock(uint8_t irq);

void taskSleep();

/***In tasking.asm***/
extern void switcher();
extern void save_eip();
extern uint32_t new_eip;
extern uint32_t new_cs;
extern uint32_t new_eflags;
extern uint32_t old_eip;
extern uint32_t old_cs;
extern uint32_t old_eflags;
extern uint32_t old_esp;
extern uint32_t new_esp;
extern uint16_t new_seg;

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
