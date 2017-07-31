#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "tasking.h"
#include "task.h"
#include "Scheduler/Scheduler.h"
#include "process.h"

void __attribute__((optimize("O0"))) IPC_init()
{
  SchedulerKits_t* tmp = (SchedulerKits_t*)MotherSpace;
  IPCRecord_t* tr;
  for(int i = 0; i < (int)total_CPU_Cores-1; i++)
  {
    tr = kmalloc(sizeof(IPCRecord_t));
    tmp->IPCRecord_addr = (uintptr_t)tr;
    tr->entries = 0;
    tr->start = tr->last = NULL;

    uint32_t sizeof_IPC = IPCprocessorP_end_t - IPCprocessorP_t;
    tr->IPCprocessorP = kmalloc(sizeof_IPC);

    memcpy(tr->IPCprocessorP, IPCprocessorP_t, sizeof_IPC);

    ByteSequence_Replace(0x4284A291, 4, (uint32_t)tr, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A292, 4, (uint32_t)tmp, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A293, 4, (uint32_t)&Priority_promoter, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A294, 4, (uint32_t)&Activate_task_direct_SP, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A290, 4, (uint32_t)&printf, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A295, 4, (uint32_t)&Task_sleep, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A296, 4, (uint32_t)&Task_wakeup, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A297, 4, (uint32_t)&Priority_changer, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A298, 4, (uint32_t)&Task_Refresh, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A299, 4, (uint32_t)&_kill, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));
    ByteSequence_Replace(0x4284A2A0, 4, (uint32_t)&Task_Swap, 4, (uint32_t*)tr->IPCprocessorP, (uint32_t*)((uint32_t)tr->IPCprocessorP + sizeof_IPC));


    tr->IPCtask = create_task("IPCprocessorP", tr->IPCprocessorP, 20, 0x202, kernel_proc);

    Activate_task_direct_SP(tr->IPCtask, tmp);

    ++tmp;

  }
}

void __attribute__((optimize("O0"))) IPCdispatch(IPCPacket_t* pp)
{
  IPCRecord_t* tr = pp->destination->IPCRecord_addr;
  while(tr->lock);
  tr->lock = 1;
  if(tr->entries == 0)
  {
    tr->start = pp;
  }
  else
    tr->last->next = pp;
  tr->last = pp;
  pp->next = NULL;
    //printf("-]>>%d %d %d %d\n",pp->type, pp->no_opt, pp->options, tr->entries);
  ++tr->entries;

//  Task_wakeup_direct(tr->IPCtask);

  //Priority_promoter(tr->IPCtask);
  tr->lock = 0;
}

void __attribute__((optimize("O0"))) IPCprocessorP_t()
{
  IPCRecord_t* tr = (IPCRecord_t*)(uint32_t)(0x4284A291);
  SchedulerKits_t* kit = (SchedulerKits_t*)(uint32_t)(0x4284A292);
  func_t p = (func_t)(0x4284A293);
  func_t atds = (func_t)(0x4284A294);
  func_t pf = (func_t)(0x4284A290);
  func_t ts = (func_t)(0x4284A295);
  func_t tw = (func_t)(0x4284A296);
  func_t pc = (func_t)(0x4284A297);
  func_t r = (func_t)(0x4284A298);
  func_t k = (func_t)(0x4284A299);
  func_t tsw = (func_t)(0x4284A2A0);
  while(1)
  {
    asm volatile("cli");
//    pf("--%d--", kit->identity);

    if(tr->entries)
    {

    //  pf("asd");

      IPCPacket_t* pp = tr->start;
      while(tr->lock) asm volatile("int $50");
      tr->lock = 1;
    //  pf("A2a  %d %d %s\n", tr->entries, pp->type, pp->task->name);
      switch(pp->type)
      {
        case TASK_PRIORITY_REFRESH:
          p(pp->task);
      //    pf("Abbbdddttggggg\n");
          break;

        case TASK_ACTIVATE:
          atds(pp->task, kit);
        //  pf("asas\n");
          break;

        case TASK_SLEEP:
          ts(pp->task);
          break;

        case TASK_WAKEUP:
          tw(pp->task);
          break;

        case TASK_PRIORITY_CHANGE:
          pc(pp->task, pp->options[0]);
          break;

        case TASK_REFRESH:
          r(pp->task, (func_t)pp->options[0]);
        //  pf("Abbbdddttggggg\n");
          break;

        case TASK_KILL:
          k(pp->task);
          break;

        case TASK_SWAP:
        //pf("asda");
          tsw(pp->task, (task_t*)pp->options[0]);
          break;

        default:;
          //pf("->%d %d %d ",pp->type, pp->no_opt, pp->options);
      }

      //

      tr->start = pp->next;
      //kfree(pp);
      --tr->entries;
      pp->done = 1;
    }
    else if (multitasking_ON)
    {
      //Task_sleep(tr->IPCtask);
    }
    tr->lock = 0;
    asm volatile("int $50");
  }
}

void IPCprocessorP_end_t()
{
  return;
}
