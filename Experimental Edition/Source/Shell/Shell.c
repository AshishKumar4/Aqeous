#include "shell.h"
#include "mem.h"
#include "vmem.h"
#include "console.h"
#include "tasking.h"
#include "kb_queue.h"
#include "common.h"
#include "paging.h"
#include "vesa.h"

uint32_t VGA_size;

void init_shell()
{
//  VGA_buffer_location = 3745513472;
  //VGA_buffer = 0xCF000000;//(uint16_t*)VGA_buffer_location;
//  VGA_buffer_ptr = VGA_buffer;
  Switch_to_system_dir();
//  console_buffer = vga_mem;
  memset((void*)VGA_buffer, 0, 4194304);
  VGA_size = VGA_WIDTH*VGA_HEIGHT*2;
  shell_buf = (char*)kmalloc(4096);
  Switch_back_from_System();
}

void Console_Writer()
{
//  uint32_t volatile tmp = 0;
  while(1)
  {
    asm volatile("cli");

  //  Switch_to_system_dir();
    //TODO: COPY THE VGA_BUFFER TO THE ACTUAL VGA BUFFER (console_buffer)

  //  tmp = VGA_buffer_ptr/VGA_WIDTH;
  //  ++tmp;
  //  tmp *= VGA_WIDTH;
  //  tmp -= VGA_size;

    memcpy((void*)console_buffer, (void*)VGA_buffer, VGA_size);
    //memset((void*)VGA_buffer, 0, VGA_size);
  //  Switch_back_from_System();
    asm volatile("sti;\
    int $50");
  }
}

void Shell_Input()
{
  while(1)
  {
    asm volatile("cli");

  //  Switch_to_system_dir();
    if(q_elements)
    {
      if(enter_pressed||Current_strlen >= Start_q->str_ln)
      {
        Current_q = Start_q;
        Start_q = Start_q->next;
        Task_wakeup(Current_q->task);
        memset((void*)Current_q, 0, 32);
        --q_elements;
        if(!q_elements)
        {
          Task_wakeup(Shell_task);
        }
        //TODO: Load the next element
        Current_buf = (uint8_t*)Start_q->buffer;
        Current_strlen = 0;
      }
      else if(key_pressed)
      {
        key_pressed = false;
        //TODO: Copy ONLY the string length required, shold be less then or equal to [q_element]->str_ln.
        uint32_t tmp = Istream_ptr - kb_buff;
        if(kb_buff + Current_strlen <= Start_q->str_ln)
        {
          memcpy(Current_buf, tmp, kb_buff);
          Current_buf += kb_buff;
          Current_strlen += kb_buff;
          kb_buff = 0;
        }
        else
        {
          //TODO: copy the length of string required and do something with the ones left over.
        }
      }
    }
    else  //TODO: Give all input to the Shell Directly!
    {
      if(key_pressed)
      {
        Priority_promoter(Shell_task);
        shell_in = 1;
      }
    }

  //  Switch_back_from_System();
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
    printf("\n%s>",curr_dir.full_name);

    for(int i=0; *Istream_ptr != '\n'; i++)
    {
      while(!shell_in);
      *tmp = *Istream_ptr;
      ++tmp;
    }

    flg=console_manager(shell_buf);
    memset((void*)shell_buf, 0, strlen(shell_buf));
  }
}

void Shell_sleep()  //TODO: Make the Shell task to sleep.
{
  while(1)
  {
    asm volatile("cli");
    DBuff();
    asm volatile("sti;\
    int $50");
  }
}

/*
void shell()
{
  printf("cmd>");
  getline(Input);
  void (*Command)(void);
  int i = findCommand(Input);
  if(i >= 0)
  {
      Command = Command_table[i].function;
      Command();
  }
  else
  {
      return;
  }

  return;
}

void Add_command(char* name, char* discription, void* function)
{
  if(!(MAX_COMMANDS-Number_of_commands-1))
  {
    Command_table[Number_of_commands].name=name;
    Command_table[Number_of_commands].discription=discription;
    Command_table[Number_of_commands].function=function;
    ++Number_of_commands;
  }
  return;
}

int findCommand(char* command)
{
    int i;
    int ret;

    for(i = 0; i < Number_of_commands + 1; i++)
    {
        if(!strcmp(command, Command_table[i].name))
            return i;
        else
            return -1;
    }
}*/
