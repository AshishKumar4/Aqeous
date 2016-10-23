#include "string.h"
#include "stdio.h"
#include "fonts.h"
#include "graphics.h"
#include "math.h"
#include "common.h"
#include "fonts_typeface.c"

void font_maker()
{
  make_B_Font();
  make_C_Font();
  make_E_Font();
  make_1_Font();
  make_2_Font();
  make_3_Font();
  make_4_Font();
}

void font_renderer(Character_t* ch, uint32_t x, uint32_t y, uint32_t colour, uint32_t sz, uint32_t thickness)
{
  Bridge_t* tmp;
  tmp = ch->bridges;
  for(int i = 0; i < ch->length; i++,tmp++)
  {
    line_plot(x+(tmp->x1*sz*ch->font_multiplier),y+(tmp->y1*sz*ch->font_multiplier),x+(tmp->x2*sz*ch->font_multiplier),y+(tmp->y2*sz*ch->font_multiplier),colour, thickness);
    //RectD(100,300,50,10,7,7,7);
    //line_fast(x+(tmp->x1*sz*ch->font_multiplier)-thickness,y+(tmp->y1*sz*ch->font_multiplier)-thickness,x+(tmp->x2*sz*ch->font_multiplier)+thickness,y+(tmp->y2*sz*ch->font_multiplier)+thickness,colour, thickness); //GREAT FONT

  }
  //line_plot(100,100,200,500,colour,sz);
}
