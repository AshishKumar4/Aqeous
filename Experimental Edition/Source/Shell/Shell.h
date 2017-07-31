#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include <console.h>
#include <common.h>
#include <tasking.h>

//#define MAX_COMMANDS 100
//#define Output_stream 524288000
//#define Shell_Istream_start 577773568
//#define Shell_Istream_end 578822144


#define CSI_addr 52428800 //50th MB; CSI = Common Shell Interface; a Memory mapped area for programs to interact with the shell.


volatile uint32_t* Shell_Commands_list;
volatile uint32_t up_input = 0;
volatile uint32_t _ctrl_C_pressed = 1;
volatile uint32_t shell_harbor = 0;

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

volatile const CSI_struct_t* Main_CSI_struct = (CSI_struct_t*)52428800;

uint32_t* CSI_entries_ptr;
uint32_t* tot_entries;

volatile const uint32_t* CSI_mem_start = (volatile const uint32_t*)52428800;
volatile bool key_pressed = false;
volatile int enter_pressed = 0;
volatile char* Istream_ptr;
volatile uint8_t* Current_buf;
volatile int kb_buff = 0;
volatile char* shell_buf;
volatile uint32_t shell_in = 0, Current_strlen = 0;
volatile uint32_t shell_awake = 1;
volatile int shell_sleeping = 0;
volatile uint32_t* Shell_Istream;
volatile uint32_t Input_stream;
volatile uint32_t Istream_end;

volatile task_t* shell_cmdTask;

volatile uint32_t* tmpIstream;
volatile func_t shell_cmdFunc;

volatile  int shlock = 0;

DECLARE_LOCK(SHELL_CMDTASK);

void init_shell();

void Shell_Double_buffer();

void Shell_Dbuff_sync();

void Shell_Input();

void Shell_CFexecute(uint32_t* buffer, uint32_t sz);

void Shell_SFexecute(char* command);

void Shell_sleep();

void Shell();

void Shell_scrollUp();

void Shell_scrollDown();

void Shell_wakeup();

void vesa_test_dbuf();

void Shell_cmdWrapper();

int Shell_command_locator(char *inst);

int Shell_command_locator_CC(char *inst);

void console_manager_init();

uint32_t* CSI_Read(uint32_t entry);

uint32_t* CSI_ReadAPS(char* str); //Read As the Provided String!

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name);

#endif
