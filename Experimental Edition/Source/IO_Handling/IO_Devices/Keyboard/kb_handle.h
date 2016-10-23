#ifndef KB_QUEUE_h
#define KB_QUEUE_h

#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdio.h"
#include "serials.h"
#include "std_iohandling.h"

volatile uint32_t kb_end = 0, kb_q_elements = 0;
/*
typedef struct __attribute__((packed)) kb_queue
{
    task_t* task;
    uint32_t str_ln;
    uint32_t* buffer;
    struct kb_queue* next;
}kb_queue_t;
*/
/*
typedef struct __attribute__((packed)) kb_i_handle
{
}kb_i_handle_t;

typedef struct __attribute__((packed)) kb_o_handle
{
}kb_o_handle_t;
*/
volatile serials_i_struct_t* kb_Start_q, *kb_Current_q, *kb_Last_q;

void kb_io_init();
void kb_getline(char* str, uint32_t length);

#endif
