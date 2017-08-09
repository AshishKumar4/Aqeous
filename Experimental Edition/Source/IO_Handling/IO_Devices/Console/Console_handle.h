#ifndef CONSOLE_HANDLE_H
#define CONSOLE_HANDLE_H

#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "serials.h"
#include "std_iohandling.h"

volatile uint32_t console_end = 0, console_q_elements = 0;
volatile serials_o_struct_t* console_Start_q, *console_Current_q, *console_Last_q;

void console_io_init();
void _stdprint(const char* str, uint32_t length);

#endif
