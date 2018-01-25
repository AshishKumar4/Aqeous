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

Process_t* create_process(char* name, uint32_t privil, uint32_t priority, Process_t* parent)  /// Create a New Task for a given Process
{
  Process_t* New_Proc = (Process_t*)kmalloc(sizeof(Process_t));

  PrTbl_entry(main_prdir, New_Proc);

  New_Proc->priority = priority;

  //New_Proc->code = ;
  New_Proc->privileges = privil;

  if((int)parent > 1)
  {
    New_Proc->parent = parent;
    New_Proc->pgdir = parent->pgdir;
  }
  else if(!parent)
  {
    New_Proc->pgdir = (uint32_t)pgdir_maker();
    MapKernelPages((PageDirectory_t*)New_Proc->pgdir);
    Map_non_identity(phy_alloc4K(), 0xC0004000, 4096, (PageDirectory_t*)New_Proc->pgdir);
  }
  else New_Proc->parent = 0;

  strcpy(New_Proc->name,name);

  New_Proc->total_tasks = 0;

  New_Proc->processID = pidcounter;
  pidcounter++;

  processes++;

  return New_Proc;
}

void Delete_Process(Process_t* proc)
{
  asm volatile("cli");
  printf("\nDeleting Process %s", proc->name);
  // Free all frames allocated to the process 

  // Free all page tables, Page Directory
  PageDirectory_t* pgdir = (PageDirectory_t*)proc->pgdir;

  // Free all memory requested by the process 
  // Free all threads Made by the process 
  task_table_t* tmp = proc->first_task_entry, *tmp2;
  printf("\n{%d}", proc->total_tasks);
  for(uint32_t i = 0; i < proc->total_tasks; i++)
  {
    tmp2 = tmp->next;
    task_t* tt = (task_t*)tmp;
    tt->magic = 0x1111; // For notifying other processors that this is gonna be deleted
    printf("\n=>%s;", tt->name);
    if(tt->active == 0)
    {
      asm volatile("mov $0x4284, %eax; hlt");
    }
    _kill(tt);
    tmp = tmp2;
    pfree(tmp);
  }
  
  pfree((void*)(pgdir->table_entry[0] & 0xfffff000));
  *(proc->prdir_Entry) = 0;
  // Delete the page structures
  pfree(proc);
  asm volatile("sti");
} 

void init_Processing()
{
  ProcDirectory_t* prdir = PrDir_create();
  main_prdir = prdir;
}
