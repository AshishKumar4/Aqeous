#ifndef RAND_h
#define RAND_h
#include "stdint-gcc.h"
#include "cmos.h"

volatile unsigned long int next = 0;
uint32_t time;
uint32_t ticks=5;

inline int rand() // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return ((next / 65536) % 32768);
}

inline void srand( unsigned int seed )
{
    //ticks=(next%5)+5;
    next = (next+seed)%100000;
}

inline void srandInit()
{
    time=ReadFromCMOS();
    srand(time);
}

int seeder()
{
    //rn=(int*)kmalloc(sizeof(int));
    int r=rand();
   // r+=*rn;
   // kfree((void*)rn);

    return r;
}

#endif
