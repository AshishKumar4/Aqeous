#ifndef THREADTABLE_H
#define THREADTABLE_H 

#include "library.h"
#include "tasking.h"
#include "threadtable.h"

#define THREADTABLE_VMEM                0xC2A00000
#define THREADTABLE_SIZE                (2048*1024)

typedef struct ThreadTableEntry
{
    task_t* task;
    PageDirectory_t* pgdir;
}ThreadTableEntry_t;

volatile ThreadTableEntry_t* ThT_phy_mm, *ThT_vir_mm = (ThreadTableEntry_t*)THREADTABLE_VMEM;

#define THREADTABLE_MAX_ENTRYS          THREADTABLE_SIZE/sizeof(ThreadTableEntry_t)


ThreadTableEntry_t* ThreadTable_alloc();
void ThreadTable_free(ThreadTableEntry_t* entry);
void ThreadTable_SetEntry(task_t* task);

#endif 