#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "process.h"
#include "sys.h"
#include "Shell.h"
#include "vesa.h"
#include "kb_handle.h"
#include "stdlib.h"
#include "FS_Handling.h"
#include "apic.h"
#include "pic.h"
#include "hpet.h"
#include "MManager/mmanagerSys.h"

#include "cpu/cpu.h"
#include "Scheduler/Scheduler.h"

#include "RandomLib/random.h"
#include "math.h"

#include "NeuralNetwork/Neuron/NeuralProcessing.h"
#include "ProcManager/ProcManager.h"

#include "IPCInterface/IPCInterface.h"
#include "math.h"

#include "CancerTherapy/CancerCure.c"

#include "common.h"
#include "FileSystem/AqFS/AqfsTools.h"
#include "FileSystem/AqFS/fs_alloc.h"

#include "Processing/LibSymTable/LibSymTable.h"
#include "Processing/SysCalls/SysCalls.h"
#include "Processing/processing.h"
#include "ThreadTable.h"

void idle()
{
  //scheduler();
  while(1)
  {
    kill();
    asm volatile("int $50");
  }
}

inline int tf(uint32_t a, uint32_t b)
{
  return MAX(a,b)%MIN(a,b);
}

uint32_t tyt = 0;

Process_t* test_proc;
task_t* test_task;
int tttt = 0;
int bbb = 0;
int *bby = &bbb;

void test_thread()
{
  //uint32_t cd = Get_Scheduler()->current_pdir;
  /*
  while(1)
  {
  //  printf("\nAa");
    if(bbb)
    {
      bbb = 0;
    }
    asm volatile("int $50");
  }*/
  asm volatile("cli");
  bbb = 0;
  char c[10];
  c[0] = 'n';
  printf("\nProgram to convert a Number to the ratio of 2 Natural Numbers-->\n");
  while (c[0] != 'y')
  {
    int a = 1, b = 1;
    long double d, e = 2;
    char tmp[20];
    printf("\nEnter the Number: ");
    kb_getline(tmp, 20);
    d = StrToInt(tmp);
//    scanf("%d", (uint32_t)&d);
    printf("as%d", d);
    while (b != e)
    {
      a++;
      b = a*d;
      e = a*d;
    }
    printf("The value of Numerator: %x %s %x", b, "\nThe Value of Denominator: ", a);
    printf("\n\nDo you Want to quit now? (y/n) ");

    kb_getline(c, 10);
  }
  tttt = 0;
  kill();
  asm volatile("int $50");
  while(1);
}

void user_test()
{
 // asm volatile("hlt");
  asm volatile("int $56");
  while(1);
}

void kernel_test()
{
  while(1);
  printf("\nIt Works");
  asm volatile("hlt");
  Shell_Dbuff_sync();
  while(1);
}

void __attribute__((optimize("O0"))) test_process()
{/*
  printf("\nYOLO");
  asm volatile("int $55");
  while(1);*/
  uint32_t tl = PIT_Counter;
  for(int i = 1; i < 1000; i++)
  {
   // printf("\n[%d]", AqAlloc(i)->lower32);
   // sec_alloc(1);
    asm volatile("int $50");
    //kmalloc(2);
  }
  printf("\nPIT Counter: %d", PIT_Counter - tl);
  
  kill();
  asm volatile("int $50");
  while(1);//*/
}

extern void kernel_main();

void Spawner_Task()
{
  kill();
  while(1);
}

void tasking_initiator()
{
  CancerCure_init();

  SchedulerKits_t* kit = Get_Scheduler();
  Kernel_task = create_task("Main_Kernel",kernel_main, 0, 0x202, kernel_proc);
  Kernel_task->special = 2;
  kit->current_task = (uint32_t)Kernel_task;

  init_libsym();
  syscall_init();
  ThreadRipper_init();

  TemplatePgDir = pmem_4k(1);
  kernelPgDir_Builder(TemplatePgDir);

  printf("\n\n\n\t\t--------------MISSION ACCOMPLISHED--------------\n\n\t--------------Welcome to the MultiThreading World!!!--------------\n");
  printf("\n\t-----------Launching the Shell and input/output processes-----------\n\t\t\t\tStarting in 3...2...1... GO...\n\n");
/*
  for(int i = 0; i < total_CPU_Cores; i++)
  {
    printf("\n\t{scheduler: %d}, {dispatcher: %d", KitList[i].scheduler, KitList[i].switcher);
  }*/

    
  kb_io_init();
  init_shell();
  multitasking_ON = 1;

  PIT_Task = Shell_Dbuff_sync;

  //Here it goes, The entry to the multitasking world.

  for(uint32_t i = 0; i < total_CPU_Cores - 1; i++)
  {
    KitList[i+1].current_task = (uint32_t)create_task("System_Spawner", Spawner_Task, 0, 0x202, kernel_proc);
    Activate_task_strict_SP((task_t*)(KitList[i+1].current_task), &KitList[i+1]);
    //printf("\nCore %d Initialized", i);
    *(uint32_t*)(0x3000 + (i*0x2000) + AP_startup_Code_sz + 8) = 0x3240; //Notifies the cores that its time to start scheduling...
  }

  KitList[0].current_task = (uint32_t)create_task("System_Spawner", Spawner_Task, 0, 0x202, kernel_proc);
  Activate_task_strict_SP((task_t*)(KitList[0].current_task), &KitList[0]);

  UNLOCK(TASK_LOCK_KILL);
  UNLOCK(TASK_LOCK_ATD);
  
  clearIRQMask(0);
  clearIRQMask(1);
  apic_start_timer(APIC_LOCAL_BASE);       //The respective Timer initialization function of the timer of choice
  
  asm volatile("sti;");
  kill();
}

void init_multitasking()
{

  init_Processing();  

  kernel_proc = create_process("microkernel", 0, 1, (Process_t*)1);
  kernel_proc->pgdir = (uint32_t)system_dir;

  Init_Scheduler();
  ThreadTable_init();

  printf("\nSchedulers Created Successfully");

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Screen_BuffSync = Shell_Dbuff_sync;
 /* Shell_Ostream_task = create_task("Shell_Ostream", Shell_Double_buffer, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task);//, &KitList[0]); //*///This is the task which would make printing to console possible!

  Shell_task = create_task("Shell_task", Shell_Alternate, 20, 0x202, Shell_proc);  //Main shell task.
  //Shell_task->special = 1;
  Activate_task_direct(Shell_task);//, &KitList[0]);

  Shell_Istream_task = create_task("Shell_Istream", Shell_Input, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Istream_task);//, &KitList[0]); //This would manage keyboard input and delivery to the required process.

  Shell_Ostream_task = create_task("Shell_Ostream", Shell_Ostream, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task);//, &KitList[0]); //This would manage keyboard input and delivery to the required process.//*/
  //Shell_Istream_task->special = 1;
  printf("\n<-------------------->");
  Idle_task = create_task("System_idle_task",idle, 0, 0x202, kernel_proc);  //default task, this dosent run
  Idle_task->special = 1;
  ThreadTable_SetEntry(Idle_task);
  

  SAS_proc = create_process("SAS", 0, 1, kernel_proc); //Scheduler Assistance System process.

  printf("\nInitializing SAS...");
  SAS_init();
  printf("\nSAS Initialized Successfully");

  IPC_init();
  Setup_MMADS();
  tasking_initiator();
  while(1); //Never comeback :D
}

void Spurious_task_func_t()
{
  while(1) //Dont worry; It would eventually sink to the bottom queue where it would be removed by some other task.
  {
    //if(Get_Scheduler()->identity != 0) printf(" C ");
    asm volatile("int $51");
  }
}

void Spurious_task_func_end_t()
{
  return;
}
