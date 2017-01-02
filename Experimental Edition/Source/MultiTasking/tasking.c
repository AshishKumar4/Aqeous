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

uint32_t x=1,y=2;

void __attribute__((optimize("O0"))) bxd()
{
  //printf("IT WORKS");
  x=4284;
  y=1;
  //return;
}


void a()
{
  *(uint32_t*)(0x42849999) = 4284;
  uint32_t bcd = aad;
  func_t pf = 0x99997777;
  pf("%x",bcd);
  return;
}

void bxend()
{
  return;
}

uint32_t tyt = 0;

void test_process()
{
//  Switch_to_system_dir();

  printf("\n%x\n%x", Get_Scheduler()->identity, adeg);
  Shell_wakeup();
  asm volatile("cli");
//  kfree(test_str);
  kill();
  while(1);
}

extern void kernel_main();

void __attribute__((optimize("O0"))) tbcd1()
{
  //AP_init_LAPIC();
  //printf("\nasd");
  //printf("%x", Get_Scheduler()->identity);
  //asm volatile("hlt");
  while(1)
  {
  //  delay(1);
  //  if(adeg > 600) break;
  //  printf("A%x %x-",Get_Scheduler()->identity);
  //  ++Get_Scheduler()->tasks;
    ++adeg;
  //  printf("A1 ");
    asm volatile("int $50");
  }
  asm volatile("hlt":::"memory");
  while(1);
}
void __attribute__((optimize("O0"))) tbcd2()
{
  //AP_init_LAPIC();
  //printf("\nasd");
  //printf("%x", Get_Scheduler()->identity);
  //asm volatile("hlt");
  while(1)
  {
  //  delay(1);
  //  if(adeg > 600) break;
  //  printf("A%x %x-",Get_Scheduler()->identity);
  //  ++Get_Scheduler()->tasks;
    ++adeg;
  //  printf("A2 ");
    asm volatile("int $50");
  }
  asm volatile("hlt":::"memory");
  while(1);
}
void __attribute__((optimize("O0"))) tbcd3()
{
  //AP_init_LAPIC();
  //printf("\nasd");
  //printf("%x", Get_Scheduler()->identity);
  //asm volatile("hlt");
  while(1)
  {
  //  delay(1);
  //  if(adeg > 600) break;
  //  printf("A%x %x-",Get_Scheduler()->identity);
  //  ++Get_Scheduler()->tasks;
    ++adeg;
  //  printf("A3 ");
    //Shell_Double_buffer();
    asm volatile("int $50");
  }
  asm volatile("hlt":::"memory");
  while(1);
}

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

  for(int i = 0; i < total_CPU_Cores - 1; i++)
  {
    *(uint32_t*)(0x3000 + (i*0x2000) + AP_startup_Code_sz + 8) = 0x3240;
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

  kernel_proc = create_process("microkernel", 0, 1, 0);
  kernel_proc->pgdir = (uint32_t)system_dir;

  Init_Scheduler();

  printf("\nSchedulers Created Successfully");

  Setup_MMADS();
  SchedulerKits_t* kit = Get_Scheduler();

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Shell_Ostream_task = create_task("Shell_Ostream", Shell_Double_buffer, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task);//, &KitList[0]); //This is the task which would make printing to console possible!
  Shell_Istream_task = create_task("Shell_Istream", Shell_Input, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Istream_task);//, &KitList[0]); //This would manage keyboard input and delivery to the required process.
  //Shell_Istream_task->special = 1;
  Shell_task = create_task("Shell_task", Shell, 5, 0x202, Shell_proc);  //Main shell task.
  //Shell_task->special = 1;
  Activate_task_direct(Shell_task);//, &KitList[0]);

  for(int i = 1; i < total_CPU_Cores - 1; i++ )
  {
    Activate_task_direct(create_task("Idle", tvkc1, 20, 0x202, kernel_proc));//, &KitList[i]);
    Idle_task = create_task("System_idle_task1",tbcd1, 20, 0x202, kernel_proc);  //default task, this dosent run
  //  Idle_task->special = 1;
    Activate_task_direct(Idle_task);//, &KitList[i]);
    Idle_task = create_task("System_idle_task2",tbcd2, 20, 0x202, kernel_proc);  //default task, this dosent run
    Idle_task->special = 1;
    Activate_task_direct(Idle_task);//, &KitList[i]);
    Idle_task = create_task("System_idle_task3",tbcd3, 0, 0x202, kernel_proc);  //default task, this dosent run
    Idle_task->special = 1;
    Activate_task_direct(Idle_task);//, &KitList[i]);

  //  Activate_task_direct_SP(create_task("Shell_Ostream", Shell_Double_buffer, 10, 0x202, Shell_proc), &KitList[i]); //This is the task which would make printing to console possible!
  //  Activate_task_direct_SP(Idle_task, &KitList[i]);
  }

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
