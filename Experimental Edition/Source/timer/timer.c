#include "timer.h"
#include "console.h"
extern void timertest();
u32int tick = 0;

void timer_callback()
 {
    tick++;
    console_writestring("Tick: ");
    console_write_dec(tick);
 }
 /**REAL TIMER RTC, might not work**/
 void init_timer_RTC()
 {
    // asm volatile("cli");
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
    register_interrupt_handler(IRQ8,&timertest);
 }

 /**PIT TIMER, working**/
void init_timer(u32int frequency)
 {
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timertest);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    u32int divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
 }
