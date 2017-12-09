#include "tasking.h"
#include "task.h"
#include "ProcManager/ProcManager.h"
#include "Scheduler/Scheduler.h"
#include "ProcManagement.h"

void Command_proc()
{
  char* list_option = (char*)CSI_ReadAPS("list");
  if(!list_option) list_option = (char*)CSI_ReadAPS("-l");

  uint32_t mmtog = StrToInt(CSI_ReadAPS("-m"));

  Process_t* entry;
  if(list_option)
  {

    ProcDirectory_t* dr = main_prdir;
    ProcTable_t* tbl;
    int c = 0;
    if(!strcmp(list_option, "all"))
    {
      for(int i = 0; i<1024; i++)
      {
          if(dr->procTables[i])
          {
              tbl = (ProcTable_t*)dr->procTables[i];
              for(int j = 0; j<1024; j++)
              {
                  if(tbl->procEntrys[j])
                  {
                    entry = (Process_t*)tbl->procEntrys[j];
                    printf("\n%x\t%s\t%d\tthreads: %x\t",j,entry->name, entry->processID, entry->total_tasks);
                    ++c;
                    if(c == (int)processes) goto out;
                  }
              }
          }
      }
    }
    else
    {
      for(int i = 0; i<1024; i++)
      {
          if(dr->procTables[i])
          {
              tbl = (ProcTable_t*)dr->procTables[i];
              for(int j = 0; j<1024; j++)
              {
                  if(tbl->procEntrys[j])
                  {
                    entry = tbl->procEntrys[j];
                    if(!strcmp(list_option, entry->name))
                    {
                      printf("\n%s\t%d\tthreads: %x\t\n\nChild Threads-->",entry->name, entry->processID, entry->total_tasks);
                      task_table_t* tt = entry->first_task_entry;
                      task_t* t;
                      for(int j = 0; j < entry->total_tasks; j++ )
                      {
                        t = &tt->task;
                        printf("\n\t%s\t%d\tpriority: %d\tcore: %d; {%x}", t->name, t->task_id, t->priority, (((uint32_t*)t->Scheduler - MotherSpace)*4)/(sizeof(SchedulerKits_t)), t->func);
                        if(mmtog)
                        {
                          printf(" {%x}", t->func);
                        }
                        tt = tt->next;
                      }
                      goto out;
                    }

                    ++c;
                    if(c == processes) goto out;
                  }
              }
          }
      }
      out:
      tbl = 0;
    }
  }

  char* o_option = (char*)CSI_ReadAPS("-o");
  if(!strcmp(o_option, "start"))
  {

  }
  else if(!strcmp(o_option, "stop"))
  {

  }
  else if(!strcmp(o_option, "t"))
  {
    printf("\nEnter the task id: ");
    int id;
    //scanf("%d", &id);
    char* st = kmalloc(8);
    kb_getline(st, 8);
    id = StrToInt(st);
    if(entry)
    {
      task_table_t* tt = entry->first_task_entry;
      task_t* t;
      for(int j = 0; j < (int)(entry->total_tasks); j++ )
      {
        t = &tt->task;
        if(id == (int)(t->task_id))
        {
          int op;
          printf("\nEnter the option:\n1. KILL\n2. SLEEP\n3. WAKE UP\n:");
          //scanf("%d", op);
          st = kmalloc(8);
    			kb_getline(st, 8);
    			op = StrToInt(st);
          if(op == 1)
          {
            _kill(t);
          }
          else if(op == 2)
          {
            Task_sleep(t);
          }
          else if(op == 3)
          {
            Task_wakeup(t);
          }
          break;
        }
        tt = tt->next;
      }

    }
  }
}
