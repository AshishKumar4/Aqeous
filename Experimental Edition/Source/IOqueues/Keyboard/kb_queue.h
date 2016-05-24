#ifndef KB_QUEUE_h
#define KB_QUEUE_h

#include "tasking.h"
#include "mem.h"
#include "stdio.h"

uint32_t kb_end = 0, q_elements = 0;

typedef struct __attribute__((packed)) kb_queue
{
    task_t* task;
    uint32_t str_ln;
    uint32_t* buffer;
    struct kb_queue* next;
}kb_queue_t;

kb_queue_t* Start_q, Current_q, Last_q;

#endif
