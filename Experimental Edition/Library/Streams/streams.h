#ifndef STREAMS_H
#define STREAMS_H

#include "stdio.h"
#include "string.h"
#include "common.h"
#include "stdlib.h"

/*
    Circular Buffer #1 -->

    memory addresses->          1   2       3   4       5   6         n-2  n-1          n          n+1
    memory start->          [AOI][DATA] [AOI][DATA] [AOI][DATA] ... [AOI][DATA] [mem address #1][NOTHING]
    {AOI = Address of itself}
*/

typedef struct __attribute__((packed)) CircularSR_Buffer //Circular Self Refrencing Buffer
{
    uint32_t* addr;
    uint32_t data[];
}CircularSR_Buffer_t;

typedef struct __attribute__((packed)) CircularSRB_header // Custom CSRB Header for Memory Management
{
    uint32_t* curr_ptr;
    uint32_t* tail;
    uint32_t* head;
    uint32_t* real_head;
}CircularSRB_header_t;

#endif