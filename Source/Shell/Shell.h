#ifndef SHELL_h
#define SHELL_h

#include <string.h>
#include <stdio.h>
#include <console.h>
#include <common.h>

//#define MAX_COMMANDS 100
#define Input_stream 576716800
#define Output_stream 524288000
#define Istream_end 576733184

volatile bool key_pressed = false;
volatile int enter_pressed = 0;
volatile char* Istream_ptr = (char*)Input_stream;
volatile uint8_t* Current_buf;
volatile int kb_buff = 0;
volatile char* shell_buf;
volatile uint32_t shell_in = 0, Current_strlen = 0;
volatile bool shell_awake = true;

void init_shell();

void Console_Writer();

void Shell_Input();

void Shell_sleep();

void Shell();

void Shell_wakeup();

void vesa_test_dbuf();

#endif
