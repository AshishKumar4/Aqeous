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
#include "common.h"

void idle()
{
  //scheduler();
  while(1)
  {
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
  uint32_t cd = Get_Scheduler()->curr_dir;
  asm volatile("cli");

  printf("Test Thread worked perfectly!!!");

  tttt = 0;
  kill();
  asm volatile("int $50");
  while(1);
}

void AP_c_Booter()
{
  asm volatile("cli");

  for(int i = CORES_BOOT_SINGLE_SQ; i < (total_CPU_Cores - 1); i++)
  {
    
  }
  kill();
  while(1);
}

DECLARE_LOCK(test);
#include "rand.h"
void test_process()
{
  LOCK(test);


  asm volatile("cli");

  if(!tttt)
  {
    test_proc = create_process("test_process", 0, 1, kernel_proc);

    test_task = create_task("test_task", test_thread, 10, 0x202, test_proc);
    Activate_task_direct(test_task);//, Get_Scheduler());
  //  _kill(test_task);

    tttt = 1;
  }
  else
  {
    printf("Starting...");
    bbb = 1;
  }
  kill();
  asm volatile("int $50");
  while(1);
}

extern void kernel_main();

void tvkc1()
{
  kill();
  asm volatile("int $51");
  while(1);
}

void tasking_initiator()
{
  UNLOCK(test);
  SchedulerKits_t* kit = Get_Scheduler();
  Kernel_task = create_task("Main_Kernel",kernel_main, 0, 0x202, kernel_proc);
  Kernel_task->special = 2;
  kit->current_task = (uint32_t)Kernel_task;

  printf("\n\n\n\t\t--------------MISSION ACCOMPLISHED--------------\n\n\t--------------Welcome to the MultiThreading World!!!--------------\n");
  printf("\n\t-----------Launching the Shell and input/output processes-----------\n\t\t\t\tStarting in 3...2...1... GO...\n\n");
  delay1(1);
  kb_io_init();
  init_shell();
  multitasking_ON = 1;

  setIRQMask(0);

  apic_start_timer(APIC_LOCAL_BASE);       //The respective Timer initialization function of the timer of choice
  //Here it goes, The entry to the multitasking world.

  for(uint32_t i = 0; i < total_CPU_Cores - 1; i++)
  {
    *(uint32_t*)(0x3000 + (i*0x2000) + AP_startup_Code_sz + 8) = 0x3240; //Notifies the cores that its time to start scheduling...
  }

  UNLOCK(TASK_LOCK_KILL);

  UNLOCK(TASK_LOCK_ATD);
  asm volatile("sti;");
  kill();
  while(1);
}

extern void kernel_main();

void init_multitasking()
{
  asm volatile("cli");

  init_Processing();

  kernel_proc = create_process("microkernel", 0, 1, 1);
  kernel_proc->pgdir = (uint32_t)system_dir;

  Init_Scheduler();

  printf("\nSchedulers Created Successfully");

  Setup_MMADS();

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Shell_Ostream_task = create_task("Shell_Ostream", Shell_Double_buffer, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task);//, &KitList[0]); //This is the task which would make printing to console possible!

  Shell_task = create_task("Shell_task", Shell, 15, 0x202, Shell_proc);  //Main shell task.
  //Shell_task->special = 1;
  Activate_task_direct(Shell_task);//, &KitList[0]);

  Shell_Istream_task = create_task("Shell_Istream", Shell_Input, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Istream_task);//, &KitList[0]); //This would manage keyboard input and delivery to the required process.
  //Shell_Istream_task->special = 1;

//  Idle_task = create_task("System_idle_task",idle, 0, 0x202, kernel_proc);  //default task, this dosent run
//  Idle_task->special = 1;

  SAS_proc = create_process("SAS", 0, 1, kernel_proc); //Scheduler Assistance System process.
  SAS_init();

  IPC_init();

  //kit->reached_bottom = 0;
  tasking_initiator();
  while(1); //Never comeback :D
}

void Spurious_task_func()
{
  while(1) //Dont worry; It would eventually sink to the bottom queue where it would be removed by some other task.
  {

    //if(Get_Scheduler()->identity != 0) printf(" C ");
    asm volatile("int $50");
  }
}
