#ifndef PROCMANAGER_h
#define PROCMANAGER_h

#include "process.h"
#include "tasking.h"

typedef struct __attribute__((packed)) ProcTable
{
    uint32_t procEntrys[1024];
}ProcTable_t;

typedef struct __attribute__((packed)) ProcDirectory
{
    uint32_t procTables[1024];
}ProcDirectory_t;

ProcDirectory_t* main_prdir;

ProcDirectory_t* PrDir_create();

ProcTable_t* PrTbl_create(uint32_t* procdir);

int PrTbl_entry_i(uint32_t* proctbl, Process_t* process);

int PrTbl_entry(ProcDirectory_t* prdir, Process_t* process);

#endif
