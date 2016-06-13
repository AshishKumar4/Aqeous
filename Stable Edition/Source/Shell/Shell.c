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

uint32_t VGA_size;

void init_shell()
{
//  VGA_buffer_location = 3745513472;
// VGA_buffer = 0xCF000000;
// (uint16_t*)VGA_buffer_location;
//  VGA_buffer_ptr = VGA_buffer;
  Switch_to_system_dir();
  consolerow = 0;
	consolecolumn = 0;
//  console_buffer = vga_mem;
  memset((void*)VGA_buffer, 0, 4194304);
  VGA_size = VGA_WIDTH*VGA_HEIGHT*2;
  shell_buf = (char*)kmalloc(4096);
  Switch_back_from_System();
}

void Console_Writer()
{
  while(1)
  {
    asm volatile("cli");

    //TODO: COPY THE VGA_BUFFER TO THE ACTUAL VGA BUFFER (console_buffer)

    memcpy((void*)console_buffer, (void*)VGA_buffer, VGA_size);

    asm volatile("sti;\
    int $50");
  }
}

void Shell_Input()
{
  while(1)
  {
    asm volatile("cli");
    if(enter_pressed)
    {
      if(kb_buff)
      {
        _printf("\n");
        *(Istream_ptr) = '\0';
        if(q_elements)
        {
          char* tmp = Istream_ptr - kb_buff;
          strcpy(Start_q->buffer, tmp);
          kb_buff = 0;

          Task_wakeup(Start_q->task);
          memset((void*)Start_q, 0, 16);
          Start_q = Start_q->next;
          --q_elements;
          //TODO: Load the next element
          Current_buf = (uint8_t*)Start_q->buffer;
          Current_strlen = 0;
        }
        else  //TODO: Give all input to the Shell Directly!
        {
          Priority_promoter(Shell_task);
          shell_in = 1;
          shell_buf = Istream_ptr - kb_buff;
          kb_buff = 0;
        }
      }
      enter_pressed = 0;
    }
    asm volatile("sti;\
    int $50");
  }
}

extern uint8_t console_manager(char* inst);

void Shell()
{
  char* tmp = shell_buf;
  uint8_t flg=0;
  while(1)
  {
    _printf("\n%s>",curr_dir.full_name);
    while(!shell_in)
    {
      asm volatile("int $50");
    }
    asm volatile("cli");
    flg=console_manager(shell_buf);
    shell_in = 0;
    shell_buf = 0;
    asm volatile("sti;\
    int $50");
  }
}

void Shell_sleep()  //TODO: Make the Shell task to sleep.
{
  if(shell_awake)
  {
    Switch_to_system_dir();
    uint32_t* volatile shell_place_holder = Shell_task->active;
    *shell_place_holder = Spurious_task;
    shell_awake = false;
    Switch_back_from_System();
  }
}

void Shell_wakeup()
{
  Task_wakeup(Shell_task);
}

extern void dbug();

uint8_t console_manager(char *inst)
{
	if(!strcmp(inst,"help"))
	{
		_printf("\nAvialable Commands are:\n");
		_printf("\n\thelp         To get all the available commands");
		_printf("\n\tmdbug        To test the Memory Managment System");
		_printf("\n\tshutdown     To ACPI Power off the system (may not work on few machines)");
		_printf("\n\tstart vesa   To enter VESA Super VGA Mode");
		_printf("\n\tmemmap       To view the Physical Memory Map of the system");
		_printf("\n\ttest multi   To Test multitasking");
		_printf("\n\tls           To show the current directory's contents");
		_printf("\n\tcd <dir>     To move to a directory ('0' for root, '..' to move to parent)");
		_printf("\n\tcreate <type> <name>  To create a file or a directory ('file' for file, 'dir' for directory)\n");
		_printf("\n\tedit <name> {<data>}  To edit a file");
		_printf("\n\tdel <name>    To delete a file");
		_printf("\n\tremove        To remove a directory");
		_printf("\n\tother commands\n");
		return 0;
	}
	else if(!strcmp(inst,"shutdown"))
	{
			_printf("\n Turning Power off");
			Switch_to_system_dir();
			acpiPowerOff();
			return 0;
	}
	else if(!strcmp(inst,"mdbug"))
	{
			_printf("\n Testing Virtual Memory Manager");
			//Switch_to_system_dir();
			dbug();
			//Switch_back_from_System();
			return 0;
	}
	else if(!strcmp(inst,"start vesa"))
	{
		_printf("\n Entering VESA SVGA mode 1024*768");
		Switch_to_system_dir();
		vesa(0x117);
    Activate_task_direct(create_task("Vesa_test_buf", vesa_test_dbuf, 10, 0x202, Shell_proc));
		Switch_back_from_System();
		return 1;
	}
	else if(!strcmp(inst,"memmap"))
	{
    _printf("\nMemory Map:");
    MemRegion_t* memmap_info=mmap_info;
		Switch_to_system_dir();

    for(int i=0;i<15;i++)
    {
        if(memmap_info->startLo==0) break;
        _printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,memmap_info->startHi,memmap_info->sizeHi,
               memmap_info->type,strMemoryTypes[memmap_info->type-1]);
        memmap_info++;
    }
		Switch_back_from_System();
		return 0;
	}
  else if(!strcmp(inst,"test multi"))
  {
    Activate_task_direct(create_task("Test_process", test_process, 10, 0x202, kernel_proc));
    return 0;
  }
  else if(!strcmp(inst,"ls"))
  {
    find_dir(0);
    return 0;
  }
  else if(!strncmp(inst,"cd",2))
  {
    
    return 0;
  }
	_printf("\n Command Not Recognized! type help for help\n");
	return 0;
}

void vesa_test_dbuf()
{
  while(1)
  {
    asm volatile("cli");
    DBuff();
    asm volatile("sti;");
  //  int $50");
  }
}
