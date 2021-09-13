#include "library.h"
#include "MultiTasking/tasking.h"
#include "MultiTasking/ThreadTable.h"

void ThreadTable_init()
{
    ThT_phy_mm = pmem_4k(THREADTABLE_SIZE/4096);
  /*  Unmap_non_identity(ThT_phy_mm, ThT_phy_mm, THREADTABLE_SIZE, system_dir);
    Map_non_identity(ThT_phy_mm, THREADTABLE_VMEM, THREADTABLE_SIZE, system_dir);*/
}

ThreadTableEntry_t* ThreadTable_alloc()
{
    LOCK(THREADTABLE_LOCK);
    ThreadTableEntry_t* t = ThT_phy_mm;
    for(int i = 0; i < THREADTABLE_MAX_ENTRYS; i++)
    {
        if(t[i].task == 0)
        {
           // printf("[%x]", i);
            UNLOCK(THREADTABLE_LOCK);
            return &t[i];
        }
    }
    UNLOCK(THREADTABLE_LOCK);
}

void ThreadTable_free(ThreadTableEntry_t* entry)
{
    entry->task = 0;
}

void ThreadTable_SetEntry(task_t* task)
{
    ThreadTableEntry_t* e = ThreadTable_alloc();
    task->ThreadTable_entry = (uint32_t)e;
    e->task = task;
    e->pgdir = task->pgdir;
}