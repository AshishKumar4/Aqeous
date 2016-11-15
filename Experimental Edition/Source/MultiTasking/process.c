#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "phy_mm/mem.h"
#include "process.h"
#include "ProcManager/ProcManager.h"

uint32_t pidcounter = 1;

Process_t* create_process(char* name, uint32_t* code, uint32_t priority, Process_t* parent)  /// Create a New Task for a given Process
{
  Process_t* New_Proc = (Process_t*)mtalloc(1);
  
  PrTbl_entry(main_prdir, New_Proc);

  New_Proc->priority = priority;

  New_Proc->code = code;

  New_Proc->parent = parent;

  if(parent)
  {
    New_Proc->pgdir = parent->pgdir;
  }
  else
  {
    New_Proc->pgdir = (uint32_t)pgdir_maker();
    Kernel_Mapper((PageDirectory_t*)New_Proc->pgdir);
    map((uint32_t)New_Proc,4096,(PageDirectory_t*)New_Proc->pgdir);
  }

  strcpy(New_Proc->name,name);

  New_Proc->total_tasks = 0;

  New_Proc->processID = pidcounter;
  pidcounter++;

  processes++;

  return New_Proc;
}

void init_Processing()
{
  ProcDirectory_t* prdir = PrDir_create();
  main_prdir = prdir;
}
