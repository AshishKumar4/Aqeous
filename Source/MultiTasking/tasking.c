#include "tasking.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "vmem.h"
#include "paging.h"
#include "process.h"

void idle()
{
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf(" 1 ");
    asm volatile("sti");
  }
}

void idle2()
{
  //scheduler();
  /*uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    ++i;
    if(i%2)
      printf("2");
    asm volatile("sti");
  }
  i=0;
  while(1)
  {
    asm volatile("cli");
    if(i%2)
      printf("--2%x--",i);
    ++i;
    asm volatile("sti");
  }//*/
  while(1)
  {
    asm volatile("cli");
    printf(" 2 ");
    asm volatile("sti");
  }
}

void idle3()
{
  //scheduler();
  /*uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      printf("3");
    asm volatile("sti");
  }
  i=0;
  while(1)
  {
    asm volatile("cli");
    if(i%2)
      printf("--3%x--",i);
    ++i;
    asm volatile("sti");
  }//*/
  while(1)
  {
    asm volatile("cli");
    printf(" 3 ");
    asm volatile("sti");
  }
}

void idle4()
{
  //scheduler();
  /*
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      printf("4");
    asm volatile("sti");
  }
  i=0;
  while(1)
  {
    asm volatile("cli");
    if(i%2)
      printf("--4%x--",i);
    i++;
    asm volatile("sti");
  }//*/
  while(1)
  {
    asm volatile("cli");
    printf(" 4 ");
    asm volatile("sti");
  }
}

void idle5()
{
  //asm volatile("cli");
  //while(1);
  //scheduler();
/*
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      printf("5");
    asm volatile("sti");
  }
  i=0;
  while(1)
  {
    asm volatile("cli");
    if(i%2)
      printf("--5%x--",i);
    i++;
    asm volatile("sti");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");//*/
  while(1)
  {
    asm volatile("cli");
    printf(" 5 ");
    asm volatile("sti");
  }
}

void idle6()
{
  //asm volatile("cli");
  //while(1);
  //scheduler();
/*
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    if(i%2)
      printf("6");
    asm volatile("sti");
  }
  i=0;
  while(1)
  {
    asm volatile("cli");
    if(i%2)
      printf("--6%x--",i);
    i++;
    asm volatile("sti");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
*/
  while(1)
  {
    asm volatile("cli");
    printf(" 6 ");
    asm volatile("sti");
  }
}

void tasking_initiator()
{
  current_task = Idle_task;
  apic_start_timer();       //The respective Timer initialization function of the timer of choice
  asm volatile("sti");
  while(1);
}

void init_multitasking()
{
  memset((void*)(200*1024*1024),0,(40*1024*1024));
  Process_t* kernel_proc = create_process("microkernel", 0, 1, 0);
  kernel_proc->pgdir = (uint32_t)system_dir;
  old_process = (uint32_t)kernel_proc;
  new_process = (uint32_t)kernel_proc;
  current_task = create_task("initiating_task",tasking_initiator, 20, 0x202, kernel_proc);  //Scheduler initalization task
  old_task = current_task;
  Idle_task = create_task("System_idle_task",idle, 11, 0x202, kernel_proc);  //default task
  Activate_task_direct(Idle_task);

  Activate_task_direct(create_task("idle2",idle2, 10, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle3",idle3, 9, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle4",idle4, 5, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle5",idle5, 4, 0x202, kernel_proc));
  Activate_task_direct(create_task("idle6",idle6, 3, 0x202, kernel_proc));

  Scheduler_init(); // Let the FUN Begin :D Lets Switch from the old monotasking world to Multitasking World :D defined in tasking.asm
}
