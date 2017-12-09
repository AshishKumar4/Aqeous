#ifndef PIC_h
#define PIC_h

void setIRQMask(int _IRQ);

void clearIRQMask(int _IRQ);

void enable_pic();

void disable_pic();

#endif
