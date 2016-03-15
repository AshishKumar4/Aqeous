
#include <graphics.c>
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
 void mouse_handler();
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
    register_interrupt_handler(IRQ12,&mouse_handler);
}

int mousex=100,mousey=100;

 void mouse_handler()
{
  //asm volatile("cli");
  static unsigned char cycle = 0;
  static char mouse_bytes[3];
  while(cycle<3)
  mouse_bytes[cycle++] = inb(0x60);
  int deltax=0,deltay=0;
  if (cycle == 3) { // if we have all the 3 bytes...
    cycle = 0; // reset the counter
    // do what you wish with the bytes, this is just a sample
    if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40))
      return; // the mouse only sends information about overflowing, do not care about it and return
    if (!(mouse_bytes[0] & 0x20))
      deltay |= 0xFFFFFF00; //delta-y is a negative value
    if (!(mouse_bytes[0] & 0x10))
      deltax |= 0xFFFFFF00; //delta-x is a negative value
   // if (mouse_bytes[0] & 0x4)
      //RectD(100,100,50,50,000,100,1000); //A Mouse button click
    //if (mouse_bytes[0] & 0x2)
      //RectD(100,100,100,50,1000,1000,1000); //Another Mouse Button Click
   // if (mouse_bytes[0] & 0x1)
      //RectD(100,100,50,100,1000,90,2000);  //Another Mouse Button Clicked
    /*if(mouse_bytes[1]>=1||mouse_bytes>=1)
        RectD(10,10,50,50,1000,1000,90);*/
    deltax=mouse_bytes[1]/2;
    deltay=mouse_bytes[2]/2;
    mousex+=(deltax);
    mousey-=(deltay);
    //asm volatile("sti");
   // WriteText(mouse_bytes[0],100,200,1000,0);
  }
}
