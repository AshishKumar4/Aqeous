#include "shell.h"
#include "stdlib.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "phy_mm/mem.h"
#include "console.h"
#include "tasking.h"
#include "kb_handle.h"
#include "common.h"
#include "vesa.h"
#include "fs.h"
#include "graphics.h"
#include "PIC.h"
#include "memfunc.h"
#include "fonts.h"
#include "mouse.h"
#include "timer.h"
#include "hpet.h"

#include "ext2/ext2_fs.h"

#include "init/init.c"

#include "filesystem/general.c"
#include "filesystem/FSspecific/aqfs.c"
#include "filesystem/FSspecific/aqfs2.c"
#include "filesystem/FSspecific/ext2.c"
#include "processing/elfload.c"

#include "Scheduler/Scheduler.h"
#include "ProcManagement/ProcManagement.h"
#include "stdlib.h"

#include "rn/rn.c"

#include "ANN/ann.c"
#include "vesa/vesa_commands.c"

extern uint32_t time_slice;

extern uint32_t test_pit_timer;

extern int detect_cpu(void);

extern void dbug();

volatile uint32_t test_counter = 0;

uint32_t pit_freq = 1000;

void Test_counter()
{
  while(1)
  {
    ++test_counter;
    if(test_counter>10000)
    {
      //printf("%x-%x-%x ",test_counter,test_pit_timer,(pit_freq*test_counter*(*bottom_queue))/HPET_main_counter);
      printf("Task Occured: %x\nTimer ticks: %x",test_counter,*HPET_main_counter);
      test_counter = 0;
    }//*/
    asm volatile("int $50");
  }
}

void Command_help()
{
   printf("\nAvialable Commands are:\n");
   printf("\n\thelp         To get all the available commands");
   printf("\n\tmdbug        To test the Memory Managment System");
   printf("\n\tshutdown     To ACPI Power off the system (may not work on few machines)");
   printf("\n\tstart vesa   To enter VESA Super VGA Mode");
   printf("\n\tmemmap       To view the Physical Memory Map of the system");
   printf("\n\ttest         To Test multitasking");
   printf("\n\tls           To show the current directory's contents");
   printf("\n\tcd <dir>     To move to a directory ('0' for root, '..' to move to parent)");
   printf("\n\tmkdir <name>            To create a directory");
   printf("\n\tmkfl <name>             To create a file");
   printf("\n\tcreate <type> <name>    To create a file or a directory ('file' for file, 'dir' for directory)\n");
   printf("\n\teditfl <name> {<data>}  To edit a file");
   printf("\n\tdel <name>    To delete a folder/file (by default folder)");
   printf("\n\tremove        To remove a directory");
   printf("\n\tother commands\n");
}

void Command_aptest()
{
  printf("\n\t%x",*(uint32_t*)(0x00000500));
}

void Command_shutdown()
{
  printf("\n Turning Power off");
  acpiPowerOff();
  asm volatile("cli;\
  hlt");
}

void Command_mdbug()
{
  printf("\n Testing Virtual Memory Manager");

	uint32_t *temp1=(uint32_t*)kmalloc(810),*temp2=temp1;
	*temp1=4284;

	//kmalloc(4096);
	uint32_t *test1=(uint32_t*)kmalloc(1024),*test2=test1;
	uint32_t *test3=(uint32_t*)kmalloc(1024*1024);
	uint32_t *test4=(uint32_t*)kmalloc(432);
	//printf("Ax%x",kmalloc(1024*1024*40));
	uint32_t *test5=(uint32_t*)kmalloc(128);

	printf("\n\tLocation of var 1: %x, var 2: %x var 3: %x var 4: %x var 5: %x \n",temp1,test1,test3,test4,test5);
//	printf("\nvar1->special: %x, var2->esp: %x", &((task_t*)temp1)->special, &((task_t*)test1)->esp);
	printf("\tPutting Magic Numbers into first two addresses\n");
	for(int i=0;i<8;i++)
	{
		*temp1=4284;
		++temp1;
	}
	temp1=temp2;
	for(int i=0;i<8;i++)
	{
		*test1=100;
		++test1;
	}
	printf("\tDone, Now Reading what we just wrote, 4284 on first few memory, 100 on others\n");
	test1=test2;
	for(int i=0;i<8;i++)
	{
		printf(" %d ",*temp1);
		++temp1;
	}
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*test1);
		++test1;
	}
	temp1=temp2;
	test1=test2;
	*test3=12345678;
	*test4=87654321;
	printf(" %x %x ",*test3,*test4);
	printf("If you just saw few 4284's and 100's and nothing else, no extra space; everything worked fine!\n");
	printf("Now Freeing the memory!\n");
	//free(temp2);
	kfree(test1);
//	kfree(temp2);
	//free(test3);
///*
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*temp1);
		++temp1;
	}
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*test1);
		++test1;
	}
//	printf(" %x %x ",*test3,*test4);
	printf(" If you didnt saw any numbers above, It worked!!!\n");
	uint32_t *tmp1=(uint32_t*)kmalloc(32),*tmp2=tmp1;
	//kmalloc(4096);
	uint32_t *oooooopp=(uint32_t*)kmalloc(64),*tst2=oooooopp;

	printf("\tLocation of var 1: %x, var 2: %x %x\n",tmp1,oooooopp,kmalloc(12));//*/
}


void Command_memmap()
{
   printf("\nMemory Map:");
   MemRegion_t* memmap_info=mmap_info;
  // Switch_to_system_dir();

   for(int i=0;i<15;i++)
   {
        if(memmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,memmap_info->startHi,memmap_info->sizeHi,
               memmap_info->type,strMemoryTypes[memmap_info->type-1]);
        memmap_info++;
   }
  // Switch_back_from_System();
}

void Command_test_structs()
{
  asm volatile("cli");
  task_t* a = (task_t*)kmalloc(sizeof(task_t));
  task_t* b = (task_t*)kmalloc(sizeof(task_t));
  printf("\n%x", sizeof(task_t));
  a->esp = 10;
  printf("\na->esp: %x", a->esp);
  b->esp = 2;
  printf("\na->esp: %x", a->esp);
  a->special = 1;
  printf("\na->esp: %x", a->esp);
  b->special = 2;
  printf("\na->esp: %x", a->esp);
  printf("\n &a: %x, &b: %x; &a->esp: %x, &b->name: %x; a->esp: %x, b->esp: %x, a->special: %x, b->special: %x", a, b, &a->esp, &b->name, a->esp, b->esp, a->special, b->special);
  *a = *b;
  printf("\n a->esp: %x, b->esp: %x, a->special: %x, b->special: %x", a->esp, b->esp, a->special, b->special);
}

void Command_start_counter()
{
   //asm volatile("cli");
   //init_timer(pit_freq);
   printf("\n%x",hpet->GCIDReg[1]+1);
   Activate_task_direct(create_task("Test_counter", Test_counter, 10, 0x202, kernel_proc));
   test_pit_timer = 0;
   *HPET_main_counter = 0;
   hpet->Main_Counter_Reg = 0;
}

void Command_counter()
{
  uint32_t rtc = ReadFromCMOS();
  uint32_t* rt = (40*1024*1024);
  for(int i = rtc; i < rtc + 5;)
  {
    if(ReadFromCMOS() != rtc)
    {
      printf("\n=>%d", *rt);
      *rt = 0;
      ++i;
    }
  }
}

void Command_trate()
{
  asm volatile("cli");
  //uint32_t trate = (pit_freq*test_counter)/HPET_main_counter;
  //printf("Task switch rate = %x/sec\nOccurance of each task = %x/sec\nTask Occured: %x\nTimer ticks: %x",trate*(*bottom_queue),trate,test_counter,test_pit_timer);
  printf("Task Occured: %x\nTimer ticks: %x",test_counter,*HPET_main_counter);
  test_counter = 0;
  //test_pit_timer = 0;
  *HPET_main_counter = 0;
  hpet->Main_Counter_Reg = 0;

  int pit_val = (*(uint32_t*)(40*1024*1024));
  (*(uint32_t*)(40*1024*1024)) = 0;
  int rtc_val = ReadFromCMOS();
  printf("\tPIT: %d RTC: %d",pit_val, rtc_val);
}

void Command_timeslice()
{
  asm volatile("cli");
  SchedulerKits_t* kit = Get_Scheduler();
   printf("%x",kit->timeslice);
}

void Command_topq()
{
  SchedulerKits_t* kit = Get_Scheduler();
   printf("%x",kit->top_queue);
}

void Command_test()
{
   asm volatile("cli");
   Activate_task_direct_SP(create_task("Test_process", test_process, 10, 0x202, kernel_proc), &KitList[0]);

/*   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));//, Get_Scheduler());/*/
   //asm volatile("int $50");
   return;
}

void Command_check()
{
  asm volatile("cli");
  char* device_name = (char*)CSI_Read(1);
  if(!strcmp("ahci", device_name))
  {
    checkAHCI();
  }
}

void Command_test_multi()
{
   asm volatile("cli");
   for(int i=0; i<5; i++)
   {
      Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));
   }
   asm volatile("int $50");
}

void Command_testfs()
{
  asm volatile("cli");
  //file_load("test.txt");
//  file_load("test2.txt");
  char* path = CSI_Read(1);
//  path_finder(path);
  //search_folderOGP(path);
  file_loadOGP(path);
  printf("\n%s\n",path);
  //file_load("test3.txt");

  printf("\nsizeof(File_Header_t) %x, sizeof(File_t) %x\n",sizeof(File_Header_t),sizeof(File_t));

  char str[]="Hello!!! This is Aqeous OS Speaking And I am Testing the Filesystem. I need a really very very very big message to test the I/O operations of my file system's file handling. So please let me do what I want. This is a completely new FileSystem and thus needs a lot of testing. There are Several bugs and I am gonna fix them all!!! As I Said that I need a really very long message to test wether the file handling can actually manage reading writing operations with data bigger then the sector size, that is 512 bytes, so thats why I am writing this long test. The text should be longer then 512 bytes, That is the text should contains 512 characters. I am writing this in block alignmnet to manage space :3 This filesystem can be potentially better then FAT and so Its necessary to remove allthe bugs before releasing it. Thats obvious LOL xD .So please be patient. Thank you :)";
  printf("%x", strlen(str));
  if(!file_writeAppend((uint32_t)str,strlen(str),path))
  {
    printf("\nWritten Successfully");
  }
  file_flushOGP(path);

  uint32_t buffer=fsalloc(8192);
  file_readTM(buffer,0,2048,path);
  file_closeOGP(path);


  printf("\nThe file reads as: %s\n", buffer);/*/
  file_size("test2.txt");

  //file_close("test.txt");
  file_close("test2.txt");
  //file_close("test3.txt");*/

}

void Command_secalloc()
{
  printf("\n%x", sec_alloc(1));
}

void Command_clrscr()
{
   asm volatile("cli");
   console_dbuffer = (uint16_t*)(console_dbuffer_original);
   memset_fast((void*)console_dbuffer, 0, 4194304);
   console_rows_skipped = 0;
   consolerow = 0;
   consolecolumn = 0;
   console_skip = 0;
   asm volatile("sti");
}

void Command_baseln()
{
  SchedulerKits_t* kit = Get_Scheduler();
   printf("\n%x",*kit->bottom_queue);
}

void Command_baseshow()
{
   asm volatile("cli");
   char* core = CSI_Read(1);
   uint32_t coreNum = 0;
   if(core)
    coreNum = StrToInt(core);
   SchedulerKits_t* kit = &KitList[coreNum];
   printf("\n%x",*kit->bottom_queue);
   uint32_t* tmp = kit->bottom_queue + 1;
   for(uint32_t i=0;i<*kit->bottom_queue;i++)
   {
      printf("\t%s", ((task_t*)(*tmp))->name);
      ++tmp;
   }
}

void Command_qelements()
{
   printf("\n%x",kb_q_elements);
}

void Command_dbuffplusplus()
{
   console_dbuffer += VGA_WIDTH;
}

void Command_detect_cpu()
{
  detect_cpu();
  check_SSE();
  if(SSE_available) printf("\nSSE is Available");
}

