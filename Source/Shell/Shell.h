#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include <console.h>
#include <common.h>

#define MAX_COMMANDS 100
#define Input_stream 576716800
#define Output_stream 524288000

bool key_pressed = false, enter_pressed = false;
uint32_t* Istream_ptr = (uint32_t*)Input_stream, Current_strlen;
uint8_t* Current_buf;
int kb_buff = 0;
char* shell_buf;
uint32_t shell_in = 0;

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

void Console_Writer();

void Shell_Input();

void Shell_sleep();

void Shell();

#endif
