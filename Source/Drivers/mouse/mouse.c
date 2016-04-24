#include <mouse.h>
#include <graphics.h>
#include <common.h>
#include <sys.h>
#include <interrupts.h>

void mouse_wait(unsigned char type)
{
  unsigned int _time_out=100000;
  if(type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(unsigned char a_write)
 {
 //Wait to be able to send a command
 mouse_wait(1);
 //Tell the mouse we are sending a command
 outb(0x64, 0xD4);
 //Wait for the final part
 mouse_wait(1);
 //Finally write
 outb(0x60, a_write);
 }
 unsigned char mouse_read()
 {
 //Get response from mouse
 mouse_wait(0);
 return inb(0x60);
 }

 extern void mouse_handler(); //defined in int_handlers.c

 void mouseinit()
{
    mouse_wait(1);
    outb(0x64,0xA8);
    mouse_wait(1);
    outb(0x64,0x20);
    unsigned char status_byte;
    mouse_wait(0);
    status_byte = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status_byte);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}
