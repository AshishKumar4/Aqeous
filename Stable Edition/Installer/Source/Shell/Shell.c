#include <shell.h>

void init_shell()
{

}

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
}
