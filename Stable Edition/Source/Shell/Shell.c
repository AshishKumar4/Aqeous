#include "shell.h"
#include "mem.h"
#include "vmem.h"
#include "console.h"

uint32_t VGA_size;

void init_shell()
{
//  VGA_buffer_location = 3745513472;
  //VGA_buffer = 0xCF000000;//(uint16_t*)VGA_buffer_location;
//  VGA_buffer_ptr = VGA_buffer;
  memset((void*)VGA_buffer, 0, 4194304);
  VGA_size = VGA_WIDTH*VGA_HEIGHT*2;
}

void Shell_task()
{
  while(1);
}

void Console_Writer()
{
//  uint32_t volatile tmp = 0;
  while(1)
  {
    asm volatile("cli");

    //TODO: COPY THE VGA_BUFFER TO THE ACTUAL VGA BUFFER (console_buffer)

  //  tmp = VGA_buffer_ptr/VGA_WIDTH;
  //  ++tmp;
  //  tmp *= VGA_WIDTH;
  //  tmp -= VGA_size;

    memcpy((void*)console_buffer, (void*)VGA_buffer, VGA_size);
    //memset((void*)VGA_buffer, 0, VGA_size);

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
