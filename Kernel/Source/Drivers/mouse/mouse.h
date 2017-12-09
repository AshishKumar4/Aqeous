#ifndef MOUSE_h
#define MOUSE_h

#include <common.h>

volatile int mousex=512,mousey=384;
volatile int mouse_left_click = 0;
volatile int mouse_right_click = 0;

volatile int mousedeltax=0,mousedeltay=0;
volatile static char mouse_bytes[3];
static uint32_t cycle = 0;
uint8_t* mouse_buff = 0;
int mouse_lclick_duration = 0;

void mouse_wait(unsigned char type);

void mouse_write(unsigned char a_write);

unsigned char mouse_read();

void mouse_updater();

#endif
