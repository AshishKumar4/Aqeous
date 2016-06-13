#include "pic.h"
#include "cpuid.h"
#include "sys.h"
#include "common.h"

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

void enable_pic()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void disable_pic()
{
    /* Set OCW1 (interrupt masks) */
    outb(0x21, 0xff);
    outb(0xa1, 0xff);
}
