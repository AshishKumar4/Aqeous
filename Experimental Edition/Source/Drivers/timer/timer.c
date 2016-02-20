#include "timer.h"
#include "console.h"
#include "sys.h"

uint32_t tick = 0;

void timer_idle_task()
{
  outb(0x20, 0x20);
  asm volatile("sti");
  asm volatile("iret");
}

uint32_t timer_task=(uint32_t)timer_idle_task;

void timer_stub()
{
  asm volatile("jmp *%0"::"r"(timer_task));
}

void timer_callback()
 {
    tick++;
 }
 /**REAL TIMER RTC, might not work**/
 void init_timer_RTC()
 {
     asm volatile("cli");
     /*outb(0x70, 0x8B);		// select register B, and disable NMI
     char prev=inb(0x71);	// read the current value of register B
     outb(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
     outb(0x71, prev | 0x40);*/
     int rate;
     rate=5;
     rate &= 0x0F;			// rate must be above 2 and not over 15

     outb(0x70, 0x8A);		// set index to register A, disable NMI
     char prev=inb(0x71);	// get initial value of register A
     outb(0x70, 0x8A);		// reset index to A
     outb(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.

     asm volatile("sti");
    //register_interrupt_handler(IRQ8,timer_task);
 }

 unsigned short masterPIC, slavePIC;

 void picInit(unsigned short _pic1, unsigned short _pic2)
 {
 	masterPIC = _pic1;
 	slavePIC  = _pic2;

 	unsigned short data1 = _pic1 + 1;
 	unsigned short data2 = _pic2 + 1;

 	outb(_pic1, 0x10 + 0x01);	// Init + ICW4 (Set up Cascade mode)
 	outb(_pic2, 0x10 + 0x01);

 	// After this init, the PICs are expecting three additional pieces
 	// of data; Vector offset, Cascade position, and mode.

 	outb(data1, 0x20);	// Relocate pic1 to interrupt 0x20
 	outb(data2, 0x28);	// Relocate pic2 to interrupt 0x28

 	outb(data1, 4);		// Tell Master it has Slave on line 2 (00000100)
 	outb(data2, 2);		// Tell the Slave it's cascading      (00000010)

 	outb(data1, 1);		// Tell the Pic it's in 8086 mode
 	outb(data2, 1);

 	// At this point, init is complete, and the pic goes into normal
 	// Operating mode. But there is still one last thing to do, Set the
 	// Interrupt Masks so we don't get interrupts until we set up the
 	// Devices on their respective IRQ Lines.

 	outb(data1, 0xFF);	// Lines 0-7 are all masked
 	outb(data2, 0xFF);	// Lines 8-15 are now masked too.
 }

 /**PIT TIMER, working**/
void init_timer(uint32_t frequency)
 {
    //Our PIT timer handler is already especially hard coded in asm in interrupts.s
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
 }
