#ifndef FONTS_H
#define FONTS_H

#include "common.h"

typedef struct __attribute__((packed)) Bridge
{
  uint32_t x1,y1;
  uint32_t x2,y2;
  uint32_t type;
  uint32_t contour;
  //intfunc2_t func;
}Bridge_t;

typedef struct __attribute__((packed)) Character
{
  uint32_t length;
  double font_multiplier;
  Bridge_t bridges[];
}Character_t;

Character_t* char_B, *char_C, *char_E, *char_1, *char_2, *char_3, *char_4, *char_5;

void font_maker();

void font_renderer(Character_t* ch, uint32_t x, uint32_t y, uint32_t colour, uint32_t sz, uint32_t thickness);

#endif
