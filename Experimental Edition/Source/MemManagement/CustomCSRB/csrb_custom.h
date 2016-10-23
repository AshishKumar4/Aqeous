#ifndef CUSTOMCSRB_h
#define CUSTOMCSRB_h

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

typedef struct __attribute__((packed)) CustomCSRB_M //Custom Circular Self Refrencing Buffer FOR memory management
{
    uint32_t* addr;
    uint32_t begin;
    uint32_t size;
    uint32_t reserved;
}CustomCSRB_M_t;

typedef struct __attribute__((packed)) CustomCSRB_M_header // Custom CSRB Header for Memory Management
{
    uint32_t entries;
    uint32_t other;
    uint32_t* tail;
    uint32_t* head;
}CustomCSRB_M_header_t;

#endif