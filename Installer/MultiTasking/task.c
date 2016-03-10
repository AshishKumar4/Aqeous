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
  i=0;
  while(1)
  {
    asm volatile("cli");
    printf("--3%x--",i);
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
  i=0;
  while(1)
  {
    asm volatile("cli");
    printf("--4%x--",i);
    i++;
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
  i=0;
  while(1)
  {
    asm volatile("cli");
    printf("--5%x--",i);
    i++;
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
  i=0;
  while(1)
  {
    asm volatile("cli");
    printf("--6%x--",i);
    i++;
    asm volatile("sti");
  }
  printf("\neverything worked fine :D MULTITASKING WORKS :D \n");
}

extern void kernel_main();

void initTasking()
{
   StartTask=(task_t*)kmalloc(sizeof(task_t));
   lastMade_task=StartTask;
   current_task=StartTask;
   queue_t* q;
   for(int i=9;i>-1;i--)
   {
     queue[i]=(uint32_t)1024*1024*(i+90); //90mb to 100mb
     last_element[i]=queue[i]+sizeof(queue_t);
     timeslice[i]=100/(1+i);
     q=(queue_t*)queue[i];
     q->next=(queue_t*)last_element[i];
     q->istask=0;
     q->task=StartTask;
     ++q;
     if(i>0)
      q->next=(queue_t*)(1024*1024*(90+i-1));
     q->istask=0;
   }
   q=(queue_t*)queue[0];
   q=(queue_t*)queue[9];

   createTask(StartTask,idle,"Idle System Task",0x202,10,main_dir);
   q=(queue_t*)queue[9];
   ++q;
   q->istask=0;

   task_t* new_task=tmalloc(sizeof(task_t));
   createTask(new_task,idle2,"Another System Task",0x202,9,main_dir);

   task_t* new_task2=tmalloc(sizeof(task_t));
   createTask(new_task2,idle3,"another System Task",0x202,3,main_dir);

   task_t* new_task3=tmalloc(sizeof(task_t));
   createTask(new_task3,idle4,"another System Task",0x202,8,main_dir);

   task_t* new_task4=tmalloc(sizeof(task_t));
   createTask(new_task4,idle5,"another System Task",0x202,2,main_dir);

   task_t* last_task=tmalloc(sizeof(task_t));
   createTask(last_task,idle6,"another System Task",0x202,5,main_dir);/*/

	 task_t* The_Kernel=tmalloc(sizeof(task_t));
	 createTask(The_Kernel,kernel_main,"Kernel",0x202,1,main_dir);//*/
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
  NewTask->esp=smalloc(4096);  // ugh.. hope this works..
  memset((void*)NewTask->esp,0,4096);
  stack=(uint32_t*)NewTask->esp+4096;

  uint32_t base=(uint32_t)stack;
  *--stack = 0; // eax
  *--stack = 0; // ecx
  *--stack = 0; // edx;
  *--stack = 0; //ebx
  *--stack = base; //esp
  *--stack = base; //ebp
  *--stack = 0; //esi
  *--stack = 0; //edi
//  *--stack = 0x10; // eax


  NewTask->eflags = flags; // eflags
	NewTask->cs = 0x8; // cs
	NewTask->eip = (uint32_t)func; // eip

  NewTask->segment=0x10; //kernel

  NewTask->esp = (uint32_t)stack; // Just initialize the stack :D
  queue_t* q=(queue_t*)last_element[priority-1];
  last_element[priority-1]+=sizeof(queue_t);
  queue_t* tmp=(queue_t*)last_element[priority-1];
  tmp->next=q->next;
  tmp->istask=0;
  q->task=NewTask;
  q->istask=1;
  q->next=tmp;

  NewTask->next = StartTask->next;
  NewTask->active=1;

  NewTask->id=next_pid;
  NewTask->priority=priority;

  NewTask->name=name;

  lastMade_task->next=NewTask;
  lastMade_task=NewTask;

  next_pid++;
}

task_t *old_task;
queue_t* old_element;

void Scheduler_exec() //Enable Multitasking !!!
{
		timer_task=(uint32_t)scheduler;
    init_timer(20000);
    old_element=(queue_t*)(queue[0]+20);
    printf("\n\nEntering Scheduling Mode!!!\nNEW EIP: %x\n",current_task->eip);
    asm volatile("sti");
}

void scheduler()
{
    if(timer_tick)
    {
      --timer_tick;
      asm volatile("sti");
      outb(0x20, 0x20); // send EoI to master PIC
      asm volatile("iret");
    }
    old_task=current_task;
    asm volatile("popl %0":"=r"(old_task->eip)::"memory");
    asm volatile("pop %0":"=r"(old_task->cs)::"memory");
    asm volatile("pop %0":"=r"(old_task->eflags)::"memory");
    //uint32_t curr_q=0;
    queue_t* queue_element=(queue_t*)queue[9]; //always start from highest priority queue
    queue_t* q;
    int c_q=(((uint32_t)queue_element)/(1024*1024))-90;
    for(;;)
    {
      queue_element=queue_element->next;
      if(queue_element->istask && queue_element->task->active) //found a task
      {
        c_q=(((uint32_t)queue_element)/(1024*1024))-90;
        if(c_q)
        {
          queue_element->istask=0;
          if(((uint32_t)(queue_element))+20==last_element[c_q])
          {
             last_element[c_q]=queue[c_q]+20; //20=sizeof(queue_t);
          }
          //queue_element=queue_element->next->next;
          current_task=queue_element->task;

          q=(queue_t*)(queue+20);
          q->next=queue_element->next;

          q=(queue_t*)last_element[c_q-1];
          last_element[c_q-1]+=20;
          q->istask=1;
          q->task=current_task;
          uint32_t tmp=(uint32_t)q->next;
          q->next=q+1;
          ++q;
          q->next=(queue_t*)tmp;
          break;
        }
        else
        {
          if((uint32_t)(old_element)==last_element[0])
          {
            old_element=(queue_t*)(queue[0]+20);
          }
          current_task = old_element->task;
          old_element = old_element->next;
          break;
        }
      }
    }
    //poptask(queue_element); //its gonna be done, remove it so.
    timer_tick=timeslice[c_q];//queue_element->ticks;
    asm volatile("jmp *%0"::"r"((uint32_t)switcher));
}

void save_esp() //USED BY switcher() function in tasking.asm
{
    old_task->esp = old_esp;
    new_esp = current_task->esp;
    new_eip = current_task->eip;
    new_cs = current_task->cs;
    new_eflags = current_task->eflags;
    new_seg = current_task->segment;
}

void helper1() //USED BY switcher() function in tasking.asm
{
    asm volatile ( "outb %0, %1" : : "a"(0x20), "Nd"(0x20) ); // send EoI to master PIC
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

task_t* getTask()
{
  uint32_t esp=0;
  asm volatile("mov %%esp, %0":"=r"(esp)::"memory");
  esp/=4096;
  task_t* task=StartTask->next;
  for(;;)
  {
    if(esp==task->esp/4096)
    {
      printf("\nFound :D %s",task->name);
      return task;
    }
    else task=task->next;
    if(task>lastMade_task)
    {
      printf("Not found esp:%x",esp);
      asm volatile("sti");
      return 0;
    }
  }
}

void taskSleep()
{
  asm volatile("cli");
  uint32_t c_q=0;
  task_t* task=getTask();
  if(!task) return;
  printf("\nGiving the task sleep pills");
  task->active=0;
  queue_t* q=(queue_t*)queue[9];
  for(;q!=(queue_t*)last_element[0];)
  {
    q=q->next;
    if(q->task==task && q->istask==1)
    {
      q->istask=0;
      c_q=(((uint32_t)q)/(1024*1024))-90;
      ((queue_t*)queue[c_q])->next=q->next;
    }
  }
  printf("\nThe task is now in sleep mode :)");
  asm volatile("sti;");
  timer_tick=0;
  return;
}

void IOLock(uint8_t irq)
{
  task_t* task=getTask();
  iolocks[irq]=task;
}
