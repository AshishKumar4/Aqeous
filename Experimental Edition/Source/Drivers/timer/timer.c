#include "timer.h"
#include "console.h"
#include "sys.h"


void timer_idle_task()
{
  outb(0x20, 0x20);
  asm volatile("sti");
  asm volatile("iret");
}

uint32_t timer_task=(uint32_t)timer_idle_task;

void timer_stub()
{
  //asm volatile("jmp *%0"::"r"(timer_task));
  ++tick;
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

 void apic_start_timer()
 {
        // Tell APIC timer to use divider 16
        localapic_write(LAPIC_TDCR, 0x3);

        // Prepare the PIT to sleep for 10ms (10000µs)
        //pit_prepare_sleep(10000);

        // Set APIC init counter to -1
        localapic_write(LAPIC_TICR, 0xFFFFFFFF);

        // Perform PIT-supported sleep
        //pit_perform_sleep();
      //  for(int i=0; i<1024*1024*1024*3; i++);
        // Stop the APIC timer
        localapic_write(LAPIC_TIMER, 0x10000);

        // Now we know how often the APIC timer has ticked in 10ms
      //  uint32_t ticksIn10ms = 0xFFFFFFFF - localapic_read(LAPIC_TCCR);

        // Start timer as periodic on IRQ 0, divider 16, with the number of ticks we counted
        localapic_write(LAPIC_TIMER, 32 | 0x20000);
        localapic_write(LAPIC_TDCR, 0x3);
        localapic_write(LAPIC_TICR, 1000);
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
