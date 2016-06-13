#include "descriptors.h"
#include "stdio.h"
#include "string.h"
#include "apic.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "paging.h"
#include "shell.h"
#include "tasking.h"
#include "console.h"

uint32_t* LAPIC_EOI_send = (uint32_t*)(0x00b0+0xfee00000);
extern void switcher();

void divByZero_handler()
{
  printf("\nFault1");
  while(1);
  asm volatile("iret");
}


void debug_handler()
{
  printf("\nFault2");
  while(1);
  asm volatile("iret");
}


void NMI_handler()
{
  printf("\nFault3");
  while(1);
  asm volatile("iret");
}


void breakpoint_handler()
{
  printf("\nFault4");
  while(1);
  asm volatile("iret");
}


void overflow_handler()
{
  printf("\nFault5");
  while(1);
  asm volatile("iret");
}


void outOfBounds_handler()
{
  printf("\nFault6");
  while(1);
  asm volatile("iret");
}


void invalidInstr_handler()
{
  printf("\nFault7");
  while(1);
  asm volatile("iret");
}


void noCoprocessor_handler()
{
  printf("\nFault8");
  while(1);
  asm volatile("iret");
}


void doubleFault_handler()
{
  printf("\nFault9");
  while(1);
  asm volatile("iret");
}


void coprocessor_handler()
{
  printf("\nFault10");
  while(1);
  asm volatile("iret");
}


void badTSS_handler()
{
  printf("\nFault11");
  while(1);
  asm volatile("iret");
}


void segmentNotPresent_handler()
{
  printf("\nFault12");
  while(1);
  asm volatile("iret");
}


void stackFault_handler()
{
  printf("\nFault13");
  while(1);
  asm volatile("iret");
}


void generalProtectionFault_handler(registers_t regs)
{
  asm volatile("cli");
  printf("\nFaul14t ticks: %x Ax%x Bx%x %x %x",tick,regs.err_code,regs.eip,regs.cs,regs.eflags);
  while(1);
  asm volatile("iret");
}

void pageFault_handler(registers_t regs)
{
  asm volatile("cli");
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    console_writestring("\nPage fault! ( ");
    if (present)
    {
      console_writestring("present, Allocating page for it ");
      //MapPage((void*)faulting_address,(void*)faulting_address);
    }
    if (rw)
    {
        console_writestring("read-only ");
    }
    if (us) {console_writestring("user-mode ");}
    if (reserved) {console_writestring("reserved ");}
    if (id) {console_writestring("id "); console_write_dec(id);}
    console_writestring(") at 0x");
    console_write_dec(faulting_address);
    console_writestring(" - EIP: ");
    console_write_dec(regs.eip);
    console_writestring("\n");
while(1);
    asm volatile("sti");
    asm volatile("iret");
   // PANIC("Page fault");
}

void unknownInterrupt_handler()
{
  printf("\nFaul15t");
  while(1);
  asm volatile("iret");
}


void coprocessorFault_handler()
{
  printf("\nFault16");
  while(1);
  asm volatile("iret");
}


void alignmentCheck_handler()
{
  printf("\nFault17");
  while(1);
  asm volatile("iret");
}


void machineCheck_handler()
{
  printf("\nFault18");
  while(1);
  asm volatile("iret");
}


void reserved_handler()
{
  printf("\nFault19");
  while(1);
  asm volatile("iret");
}



//#ifdef PIC
void PIT_handler()
{
  asm volatile("cli");
  outb(0x20,0x20);
  asm volatile("iret");
}

void keyboardInterrupt_handler()
{
  Switch_to_system_dir();
  if(kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF)
  {
    int scancode=inb(0x60);
		if (scancode == 0xE0 || scancode == 0xE1)
      printf("extended ");
    else
    {
      if(scancode&0x80) //break code
      {
        scancode-=0x80; //make it make code
        int key=scancodes[scancode];
    		//! test if a special key has been released & set it
    		switch (key)
        {
    					case KEY_LCTRL:
    					case KEY_RCTRL:
    						_ctrl = 0;
    						break;

    					case KEY_LSHIFT:
    					case KEY_RSHIFT:
    						_shift = 0;
    						break;

    					case KEY_LALT:
    					case KEY_RALT:
    						_alt = 0;
    						break;
    			}
      }
      else //it is a make code
      {
        int key=scancodes[scancode];
        //printint(scancode);
        //! test if a special key has been released & set it
    		switch (key)
        {

    					case KEY_LCTRL:
                _ctrl = 1;
                break;
    					case KEY_RCTRL:
    						_ctrl = 2;
    						break;

    					case KEY_LSHIFT:
                _shift = 1;
                break;
    					case KEY_RSHIFT:
    						_shift = 2;
    						break;

    					case KEY_LALT:
                _alt = 1;
                break;
    					case KEY_RALT:
    						_alt = 2;
    						break;

    					case KEY_CAPSLOCK:
    						_capslock = (_capslock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;

    					case KEY_KP_NUMLOCK:
    						_numlock = (_numlock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;

    					case KEY_SCROLLLOCK:
    						_scrolllock = (_scrolllock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;
              default:
                call = keyboard_scancodes(key);
                if(call == '\r')
                {
                  enter_pressed = 1;
                }
                else if(call == '\b') //TODO: Backspace is hit!
                {
                  if(kb_buff)
                  {
                    --Istream_ptr;
                    *Istream_ptr = 0;
                    --kb_buff;
                    backspace();
                  }
                }
                else
                {
                  *(Istream_ptr) = call;
                  ++Istream_ptr;
                  ++kb_buff;
                  if((uint32_t)Istream_ptr == Istream_end)
                    Istream_ptr = (uint32_t*)Input_stream;   //Reset
                  _putchar((int)call);
                }
          }
        }
      }
  }
  Switch_back_from_System();
}

void cascade_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 2");
  while(1);
  asm volatile("iret");
}

//This particular interrupt is never raised
void COM2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 3");
  while(1);
  asm volatile("iret");
}


void COM1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 4");
  while(1);
  asm volatile("iret");
}


void LPT2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 5");
  while(1);
  asm volatile("iret");
}


void floppyDisk_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 6");
  while(1);
  asm volatile("iret");
}


void LPT1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 7");
  while(1);
  asm volatile("iret");
}


void RTC_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 8");
  while(1);
  asm volatile("iret");
}


void periph1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 9");
  while(1);
  asm volatile("iret");
}


void periph2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 10");
  while(1);
  asm volatile("iret");
}


void periph3_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 11");
  while(1);
  asm volatile("iret");
}


void mouse_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 12");
  while(1);
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
  asm volatile("sti");
}


void FPU_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 13");
  while(1);
  asm volatile("iret");
}


void primaryHDD_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 14");
  while(1);
  asm volatile("iret");
}

//P -> Primary
void secondaryHDD_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 15");
  while(1);
  asm volatile("iret");
}

//S -> Secondary
