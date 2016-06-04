#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"

uint32_t pidcounter = 1;

Process_t* create_process(char* name, uint32_t* code, uint32_t priority, Process_t* parent)  /// Create a New Task for a given Process
{
  Switch_to_system_dir();
  Process_t* New_Proc = (Process_t*)tmalloc(4096);
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
    Kernel_Mapper((pdirectory*)New_Proc->pgdir);
    map((uint32_t)New_Proc,4096,(pdirectory*)New_Proc->pgdir);
  }

  strcpy(New_Proc->name,name);

  New_Proc->total_tasks = 0;

  New_Proc->processID = pidcounter;
  pidcounter++;

  Switch_back_from_System();

  return New_Proc;
}
