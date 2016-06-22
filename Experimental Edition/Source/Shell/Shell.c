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
#include "Default_Commands.c"

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
  memset_fast((void*)console_dbuffer, 0, 4194304);
  VGA_size = VGA_WIDTH*VGA_HEIGHT/2;
  shell_buf = (char*)kmalloc(4096);
  Shell_Commands_list = (uint32_t*)kmalloc(8192);
  console_manager_init();
  Switch_back_from_System();
}

void __attribute__((optimize("O0"))) Shell_Double_buffer()
{
   uint32_t* _s, *_c;
   while(1)
   {
     //asm volatile("cli");

     //TODO: COPY THE VGA_BUFFER TO THE ACTUAL VGA BUFFER (console_buffer)
     _s = (uint32_t*)console_buffer;
     _c = (uint32_t*)console_dbuffer;
     for ( int _n = VGA_size; _n != 0; _n--)
     {
      *_s++ = *_c++;
     }
     //asm volatile("cli");
     if(console_skip)
     {
      console_dbuffer += (VGA_WIDTH*console_skip);
      consolerow -= console_skip;
      console_skip = 0;
      if((uint32_t)console_dbuffer >= console_dbuffer_limit)   //TODO: Make the console buffer mapping in such a way so that this method isnt required.
      {
         console_dbuffer = (uint16_t*)console_dbuffer_original;
         _s = (uint32_t*)console_dbuffer;
         for ( int _n = 4194304; _n != 0; _n--)
         {
          *_s++ = 0;
         }
      }
     }
     asm volatile("int $50");
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
        _putchar((int)'\n');
        *(Istream_ptr) = '\0';
        if(q_elements)
        {
          char* tmp = (char*)(Istream_ptr - kb_buff);
          strcpy((char*)Start_q->buffer, tmp);
          kb_buff = 0;

          Task_wakeup(Start_q->task);
          Shell_wakeup();
          memset_faster((uint32_t*)Start_q, 0, 4);
          Start_q = Start_q->next;
          --q_elements;
          //TODO: Load the next element
          Current_buf = (uint8_t*)Start_q->buffer;
          Current_strlen = 0;
        }
        else if(shell_awake)  //TODO: Give all input to the Shell Directly!
        {
          Priority_promoter(Shell_task);
          shell_in = 1;
          shell_buf = Istream_ptr - kb_buff;
          kb_buff = 0;
        }
        else
        {
          _printf("ALERT");
        }
      }
      enter_pressed = 0;
    }
    asm volatile("int $50");
  }
}
/*
void Shell_Finput()
{
  while(1)
  {

  }
}

void Shell_Foutput()
{
  while(1)
  {

  }
}
*/
void Shell()
{
  while(1)
  {
    if(shell_awake)
    {
      _printf("\n%s>",curr_dir.full_name);
      while(!shell_in)
      {
        asm volatile("int $50");
      }
      //asm volatile("cli");
      //console_manager((char*)shell_buf);
      Shell_command_locator((char*)shell_buf);
      *Shell_Istream = (uint32_t)shell_buf;
      ++Shell_Istream;
      shell_in = 0;
      shell_buf = 0;
    }
    asm volatile("int $50");
  }
}

void Shell_sleep()  //TODO: Make the Shell task to sleep.
{
  asm volatile("cli");
  if(!shell_sleeping)
  {
    Switch_to_system_dir();
    uint32_t* shell_place_holder = (uint32_t*)Shell_task->active;
    *shell_place_holder = (uint32_t)Spurious_task;
    Shell_task->active = 0;
    shell_awake = 0;
    Switch_back_from_System();
  }
  ++shell_sleeping;
  asm volatile("sti");
}

void Shell_wakeup()
{
  asm volatile("cli");
  --shell_sleeping;
  if(!shell_sleeping)
  {
    Task_wakeup(Shell_task);
    shell_awake = 1;
  }
  asm volatile("sti");
}

int Shell_command_locator(char *inst)
{
   uint32_t tmp = 0;
   char* tmpstr = strtok(inst, " ");
   uint32_t spaces = stroccr(inst, " ");
   //int tmp2 = 0;
   /*
   for( int i = 0 ; inst[i]!='\0' ; i++ )    //Alternate Algorithm
   {
      tmp2 = (((int)inst[i])-((int)inst[i+1]));
      if(tmp2 >= 0)
         tmp += tmp2*(i+1);
      else
         tmp += (-tmp2)*(i+1);
}*/
   int i;
   for( i = 0 ; tmpstr[i]!='\0' && i <= 16; i++ )
   {
      if((uint32_t)tmpstr[i] >= 97)
         tmp += (((uint32_t)tmpstr[i]) - 97)*(i+1);
      else
         tmp += (((uint32_t)tmpstr[i]) - 65)*(i+1);
   }
   if(tmp <= 2048)
   {
      uint32_t* ptr = (uint32_t*)Shell_Commands_list[tmp];
      if(ptr)
      {
         func_t func = ((Shell_Commands_t*)ptr)->func;
         /*
         if(!strncmp(inst, ((Shell_Commands_t*)ptr)->command, i))
            func();
         else
         {
              _printf("\n Command Not Recognized! type help for help\n");
             return -1;
         }*/
         uint32_t* tmp2 = Main_CSI_struct->entries;
         uint32_t* tot_entries = Main_CSI_struct->total_entries;
         for(; ;)
         {
            tmpstr = strtok(NULL, "-");
            if(tmpstr== NULL) break;
            *tmp2 = (uint32_t)tmpstr;
            ++tmp2;
            ++(*tot_entries);
            tmpstr = strtok(NULL, " ");
            if(tmpstr== NULL) break;
            *tmp2 = (uint32_t)tmpstr;
            ++tmp2;
            ++(*tot_entries);
         }
         func();
         memset_faster(CSI_mem_start, 0, 66);
         return ((Shell_Commands_t*)ptr)->reserved;
      }
      else
      {
    	    _printf("\n Command Not Recognized! type help for help\n"); //TODO: Search within other possible files/executables like in the PATH string.
          return -1;
      }
   }
   else
   {
      _printf("\n Command Not Recognized! type help for help\n");
      return -1;
   }
}

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name)
{
   Shell_Commands_t* command = (Shell_Commands_t*)kmalloc(sizeof(Shell_Commands_t));
   command->func = func;
   command->reserved = flag;
   Shell_Commands_list[command_no] = (uint32_t)command;
   strcpy(command->command, name);
}

void console_manager_init()
{
	Shell_Add_Commands(Command_help, 108, 0, "help");
   Shell_Add_Commands(Command_shutdown, 525, 0, "shutdown");
   Shell_Add_Commands(Command_mdbug, 131, 0, "mdbug");
   //Shell_Add_Commands(Command_start_vesa, 2042, 1);
   Shell_Add_Commands(Command_memmap, 194, 0, "memmap");
   //Shell_Add_Commands(Command_start_counter, 0);
   Shell_Add_Commands(Command_counter, 380, 0, "counter");
   Shell_Add_Commands(Command_timeslice, 351, 0, "timeslice");
   Shell_Add_Commands(Command_topq, 156, 0, "topq");
   Shell_Add_Commands(Command_test, 157, 0, "test");
   //Shell_Add_Commands(Command_test_multi, 1759, 0);
   Shell_Add_Commands(Command_ls, 47, 0, "ls");
   Shell_Add_Commands(Command_cd, 8, 0, "cd");
   Shell_Add_Commands(Command_clrscr, 259, 0, "clrscr");
   Shell_Add_Commands(Command_baseln, 204, 0, "baseln");
   Shell_Add_Commands(Command_baseshow, 477, 0, "baseshow");
   Shell_Add_Commands(Command_qelements, 562, 0, "qelements");
   Shell_Add_Commands(Command_dbuffplusplus, 1344, 0, "dbuffplusplus");
   memset_faster(CSI_mem_start, 0, 66);
}
