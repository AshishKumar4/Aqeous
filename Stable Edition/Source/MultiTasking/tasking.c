#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"
#include "sys.h"
#include "Shell.h"
#include "vesa.h"
#include "kb_queue.h"

void idle()
{
  //scheduler();
  while(1)
  {
  }
}

void idle2()
{
  //scheduler();
  /**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*50) break;
    ++i;
    if(i%2)
      _printf("2");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--2%x--",i);
    ++i;
    //asm volatile("sti");
  }
  //kill();
  /**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 2-x-");
    asm volatile("sti");
  }//*/
}

void idle3()
{
  //scheduler();
  /**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*30) break;
    i++;
    if(i%2)
      _printf("3");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--3%x--",i);
    ++i;
    //asm volatile("sti");
  }/**/
  while(1)
  {
    //asm volatile("cli");
    _printf(" 3-x-");
    //asm volatile("sti");
  }//*/
}

void idle4()
{
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*10) break;
    i++;
    if(i%2)
      _printf("4");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--4%x--",i);
    i++;
    //asm volatile("sti");
  }/**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 4-x-");
    asm volatile("sti");
  }//*/
}

void idle5()
{
  //asm volatile("cli");
  //while(1);
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      _printf("5");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
  //  kill();
    //asm volatile("cli");
    if(i%2)
      _printf("--5%x--",i);
    i++;
    //asm volatile("sti");
  }
  _printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
  /**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 5-x-");
    asm volatile("sti");
  }//*/
}

void idle6()
{
  //asm volatile("cli");
  //int ij=0;
  //while(1)
  //ij++;
  //scheduler();
/**/
  uint32_t i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i>1024*70) break;
    i++;
    if(i%2)
      _printf("6");
    //asm volatile("sti");
  }
  i=0;
  while(1)
  {
    //asm volatile("cli");
    if(i%2)
      _printf("--6%x--",i);
    i++;
    //asm volatile("sti");
  }
  _printf("\neverything worked fine :D MULTITASKING WORKS :D \n");

/**/
  while(1)
  {
    asm volatile("cli");
    _printf(" 6-x-");
    asm volatile("sti");
  }//*/
}

void test_process()
{
  Shell_sleep();
  _printf("\nThis is a test process to test the capabilities of the this New System.\nI am now Gonna get some input from you.");
  _printf("\nType your name below to check if the system and the keyboard drivers are working.\n-->");
  asm volatile("cli");
  char* test_str = kmalloc(100);
  asm volatile("sti");
  kb_getline(test_str, 10);
  _printf("\nYou entered: %s\n", test_str);
  Shell_wakeup();
//  while(1);
  kill();
}

void tasking_initiator()
{
  current_task = (uint32_t)Idle_task;
  printf("\n\n\n\t\t--------------MISSION ACCOMPLISHED--------------\n\n\t--------------Welcome to the MultiThreading World!!!--------------\n");
  printf("\n\t-----------Launching the Shell and input/output processes-----------\n\t\t\t\tStarting in 3...2...1... GO...\n\n");
  delay1(30);
  init_shell();
  kb_io_init();
  //setVesa(0x110); //TEXT MODE VESA :v
  //init_hpet();
  //apic_start_timer();       //The respective Timer initialization function of the timer of choice

  clearIRQMask(0);
  clearIRQMask(1);
  init_timer(30000);
  multitasking_ON = true;
  //Here it goes, The entry to the multitasking world.
  asm volatile("sti");
  while(1);
}

extern void kernel_main();

void init_multitasking()
{
  asm volatile("cli");
  kernel_proc = create_process("microkernel", 0, 1, 0);
  kernel_proc->pgdir = system_dir;

  new_process = (uint32_t)kernel_proc;

  current_task = (uint32_t)create_task("initiating_task",tasking_initiator, 15, 0x202, kernel_proc);  //Scheduler initalization task
  old_task = current_task;

  Spurious_task = create_task("Spurious_task", Spurious_task_func, 10, 0x202, kernel_proc);

  Idle_task = create_task("System_idle_task",idle, 15, 0x202, kernel_proc);  //default task, this dosent run

  Activate_task_direct(create_task("Main_Kernel",kernel_main, 10, 0x202, kernel_proc));
//  Activate_task_direct(create_task("idle2_test",test_task, 10, 0x202, kernel_proc));

  Shell_proc = create_process("Shell", 0, 1, kernel_proc);
  Activate_task_direct(create_task("Shell_Ostream", Console_Writer, 10, 0x202, Shell_proc)); //This is the task which would make printing to console possible!
  Activate_task_direct(create_task("Shell_Istream", Shell_Input, 10, 0x202, Shell_proc));
  Shell_task = create_task("Shell_task", Shell, 10, 0x202, Shell_proc);
  Activate_task_direct(Shell_task);

  reached_bottom = 0;
  Scheduler_init(); // Let the FUN Begin :D Lets Switch from the old monotasking world to Multitasking World :D defined in tasking.asm
  while(1); //Never comeback :D
}

void Spurious_task_func()
{
    //TODO: REMOVE ITSELF FROM SCHEDULING LOOP!!!
    kill();
  //  while(1);
}
