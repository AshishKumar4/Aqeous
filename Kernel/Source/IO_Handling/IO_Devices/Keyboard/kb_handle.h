#ifndef KB_QUEUE_h
#define KB_QUEUE_h

#include "MultiTasking/tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "serials.h"
#include "std_iohandling.h"

volatile uint32_t kb_end = 0, kb_q_elements = 0;

volatile serials_i_struct_t* kb_Start_q, *kb_Current_q, *kb_Last_q;

void kb_io_init();
void kb_getline(char* str, uint32_t length);
void kb_stdin_syscall(char* str, uint32_t length);

#endif
