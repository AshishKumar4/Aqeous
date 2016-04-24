#include "descriptors.h"
#include "stdio.h"
#include "string.h"
#include "keyboard.h"
#include "mouse.h"

void divByZero_handler()
{
  asm volatile("iret");
}


void debug_handler()
{
  asm volatile("iret");
}


void NMI_handler()
{
  asm volatile("iret");
}


void breakpoint_handler()
{
  asm volatile("iret");
}


void overflow_handler()
{
  asm volatile("iret");
}


void outOfBounds_handler()
{
  asm volatile("iret");
}


void invalidInstr_handler()
{
  asm volatile("iret");
}


void noCoprocessor_handler()
{
  asm volatile("iret");
}


void doubleFault_handler()
{
  asm volatile("iret");
}


void coprocessor_handler()
{
  asm volatile("iret");
}


void badTSS_handler()
{
  asm volatile("iret");
}


void segmentNotPresent_handler()
{
  asm volatile("iret");
}


void stackFault_handler()
{
  asm volatile("iret");
}


void generalProtectionFault_handler()
{
  asm volatile("iret");
}


void pageFault_handler()
{
  asm volatile("sti");
  uint32_t faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
  printf("\npage fault");
  while(1);
  asm volatile("iret");
}


void unknownInterrupt_handler()
{
  asm volatile("iret");
}


void coprocessorFault_handler()
{
  asm volatile("iret");
}


void alignmentCheck_handler()
{
  asm volatile("iret");
}


void machineCheck_handler()
{
  asm volatile("iret");
}


void reserved_handler()
{
  asm volatile("iret");
}



//#ifdef PIC
void PIT_handler()
{
  asm volatile("iret");
}


void keyboardInterrupt_handler()
{
  asm volatile("cli");
  printf("\nabc");
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
              call=key;
    			}
        }
      }
  }
  asm volatile("iret");
}


void cascade_handler()
{
  asm volatile("iret");
}

//This particular interrupt is never raised
void COM2_handler()
{
  asm volatile("iret");
}


void COM1_handler()
{
  asm volatile("iret");
}


void LPT2_handler()
{
  asm volatile("iret");
}


void floppyDisk_handler()
{
  asm volatile("iret");
}


void LPT1_handler()
{
  asm volatile("iret");
}


void RTC_handler()
{
  asm volatile("iret");
}


void periph1_handler()
{
  asm volatile("iret");
}


void periph2_handler()
{
  asm volatile("iret");
}


void periph3_handler()
{
  asm volatile("iret");
}


void mouse_handler()
{
  asm volatile("cli");
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
  asm volatile("iret");
}


void primaryHDD_handler()
{
  asm volatile("iret");
}

//P -> Primary
void secondaryHDD_handler()
{
  asm volatile("iret");
}

//S -> Secondary
