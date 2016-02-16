#include <stdio.h>
#include <string.h>
#include "task.h"

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
    printf("11 ");
    asm volatile("sti");
  }
  scheduler();
  //while(1)
  {
    printf("12 ");
  }
  scheduler();
  //while(1)
  {
    printf("13 ");
  }
  scheduler();
  //while(1)
  {
    printf("14 ");
  }
  scheduler();
}

void idle2()
{
  printf("\nNew Task 2");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf("21 ");
    asm volatile("sti");
  }
  scheduler();
  //while(1)
  {
    printf("22 ");
  }
  scheduler();
  //while(1)
  {
    printf("23 ");
  }
  scheduler();
  //while(1)
  {
    printf("24 ");
  }
  scheduler();
}

void idle3()
{
  printf("\nNew Task 3");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf("31 ");
    asm volatile("sti");
  }
  scheduler();
  //while(1)
  {
    printf("32 ");
  }
  scheduler();
  //while(1)
  {
    printf("33 ");
  }
  scheduler();
  //while(1)
  {
    printf("34 ");
  }
  scheduler();
}

void idle4()
{
  printf("\nNew Task 4");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf("41 ");
    asm volatile("sti");
  }
  scheduler();
  //while(1)
  {
    printf("42 ");
  }
  scheduler();
  //while(1)
  {
    printf("43 ");
  }
  scheduler();
  //while(1)
  {
    printf("44 ");
  }
  scheduler();
}

void idle5()
{
  printf("\nNew Task 5");
  //scheduler();
  while(1)
  {
    asm volatile("cli");
    printf("51 ");
    asm volatile("sti");
  }
  scheduler();
  //while(1)
  {
    printf("52 ");
  }
  scheduler();
  //while(1)
  {
    printf("53 ");
  }
  scheduler();
  //while(1)
  {
    printf("54 ");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
}


void initTasking()
{
   StartTask=(task_t*)kmalloc(sizeof(task_t));
   lastMade_task=StartTask;
   current_task=StartTask;
   createTask(StartTask,idle,"Idle System Task",0x202,main_dir);

   task_t* new_task=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task,idle2,"Another System Task",0x202,main_dir);

   task_t* new_task2=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task2,idle3,"another System Task",0x202,main_dir);

   task_t* new_task3=(task_t*)kmalloc(sizeof(task_t));
   createTask(new_task3,idle4,"another System Task",0x202,main_dir);

   task_t* last_task=(task_t*)kmalloc(sizeof(task_t));
   createTask(last_task,idle5,"another System Task",0x202,main_dir);
   //last_task->regs.esp=initial_esp;
   printf("\nMultiTasking Initialized");
   // Reenable interrupts.
   //exec();
}

uint32_t *stack;

void createTask(task_t* task,void (*func)(), char *name, uint32_t flags, pdirectory *pagedir)
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
	*--stack = 0; // eax
	*--stack = 0; // ebx
	*--stack = 0; // ecx;
	*--stack = 0; //edx
	*--stack = 0; //esi
	*--stack = 0; //edi
	*--stack = base; //ebp
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
//  printf(" %x ",NewTask->regs.esp);
  NewTask->next = StartTask->next;
  NewTask->id=next_pid;
  NewTask->name=name;
  lastMade_task->next=NewTask;
  lastMade_task=NewTask;
  NewTask->used=0;
  next_pid++;
}

void Scheduler_exec()
{
    printf("\n\nEntering Scheduling Mode!!!\nNEW EIP: %x\n",current_task->regs.eip);
    asm volatile("sti");
}

task_t *old_task;

uint32_t eflags,cs,eip;

void scheduler()
{
  asm volatile("cli");
  old_task=current_task;
  current_task=current_task->next;
  asm volatile("pop %%eax":"=a"(old_task->regs.eip));

  asm volatile("movl %%ebx, %0;":"=r"(old_task->regs.ebx));
  asm volatile("movl %%ecx, %0;":"=r"(old_task->regs.ecx));
  asm volatile("movl %%edx, %0;":"=r"(old_task->regs.edx));
  asm volatile("movl %%esi, %0;":"=r"(old_task->regs.esi));
  asm volatile("movl %%edi, %0;":"=r"(old_task->regs.edi));
  asm volatile("movl %%ebp, %0;":"=r"(old_task->regs.ebp));
//
  asm volatile("pushl %eax");
  asm volatile("pushl %ebx");
  asm volatile("pushl %ecx");
  asm volatile("pushl %edx");
  asm volatile("pushl %esi");
  asm volatile("pushl %edi");
  asm volatile("pushl %ebp");
  asm volatile("pushl %ds");
  asm volatile("pushl %es");
  asm volatile("pushl %fs");
  asm volatile("pushl %gs");

  asm volatile("    movw    $16, %ax		"); // Kernel data segment (Ring 0)
  asm volatile("    movw    %ax, %ds		");
  asm volatile("    movw    %ax, %es		");
  asm volatile("    movw    %ax, %fs		");
  asm volatile("    movw    %ax, %gs		");

  asm volatile("movl %%ds, %0;":"=r"(old_task->ds));
  asm volatile("movl %%es, %0;":"=r"(old_task->es));
  asm volatile("movl %%fs, %0;":"=r"(old_task->fs));
  asm volatile("movl %%gs, %0;":"=r"(old_task->gs));
  asm volatile("movl %%ss, %0;":"=r"(old_task->ss));

  stack=(uint32_t*)current_task->StackTop;
  *--stack = current_task->regs.eflags; // eflags
  *--stack = current_task->regs.cs; // cs
  *--stack = current_task->regs.eip; // eip
  *--stack = current_task->regs.eax; // eax
  *--stack = current_task->regs.ebx; // ebx
  *--stack = current_task->regs.ecx; // ecx
  *--stack = current_task->regs.edx; //edx
  *--stack = current_task->regs.esi; //esi
  *--stack = current_task->regs.edi; //edi
  *--stack = current_task->regs.ebp; //ebp
  *--stack = current_task->ds; // ds
  *--stack = current_task->fs; // fs
  *--stack = current_task->es; // es
  *--stack = current_task->gs; // gs

  asm volatile("movl %%eax, %%esp;": :"a"(current_task->regs.esp));
  asm volatile("movl %%eax, %%ss;": :"a"(current_task->ss));

  asm volatile("popl %gs");
  asm volatile("popl %fs");
  asm volatile("popl %es");
  asm volatile("popl %ds");
  asm volatile("popl %ebp");
  asm volatile("popl %edi");
  asm volatile("popl %esi");
  asm volatile("popl %edx");
  asm volatile("popl %ecx");
  asm volatile("popl %ebx");
  asm volatile("popl %eax");
  asm volatile("out %%al, %%dx": :"d"(0x20), "a"(0x20)); // send EoI to master PIC
  asm volatile("sti");
  asm volatile("iret");
}
