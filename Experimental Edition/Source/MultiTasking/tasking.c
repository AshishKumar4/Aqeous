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
#include "MManager/mmanagerSys.h"

#include "cpu/cpu.h"
#include "Scheduler/Scheduler.h"

#include "RandomLib/random.h"
#include "math.h"

#include "NeuralNetwork/Neuron/NeuralProcessing.h"
#include "ProcManager/ProcManager.h"

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

void test_thread()
{
  while(1)
  {
    if(bbb)
    {
      asm volatile("cli");
      printf("\nThis works!");
      bbb = 0;
      uint32_t* tty = malloc(4096);
      for(int i = 0; i < 10; i++)
      {
        tty[i] = i*2;
      }
    //  Randomizer();
      for(int i = 0; i < 10; i++)
      {
        printf(" A%d", tty[i]);
      }//*/
    }
    asm volatile("int $50");
  }
}

void test_process()
{/*
  Randomizer();
  printf("Seed1: %x Seed2: %x Seed3: %x\n", seed1, seed2, seed3);

  int j;
  for(int i = 1; i < 1000; i++)
  {
    for(j = 0; random()%1000 != i; j++);
    printf("%d--", j);
  }*/
  int value = StrToInt(CSI_Read(1));
  int threshold = StrToInt(CSI_Read(2));

  printf("%d %d", (int)CheckNeuralSCE(value, threshold), (int)powf(CONST_E, value - threshold));

  if(!tttt)
  {
//    asm volatile("cli");
/*
    uint32_t* tm;
    for(int i = 0; i < 10; i++)
    {
      tm = pgdir_maker();
      Kernel_Mapper((PageDirectory_t*)tm);
    }*/
    test_proc = create_process("test_process", 0, 1, 0);
  //  memcpy(test_proc->pgdir, system_dir, 4096);
  //  pgdir_maker();
  /*
    test_proc->pgdir = tm;
    Kernel_Mapper((PageDirectory_t*)tm);
    map((uint32_t)test_proc,4096,(PageDirectory_t*)tm);*/

    test_task = create_task("test_task", test_thread, 10, 0x202, test_proc);
    Activate_task_direct(test_task);

    tttt = 1;
  }
  else
    bbb = 1;

  Shell_wakeup();
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

  Shell_task = create_task("Shell_task", Shell, 5, 0x202, Shell_proc);  //Main shell task.
  //Shell_task->special = 1;
  Activate_task_direct(Shell_task);//, &KitList[0]);

  Shell_Istream_task = create_task("Shell_Istream", Shell_Input, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Istream_task);//, &KitList[0]); //This would manage keyboard input and delivery to the required process.
  //Shell_Istream_task->special = 1;

  Idle_task = create_task("System_idle_task",idle, 0, 0x202, kernel_proc);  //default task, this dosent run
  Idle_task->special = 1;

  SAS_proc = create_process("SAS", 0, 1, kernel_proc); //Scheduler Assistance System process.
  SAS_init();

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
