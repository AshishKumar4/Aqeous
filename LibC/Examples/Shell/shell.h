#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"

volatile uint32_t* Shell_Commands_list;

typedef void (*func_t)();

typedef struct Shell_Commands
{
    func_t func;
    int reserved;
    char command[16];
}Shell_Commands_t;

typedef struct CSI_struct
{
    uint32_t SC_addr;   //SC = Shell Commands struct (above)
    uint32_t total_entries;
    uint32_t entries[64];
}CSI_struct_t;

volatile CSI_struct_t* Main_CSI_struct;

uint32_t* CSI_entries_ptr;
uint32_t* tot_entries;

volatile uint32_t* CSI_mem_start;

void Shell();

void Shell_ArgParse(char* str);

int Shell_command_locator(char *inst);

uint32_t* CSI_Read(uint32_t entry);

uint32_t* CSI_ReadAPS(char* str); //Read As the Provided String!

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name);

#endif
