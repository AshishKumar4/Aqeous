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

void test_process()
{
//  Switch_to_system_dir();
  Shell_sleep();/*

  printf("\nEnter 1st Number: ");
  uint32_t a = 0;
  uint32_t* test_str = (uint32_t*)malloc(10);
  kb_getline((char*)test_str, 20);
  a = StrToInt((char*)test_str);

  printf("\nEnter 2nd Number: ");
  uint32_t b = 0;
  test_str = (uint32_t*)malloc(10);
  kb_getline((char*)test_str, 20);
  b = StrToInt((char*)test_str);
*/
  /* MY CODE */
  Shell_wakeup();
  asm volatile("cli");
//  kfree(test_str);
  kill();
  while(1);
}

extern void kernel_main();

void tasking_initiator()
{
  Kernel_task = create_task("Main_Kernel",kernel_main, 0, 0x202, kernel_proc);
  Kernel_task->special = 2;
  current_task = (uint32_t)Kernel_task;
  printf("\n\n\n\t\t--------------MISSION ACCOMPLISHED--------------\n\n\t--------------Welcome to the MultiThreading World!!!--------------\n");
  printf("\n\t-----------Launching the Shell and input/output processes-----------\n\t\t\t\tStarting in 3...2...1... GO...\n\n");
  delay1(1);
  kb_io_init();
  init_shell();
  multitasking_ON = 1;
  //setVesa(0x110); //TEXT MODE VESA :v
  //init_hpet();
  //apic_start_timer();       //The respective Timer initialization function of the timer of choice

  setIRQMask(0);
//  init_timer(1000);
  //Here it goes, The entry to the multitasking world.
  //enable_pic();
  //disable_pic();
  asm volatile("sti;\
   int $50");
  kill();
  while(1);
}

extern void kernel_main();

void init_multitasking()
{
  asm volatile("cli");

  kernel_proc = create_process("microkernel", 0, 1, 0);
  kernel_proc->pgdir = (uint32_t)system_dir;

  new_process = (uint32_t)kernel_proc;

  current_task = (uint32_t)create_task("initiating_task",tasking_initiator, 0, 0x202, kernel_proc);  //Scheduler initalization task
  old_task = current_task;

  Spurious_task = create_task("Spurious_task", Spurious_task_func, 0, 0x202, kernel_proc);
  Spurious_task->special = 1;

  Idle_task = create_task("System_idle_task",idle, 0, 0x202, kernel_proc);  //default task, this dosent run
  Idle_task->special = 1;

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Shell_Ostream_task = create_task("Shell_Ostream", Shell_Double_buffer, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task); //This is the task which would make printing to console possible!
  Shell_Istream_task = create_task("Shell_Istream", Shell_Input, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Istream_task); //This would manage keyboard input and delivery to the required process.
  //Shell_Istream_task->special = 1;
  Shell_task = create_task("Shell_task", Shell, 5, 0x202, Shell_proc);  //Main shell task.
  //Shell_task->special = 1;
  Activate_task_direct(Shell_task);

  SAS_proc = create_process("SAS", 0, 1, kernel_proc); //Scheduler Assistance System process.
  Activate_task_direct(create_task("SAS_init", SAS_init, 1, 0x202, SAS_proc)); //Initialization of SAS system.

  reached_bottom = 0;
  Scheduler_init(); // Let the FUN Begin :D Lets Switch from the old monotasking world to Multitasking World :D defined in tasking.asm
  while(1); //Never comeback :D
}

void Spurious_task_func()
{
  while(1) //Dont worry; It would eventually sink to the bottom queue where it would be removed by some other task.
  {
    asm volatile("int $50");
  }
}
