#ifndef KB_QUEUE_h
#define KB_QUEUE_h

#include "tasking.h"
#include "mem.h"
#include "stdio.h"

volatile uint32_t kb_end = 0, q_elements = 0;

typedef struct __attribute__((packed)) kb_queue
{
    task_t* task;
    uint32_t str_ln;
    uint32_t* buffer;
    struct kb_queue* next;
}kb_queue_t;

volatile kb_queue_t* Start_q, *Current_q, *Last_q;

void kb_io_init();
void kb_getline(char* str, uint32_t length);

#endif
