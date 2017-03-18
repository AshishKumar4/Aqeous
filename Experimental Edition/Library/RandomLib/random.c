#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "common.h"
#include "mathex.h"
#include "timer.h"
#include "hpet.h"
#include "APIC/LocalAPIC/lapic.h"
#include "math.h"

void Randomizer()
{
  seed1 = ReadFromCMOS() + 9;
  seed2 = (*HPET_main_counter + 1)*3;
  seed3 = Lapic->CurrentCount_Reg[0] + 999;
}

uint32_t random()
{/*
  uint32_t  random = (seed2/seed3)*97 + 1023*seed2;
  random *= (seed1*seed1*524287);
  random /= 1023;
  random += ((seed2*(random%3))*(seed1*(random%2)))*(random%5);
  seed1 = (seed1 + seed2*seed3 + 1)%127 + (seed2%100) + 1;
  seed2 = (uint32_t)(seed2 + random*178481)/3;
  seed3 = ((seed3 + seed2*seed1)/((seed1/57)+3))%8191 + seed1;  /*
  seed1 = (seed2 % 100) + 1;
  seed2 = (uint32_t)(seed2*178481);
  seed3 = ((seed3 + random/seed1)%524287) + 1; //*/
  //printf("S1: %x S2: %x S3: %x\n", seed1, seed2, seed3);
  double r = seed1*1023*97 + seed3*255*19 + seed2;
  r /= (seed3+3)*3;
  uint32_t random = (uint32_t)(r);
  seed2 += 1023*(((*HPET_main_counter + 1)*178481)%(seed1*seed3));
  seed1 += random%(seed1+99);
  seed1 %= 127;
  seed1 += 3;
  seed3 += (seed2+random)%(seed3 + 1023);
  seed3 %= 65536;
  seed3 += 127;

  return random;
}

/*

inline uint32_t random()
{
  uint32_t random = (seed2/seed3) + 1023*seed2;
  random *= (uint32_t)(seed1*seed1*524287);
  random /= 1023;
  random += ((seed2*(random%3))*(seed1*(random%2)))*(random%5);
  seed1 = (seed1 + seed2*seed3 + 1)%127 + (seed2%100) + 1;
  seed2 = (uint32_t)(seed2 + random*178481);
  seed3 = ((seed3 + seed2*seed1)/((seed1/57)+3))%8192 + seed1;
  seed1 = (seed2 % 100) + 1;
  seed2 = (uint32_t)(seed2*178481);
  seed3 = ((seed3 + random/seed1)%524287) + 1;
  //printf("S1: %x S2: %x S3: %x\n", seed1, seed2, seed3);
  return random;
}

*/

float RandomnessCalculator(int (*random_func)(), func_t random_init, uint32_t precision1, uint32_t precision2)
{
  random_init();
  int j;
  int cofactor = 0, coprime = 0;
  int a = 0, b = 0;
  for(int i = 0; i < precision1; i++)
  {
    a = random_func()%precision2;
    b = random_func()%precision2;
    if(!coprimes(a,b)) ++coprime;
    else ++cofactor;
  }
  printf("\nCoPrimes: %d, CoFactors: %d", coprime, cofactor);

  float pr = coprime, tf = cofactor;
  pr =  ((tf+pr)*6)/pr;
  pr = sqrt(pr);

  printf("\nCalculated: %f", pr);

  float error = 3.14159265359 - pr;
  if(error < 0) error = -error;
  printf("\nError: %f", error);
  float percentage = error/3.1415926;
  percentage *= 100;
  printf("\nError Percentage: %f", percentage);

  return error;
}
