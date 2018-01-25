#include "ProcManager/ProcManager.h"
#include "task.h"
#include "process.h"
#include "tasking.h"

ProcDirectory_t* PrDir_create()
{
    ProcDirectory_t* prdir = kmalloc(sizeof(ProcDirectory_t));
    memset(prdir,0,sizeof(ProcDirectory_t));
    return prdir;
}

ProcTable_t* PrTbl_create(uint32_t* procdir)
{
    ProcTable_t* prtbl = kmalloc(sizeof(ProcTable_t));
    ProcDirectory_t* prdir = (ProcDirectory_t*)procdir;
    memset(prtbl,0,sizeof(ProcTable_t));
    for(int i = 0; i<1024; i++)
    {
        if(!prdir->procTables[i])
        {
            prdir->procTables[i] = (uint32_t)prtbl;
            return prtbl;
        }
    }
    return 0;
}

int PrTbl_entry_i(uint32_t* proctbl, Process_t* process)
{
    ProcTable_t* prtbl = (ProcTable_t*)proctbl;
    for(int i = 0; i<1024; i++)
    {
        if(!prtbl->procEntrys[i])
        {
            prtbl->procEntrys[i] = (uint32_t)process;
            process->prdir_Entry = &prtbl->procEntrys[i];
            return 1;
        }
    }
    return 0;
}

int PrTbl_entry(ProcDirectory_t* prdir, Process_t* process)
{
  back:
    for(int i = 0; i<1024; i++)
    {
        if(prdir->procTables[i])
        {
            ProcTable_t* prtbl = (ProcTable_t*)prdir->procTables[i];
            for(int j = 0; j<1024; j++)
            {
                if(!prtbl->procEntrys[j])
                {
                    prtbl->procEntrys[j] = (uint32_t)process;
                    process->prdir_Entry = &prtbl->procEntrys[j];
                    return 1;
                }
            }
        }
    }
    PrTbl_create((uint32_t*)prdir);
    goto back;
    return 0;
}
