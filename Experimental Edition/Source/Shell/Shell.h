#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include <console.h>

#define MAX_COMMANDS 100
/*
typedef struct CommandDiscription
{
    char* description;
}Description_t;

typedef struct Command
{
    char *name;
    char *description;
    void *function;
}Command_t;

Command_t Command_table[MAX_COMMANDS];

uint32_t Number_of_commands=0;

char* Input;
*/
void init_shell();

//void shell();

//void Add_command(char* name,char* discription,void* function);

void Shell_task();

void Console_Writer();

#endif
