#ifndef RANDOM_h
#define RANDOM_h

uint32_t buffer0[2];
uint32_t seed1 = 9;
uint32_t buffer1[2];
uint32_t seed2 = 8;
uint32_t buffer2[2];
uint32_t seed3 = 7;
uint32_t buffer3[2];

void Randomizer();

uint32_t random();

float RandomnessCalculator(int (*random_func)(), func_t random_init, uint32_t precision1, uint32_t precision2);

#endif
