#include "shell.h"
#include "mem.h"
#include "vmem.h"
#include "console.h"
#include "tasking.h"
#include "kb_queue.h"
#include "common.h"
#include "paging.h"
#include "vesa.h"
#include "fs.h"
#include "graphics.h"
#include "PIC.h"
#include "memfunc.h"

extern uint32_t time_slice;

extern int detect_cpu(void);

extern void dbug();

volatile uint32_t test_counter = 0;

void vesa_test_dbuf()
{
   while(1)
   {
      DBuff();
   }
}

void Test_counter()
{
  while(1)
  {
    ++test_counter;
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
   _printf("\n\tcreate <type> <name>  To create a file or a directory ('file' for file, 'dir' for directory)\n");
   _printf("\n\tedit <name> {<data>}  To edit a file");
   _printf("\n\tdel <name>    To delete a file");
   _printf("\n\tremove        To remove a directory");
   _printf("\n\tother commands\n");
}

void Command_shutdown()
{
      _printf("\n Turning Power off");
      Switch_to_system_dir();
      acpiPowerOff();
}

void Command_mdbug()
{
      _printf("\n Testing Virtual Memory Manager");
      Switch_to_system_dir();
      dbug();
      Switch_back_from_System();
}

void Command_start_vesa()
{
   _printf("\n Entering VESA SVGA mode 1024*768");
   Switch_to_system_dir();
   vesa(0x117);
   Activate_task_direct(create_task("Vesa_test_buf", vesa_test_dbuf, 10, 0x202, Shell_proc));
   Switch_back_from_System();
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

volatile void Command_test_structs()
{
  asm volatile("cli");
  task_t* a = kmalloc(sizeof(task_t));
  task_t* b = kmalloc(sizeof(task_t));
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
   Activate_task_direct(create_task("Test_counter", Test_counter, 10, 0x202, kernel_proc));
}

void Command_counter()
{
   _printf("%x",test_counter);
}

void Command_timeslice()
{
   _printf("%x",time_slice);
}

void Command_topq()
{
   _printf("%x",top_queue);
}



void Command_test()
{
   asm volatile("cli");
   Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, create_process("test_abc", 0, 1, 0)));
   //asm volatile("sti;\
   int $50");
  // uint32_t* abc = ;
}

void Command_test_multi()
{
   asm volatile("cli");
   for(int i=0; i<5; i++)
   {
      Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));
   }
   //asm volatile("sti;\
   int $50");
}

void Command_ls()
{
  // asm volatile("cli");
   find_dir(0);
}

void Command_cd()
{
  //asm volatile("cli");
    char* dir = (char*)CSI_Read(1);

//   char* dir = shell_buf + 3;
   if(!strncmp(dir,"..",2))
   {
      set_curr_dir(curr_dir.dir.parent);
   }
   else
   {
      Directory_t* new_dir = search_folder(dir);
      if(new_dir)
        set_curr_dir(new_dir->location);
    //  while(1);
      //  set_curr_dir(curr_dir.dir.location);
      //printf("%s",new_dir->name);
   }
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
   _printf("\n%x",q_elements);
}

void Command_dbuffplusplus()
{
   console_dbuffer += VGA_WIDTH;
}

void Command_detect_cpu()
{
  detect_cpu();
}
