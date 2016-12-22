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
#include "filesystem/ls.c"
#include "filesystem/cd.c"

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
      //_printf("%x-%x-%x ",test_counter,test_pit_timer,(pit_freq*test_counter*(*bottom_queue))/HPET_main_counter);
      _printf("Task Occured: %x\nTimer ticks: %x",test_counter,*HPET_main_counter);
      test_counter = 0;
    }//*/
    asm volatile("int $50");
  }
}

void Command_help()
{
   _printf("\nAvialable Commands are:\n");
   _printf("\n\thelp         To get all the available commands");
   _printf("\n\tmdbug        To test the Memory Managment System");
   _printf("\n\tshutdown     To ACPI Power off the system (may not work on few machines)");
   _printf("\n\tstart vesa   To enter VESA Super VGA Mode");
   _printf("\n\tmemmap       To view the Physical Memory Map of the system");
   _printf("\n\ttest         To Test multitasking");
   _printf("\n\tls           To show the current directory's contents");
   _printf("\n\tcd <dir>     To move to a directory ('0' for root, '..' to move to parent)");
   _printf("\n\tmkdir <name>            To create a directory");
   _printf("\n\tmkfl <name>             To create a file");
   _printf("\n\tcreate <type> <name>    To create a file or a directory ('file' for file, 'dir' for directory)\n");
   _printf("\n\teditfl <name> {<data>}  To edit a file");
   _printf("\n\tdel <name>    To delete a folder/file (by default folder)");
   _printf("\n\tremove        To remove a directory");
   _printf("\n\tother commands\n");
}

void Command_shutdown()
{
      _printf("\n Turning Power off");
      acpiPowerOff();
      asm volatile("cli;\
      hlt");
}

void Command_mdbug()
{
      _printf("\n Testing Virtual Memory Manager");
      dbug();
}

void Command_start_vesa()
{
   _printf("\n Entering VESA SVGA mode 1024*768");

   printf("\n Entering VESA SVGA mode 1024*768");
   //Switch_to_system_dir();
   setVesa(0x117);

   Task_sleep(Shell_Ostream_task);
   Shell_Ostream_task = create_task("Vesa_test_buf", DBuff, 1, 0x202, Shell_proc);
   Task_wakeup(Shell_Ostream_task);

   //Switch_back_from_System();
   /*vesa_lfb();//*/
   //RectD(0,0,1024,768, 0x0092ff);

   RectD(400,300,50,10,0x00ff00);
   RectD(500,300,50,10,0xff0000);
   RectD(600,300,50,10,0x0000ff);

   RectD(400,400,50,10,0x0f00);
   RectD(500,400,50,10,0xf000);
   RectD(600,400,50,10,0x00f0);
   RectD(700,400,50,10,0x000f);
   //RectD(100,300,50,10,7,7,900);

   font_maker();
   font_renderer(char_B, 100, 100, 0xf000, 10, 3);
   font_renderer(char_C, 160, 100, 0xf0ff, 10, 3);
   font_renderer(char_E, 240, 100, 0x0f00, 10, 3);
   font_renderer(char_1, 320, 100, 0xff0f, 10, 3);
   font_renderer(char_2, 380, 100, 0x0f0f, 10, 3);
   font_renderer(char_3, 460, 100, 0xfff0, 10, 3);
   font_renderer(char_4, 520, 100, 0xfa0f, 10, 3);
   Pixel_VESA_BUFF( 500, 500, 0xff00ff);
   line_fast(100,400,100,500,0xffff, 1);
   line_fast(130,400,130,500,0xffff, 10);
  // prime_diff_graph();
   //RectD(510, 510, 500,500,0xff00ff);
   //RectD(100,300,50,10,7,7,7);
   /*while(1)
   {
     //Mouse_Plot(mousex,mousey);
     DBuff();
   }//*/
   Activate_task_direct(create_task("Mouse_Pointer", Mouse_Plot, 2, 0x202, Shell_proc));
   //Activate_task_direct(create_task("Mouse_Updater", mouse_updater, 10, 0x202, Shell_proc));
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
  _printf("\n%x", sizeof(task_t));
  a->esp = 10;
  _printf("\na->esp: %x", a->esp);
  b->esp = 2;
  _printf("\na->esp: %x", a->esp);
  a->special = 1;
  _printf("\na->esp: %x", a->esp);
  b->special = 2;
  _printf("\na->esp: %x", a->esp);
  _printf("\n &a: %x, &b: %x; &a->esp: %x, &b->name: %x; a->esp: %x, b->esp: %x, a->special: %x, b->special: %x", a, b, &a->esp, &b->name, a->esp, b->esp, a->special, b->special);
  *a = *b;
  _printf("\n a->esp: %x, b->esp: %x, a->special: %x, b->special: %x", a->esp, b->esp, a->special, b->special);
}

void Command_start_counter()
{
   asm volatile("cli");
   //init_timer(pit_freq);
   printf("\n%x",hpet->GCIDReg[1]+1);
   Activate_task_direct(create_task("Test_counter", Test_counter, 10, 0x202, kernel_proc));
   test_pit_timer = 0;
   *HPET_main_counter = 0;
   hpet->Main_Counter_Reg = 0;
}

void Command_counter()
{
   _printf("%x %x",test_counter, test_pit_timer);
}

void Command_trate()
{
  asm volatile("cli");
  //uint32_t trate = (pit_freq*test_counter)/HPET_main_counter;
  //_printf("Task switch rate = %x/sec\nOccurance of each task = %x/sec\nTask Occured: %x\nTimer ticks: %x",trate*(*bottom_queue),trate,test_counter,test_pit_timer);
  _printf("Task Occured: %x\nTimer ticks: %x",test_counter,*HPET_main_counter);
  test_counter = 0;
  //test_pit_timer = 0;
  *HPET_main_counter = 0;
  hpet->Main_Counter_Reg = 0;
}

void Command_timeslice()
{
  asm volatile("cli");
   _printf("%x",time_slice);
}

void Command_topq()
{
   _printf("%x",top_queue);
}

void Command_test()
{
   asm volatile("cli");
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));
}

void Command_test_multi()
{
   asm volatile("cli");
   for(int i=0; i<5; i++)
   {
      Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));
   }
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
  //file_load("test3.txt");
  
  printf("\nsizeof(File_Header_t) %x, sizeof(File_t) %x\n",sizeof(File_Header_t),sizeof(File_t));

  char str[]="Hello!!! This is Aqeous OS Speaking And I am Testing the Filesystem. I need a really very very very big message to test the I/O operations of my file system's file handling. So please let me do what I want. This is a completely new FileSystem and thus needs a lot of testing. There are Several bugs and I am gonna fix them all!!! As I Said that I need a really very long message to test wether the file handling can actually manage reading writing operations with data bigger then the sector size, that is 512 bytes, so thats why I am writing this long test. The text should be longer then 512 bytes, That is the text should contains 512 characters. I am writing this in block alignmnet to manage space :3 This filesystem can be potentially better then FAT and so Its necessary to remove allthe bugs before releasing it. Thats obvious LOL xD .So please be patient. Thank you :)";
  printf("%x", strlen(str));
  if(file_writeAppend((uint32_t)str,strlen(str),path)>0)
  {
    printf("\nWritten Successfully");
  }
  file_flushOGP(path);

  uint32_t buffer=fsalloc(8192);
  file_readTM(buffer,0,2048,path);


  printf("\nThe file reads as: %s", buffer);/*/
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
   _printf("\n%x",*bottom_queue);
}

void Command_baseshow()
{
   asm volatile("cli");
   _printf("\n%x",*bottom_queue);
   uint32_t* tmp = bottom_queue + 1;
   for(uint32_t i=0;i<*bottom_queue;i++)
   {
      _printf("\t%s", ((task_t*)(*tmp))->name);
      ++tmp;
   }
}

void Command_qelements()
{
   _printf("\n%x",kb_q_elements);
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
