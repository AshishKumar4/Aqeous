#ifndef RAND_h
#define RAND_h
#include "stdint-gcc.h"
#include "cmos.h"
#include "RandomLib/random.h"

volatile unsigned long int next = 0;
uint32_t time;
uint32_t ticks=5;

int rand(void)
{
  return (int)random();
}

void srand(unsigned int seed)
{
  Randomizer();
  if(!seed2)
    seed2 = seed;
}

#endif
