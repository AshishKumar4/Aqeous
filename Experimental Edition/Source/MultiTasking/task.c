#include <stdio.h>
#include <string.h>
#include "task.h"
#include "timer.h"
#include "rand.h"

task_t* current_task,*lastMade_task;
// Some externs are needed to access members in paging.c...
extern pdirectory *main_dir;
extern pdirectory *_cur_directory;
extern int alloc_page (page_t* e);
extern uint32_t read_eip();
extern uint32_t read_esp();
extern uint32_t read_ebp();
extern uint32_t initial_eip;
extern uint32_t initial_esp;
extern uint32_t initial_ebp;
extern void perform_task_switch(uint32_t, uint32_t, uint32_t);
extern void task_helper(uint32_t);
extern void saveState();
extern void loadState();
extern void switchTask(Registers *from,Registers *to);


// The next available process ID.
uint32_t next_pid = 1;


void idle()
{
  printf("\nNew Task 1");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf(" ");
    asm volatile("sti");
  }
}

void idle2()
{
  printf("\nNew Task 2");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf("2");
    asm volatile("sti");
  }
}

void idle3()
{
  printf("\nNew Task 3");
  //scheduler();
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    printf("3");
    asm volatile("sti");
  }
  while(1)
  {
    asm volatile("cli");
    printf("--3--");
    asm volatile("sti");
  }
}

void idle4()
{
  printf("\nNew Task 4");
  //scheduler();
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    printf("4");
    asm volatile("sti");
  }
  while(1)
  {
    asm volatile("cli");
    printf("--4--");
    asm volatile("sti");
  }
}

void idle5()
{
  printf("\nNew Task 5");
  //scheduler();
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    printf("5");
    asm volatile("sti");
  }
  while(1)
  {
    asm volatile("cli");
    printf("--5--");
    asm volatile("sti");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
}

void idle6()
{
  printf("\nNew Task 6");
  //scheduler();
  uint32_t i=0;
  while(1)
  {
    asm volatile("cli");
    if(i>1024*100) break;
    i++;
    printf("6");
    asm volatile("sti");
  }
  while(1)
  {
    asm volatile("cli");
    printf("--6--");
    asm volatile("sti");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
}


void initTasking()
{
   StartTask=(task_t*)kmalloc(sizeof(task_t));
   lastMade_task=StartTask;
   current_task=StartTask;
   srandInit();
   createTask(StartTask,idle,"Idle System Task",0x202,3,main_dir);

   task_t* new_task=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task,idle2,"Another System Task",0x202,2,main_dir);

   task_t* new_task2=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task2,idle3,"another System Task",0x202,1,main_dir);

   task_t* new_task3=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task3,idle4,"another System Task",0x202,5,main_dir);

   task_t* new_task4=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task4,idle5,"another System Task",0x202,4,main_dir);

   task_t* last_task=(task_t*)kmalloc(sizeof(task_t));
   createTask(last_task,idle6,"another System Task",0x202,3,main_dir);
   //last_task->regs.esp=initial_esp;
   printf("\nMultiTasking Initialized");
   // Reenable interrupts.
   //exec();
}

uint32_t *stack;

void createTask(task_t* task,void (*func)(), char *name, uint32_t flags,uint32_t priority, pdirectory *pagedir)
{
  task_t* NewTask=task;
  NewTask->pdir = pagedir;
  uint32_t base; //temporary pointer - will be set to the kstacktop
                             //and afterwards saved in esp of proc structure
  NewTask->regs.esp=kmalloc_a(4096);  // ugh.. hope this works..
  memset((void*)NewTask->regs.esp,0,4096);
  stack=(uint32_t*)NewTask->regs.esp+4096;
  NewTask->StackTop=stack;
  base=(uint32_t)stack;
  NewTask->regs.ebp=base;
  *--stack = flags; // eflags
	*--stack = 0x8; // cs
	*--stack = (uint32_t)func; // eip
  uint32_t stacktop=stack;
	*--stack = 0; // eax
	*--stack = 0; // ecx
	*--stack = 0; // edx;
	*--stack = 0; //ebx
	*--stack = stacktop; //esp
	*--stack = base; //ebp
	*--stack = 0; //esi
	*--stack = 0; //edi
	*--stack = 0x10; // ds
	*--stack = 0x10; // fs
	*--stack = 0x10; // es
	*--stack = 0x10; // gs

  NewTask->regs.eflags = flags; // eflags
	NewTask->regs.cs = 0x8; // cs
	NewTask->regs.eip = (uint32_t)func; // eip
	NewTask->regs.eax = 0; // eax
	NewTask->regs.ebx = 0; // ebx
	NewTask->regs.ecx = 0; // ecx;
	NewTask->regs.edx = 0; //edx
	NewTask->regs.esi = 0; //esi
	NewTask->regs.edi = 0; //edi
	NewTask->ds = 0x10; // ds
	NewTask->fs = 0x10; // ds
	NewTask->es = 0x10; // ds
	NewTask->gs = 0x10; // ds

  NewTask->ss=0x10;
  NewTask->regs.esp = (uint32_t)stack; // Just initialize the stack :D

  NewTask->next = StartTask->next;

  NewTask->id=next_pid;
  NewTask->priority=priority;

  NewTask->name=name;

  lastMade_task->next=NewTask;
  lastMade_task=NewTask;

  next_pid++;
}

task_t *old_task;

void Scheduler_exec() //Enable Multitasking !!!
{
		timer_task=(uint32_t)scheduler;
    init_timer(20000);
    srandInit();
    printf("\n\nEntering Scheduling Mode!!!\nNEW EIP: %x\n",current_task->regs.eip);
    asm volatile("sti");
}

inline void scheduler()
{
    if(timer_tick)
    {
      --timer_tick;
      outb(0x20, 0x20); // send EoI to master PIC
      asm volatile("sti;\
        iret");
    }
    old_task=current_task;
    asm volatile("pop %%eax":"=a"(old_task->regs.eip));
    current_task=current_task->next;
    //current_task=(task_t*)tasks[(rn%(next_pid-2))+1];
    timer_tick=10;
    asm volatile("jmp *%0"::"r"((uint32_t)switch_task));
}

static void switch_task()
{/*
    asm volatile("movl %%ebx, %0;\
      movl %%ecx, %1;\
      movl %%edx, %2;\
      movl %%esi, %3;\
      movl %%edi, %4;\
      movl %%ebp, %5;":
      "=r"(old_task->regs.ebx),
      "=r"(old_task->regs.ecx),
      "=r"(old_task->regs.edx),
      "=r"(old_task->regs.esi),
      "=r"(old_task->regs.edi),
      "=r"(old_task->regs.ebp));
/*
    asm volatile("    movw    $16, %ax;\
      movw    %ax, %ds;\
      movw    %ax, %es;\
      movw    %ax, %fs;\
      movw    %ax, %gs;"); //16=kernel data segemnt

    asm volatile("movl %%ds, %0;\
      movl %%es, %1;\
      movl %%fs, %2;\
      movl %%gs, %3;\
      movl %%ss, %4;":"=r"(old_task->ds),
      "=r"(old_task->es),
      "=r"(old_task->fs),
      "=r"(old_task->gs),
      "=r"(old_task->ss));
*/
  //  asm volatile("movl %%esp, %0":"=r"(old_task->regs.esp));
    stack=(uint32_t*)current_task->StackTop;
    *--stack = current_task->regs.eflags; // eflags
    *--stack = current_task->regs.cs; // cs
    *--stack = current_task->regs.eip; // eip
    *--stack = current_task->regs.eax; // eax
    *--stack = current_task->regs.ecx; // ecx
    *--stack = current_task->regs.edx; // edx
    *--stack = current_task->regs.ebx; //ebx
    *--stack = current_task->StackTop-3; //original esp
    *--stack = current_task->regs.ebp; //ebp
    *--stack = current_task->regs.esi; //esi
    *--stack = current_task->regs.edi; //edi
    *--stack = current_task->ds; // ds
    *--stack = current_task->fs; // fs
    *--stack = current_task->es; // es
    *--stack = current_task->gs; // gs

    asm volatile("movl %0, %%esp;\
      movl %1, %%ss;": :"r"(current_task->regs.esp),"r"(current_task->ss));

    asm volatile("popl %gs;\
      popl %fs;\
      popl %es;\
      popl %ds;\
      popa");
    asm volatile("sti;");
    outb(0x20, 0x20); // send EoI to master PIC
    asm volatile("iret");
}

void rand_test()
{
  printf("\nTesting our Random Number Generator \n\tOutputing a Stream of random Numbers: \n");
  srandInit();
  for(int i=0;i<300;i++)
  {
    uint32_t rn=(rand()%(next_pid-1))+1;
    printf("%x ",rn);
  }
}
