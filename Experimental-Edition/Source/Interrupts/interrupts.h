#ifndef INTERRUPT_h
#define INTERRUPT_h

#include "common.h"

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif
