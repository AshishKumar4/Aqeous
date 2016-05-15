
#ifndef TIMER_H
#define TIMER_H

#include "common.h"
#include "interrupts.h"

uint32_t tick = 0;

void init_timer();

void delay1(uint32_t ms);
void apic_start_timer();

#endif
