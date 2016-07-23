#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include <console.h>
#include <common.h>

//#define MAX_COMMANDS 100
//#define Output_stream 524288000
//#define Shell_Istream_start 577773568
//#define Shell_Istream_end 578822144


#define CSI_addr 52428800 //50th MB; CSI = Common Shell Interface; a Memory mapped area for programs to interact with the shell.


volatile uint32_t* Shell_Commands_list;

typedef struct __attribute__((packed)) Shell_Commands
{
    func_t func;
    int reserved;
    char command[16];
}Shell_Commands_t;

typedef struct __attribute__((packed)) CSI_struct
{
    uint32_t SC_addr;   //SC = Shell Commands struct (above)
    uint32_t total_entries;
    uint32_t entries[64];
}CSI_struct_t;

volatile const CSI_struct_t* Main_CSI_struct = 52428800;

uint32_t* CSI_entries_ptr;
uint32_t* tot_entries;

volatile const uint32_t* CSI_mem_start = 52428800;
volatile bool key_pressed = false;
volatile int enter_pressed = 0;
volatile char* Istream_ptr;
volatile uint8_t* Current_buf;
volatile int kb_buff = 0;
volatile char* shell_buf;
volatile uint32_t shell_in = 0, Current_strlen = 0;
volatile uint32_t shell_awake = 1;
volatile uint32_t shell_sleeping = 0;
volatile uint32_t* Shell_Istream;
volatile uint32_t Input_stream;
volatile uint32_t Istream_end;

void init_shell();

void Shell_Double_buffer();

void Shell_Input();

void Shell_sleep();

void Shell();

void Shell_wakeup();

void vesa_test_dbuf();

int Shell_command_locator(char *inst);

void console_manager_init();

uint32_t* CSI_Read(uint32_t entry);

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name);

#endif
