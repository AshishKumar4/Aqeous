#include "common.h"
#include "interrupts.h"
#include "console.h"
#include "descriptors.c"

isr_t interrupt_handlers[256];

void setIRQMask(int _IRQ)
{
	unsigned short port;
	unsigned char  mask;

	if(_IRQ < 8) {
		port = 0x21;
	} else {
		port = 0xA1;
		_IRQ -= 8;
	}
	mask = inb(port) | (1 << _IRQ);
	outb(port, mask);
}

void clearIRQMask(int _IRQ)
{
	unsigned short port;
	unsigned char  mask;

	if(_IRQ < 8) {
		port = 0x21;
	} else {
		port = 0xA1;
		_IRQ -= 8;
	}
	mask = inb(port) & ~(1 << _IRQ);
	outb(port, mask);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

int slave=0;

void fault_handler(registers_t regs)
{
  int a=printf("\nUnhandled Interrupt %x\n",regs.int_no);
}

void int_init()
{
  for(int i=33;i<48;i++)
  {
      register_interrupt_handler(i,fault_handler);
  }
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    slave=0;
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no])
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    slave=0;
    if (regs.int_no>=40)
    {
        // Send reset signal to slave.
        slave=1;
    }
}

void eoi()
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (slave)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
    asm volatile("iret");
}
