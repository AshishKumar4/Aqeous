#include "fonts.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"

void make_B_Font()
{
  Character_t* B = (Character_t*)kmalloc((sizeof(Bridge_t)*10) + 12);
  char_B = B;
  B->length = 10;
  B->font_multiplier = 1;

  B->bridges[0].x1 = 0;
  B->bridges[0].x2 = 0;
  B->bridges[0].y1 = 0;
  B->bridges[0].y2 = 8;

  B->bridges[1].x1 = 0;
  B->bridges[1].x2 = 3;
  B->bridges[1].y1 = 0;
  B->bridges[1].y2 = 0;

  B->bridges[2].x1 = 3;
  B->bridges[2].x2 = 4;
  B->bridges[2].y1 = 0;
  B->bridges[2].y2 = 1;

  B->bridges[3].x1 = 4;
  B->bridges[3].x2 = 4;
  B->bridges[3].y1 = 1;
  B->bridges[3].y2 = 3;

  B->bridges[4].x1 = 4;
  B->bridges[4].x2 = 3;
  B->bridges[4].y1 = 3;
  B->bridges[4].y2 = 4;

  B->bridges[5].x1 = 3;
  B->bridges[5].x2 = 4;
  B->bridges[5].y1 = 4;
  B->bridges[5].y2 = 5;

  B->bridges[6].x1 = 4;
  B->bridges[6].x2 = 4;
  B->bridges[6].y1 = 5;
  B->bridges[6].y2 = 7;

  B->bridges[7].x1 = 4;
  B->bridges[7].x2 = 3;
  B->bridges[7].y1 = 7;
  B->bridges[7].y2 = 8;

  B->bridges[8].x1 = 0;
  B->bridges[8].x2 = 3;
  B->bridges[8].y1 = 4;
  B->bridges[8].y2 = 4;

  B->bridges[9].x1 = 0;
  B->bridges[9].x2 = 3;
  B->bridges[9].y1 = 8;
  B->bridges[9].y2 = 8;
}

void make_C_Font()
{
  Character_t* C = (Character_t*)kmalloc((sizeof(Bridge_t)*5) + 12);
  char_C = C;
  C->length = 5;
  C->font_multiplier = 1.6;

  C->bridges[0].x1 = 1;
  C->bridges[0].x2 = 3;
  C->bridges[0].y1 = 0;
  C->bridges[0].y2 = 0;

  C->bridges[1].x1 = 1;
  C->bridges[1].x2 = 0;
  C->bridges[1].y1 = 0;
  C->bridges[1].y2 = 1;

  C->bridges[2].x1 = 0;
  C->bridges[2].x2 = 0;
  C->bridges[2].y1 = 1;
  C->bridges[2].y2 = 4;

  C->bridges[3].x1 = 0;
  C->bridges[3].x2 = 1;
  C->bridges[3].y1 = 4;
  C->bridges[3].y2 = 5;

  C->bridges[4].x1 = 1;
  C->bridges[4].x2 = 3;
  C->bridges[4].y1 = 5;
  C->bridges[4].y2 = 5;
}

void make_E_Font()
{
  Character_t* E = (Character_t*)kmalloc((sizeof(Bridge_t)*4) + 12);
  char_E = E;
  E->length = 4;
  E->font_multiplier = 1.15;

  E->bridges[0].x1 = 0;
  E->bridges[0].x2 = 0;
  E->bridges[0].y1 = 0;
  E->bridges[0].y2 = 7;

  E->bridges[1].x1 = 0;
  E->bridges[1].x2 = 4;
  E->bridges[1].y1 = 0;
  E->bridges[1].y2 = 0;

  E->bridges[2].x1 = 0;
  E->bridges[2].x2 = 3;
  E->bridges[2].y1 = 4;
  E->bridges[2].y2 = 4;

  E->bridges[3].x1 = 0;
  E->bridges[3].x2 = 4;
  E->bridges[3].y1 = 7;
  E->bridges[3].y2 = 7;
}

void make_1_Font()
{
  Character_t* _1 = (Character_t*)kmalloc((sizeof(Bridge_t)*3) + 12);
  char_1 = _1;
  _1->length = 3;
  _1->font_multiplier = 1;

  _1->bridges[0].x1 = 0;
  _1->bridges[0].x2 = 1;
  _1->bridges[0].y1 = 1;
  _1->bridges[0].y2 = 0;

  _1->bridges[1].x1 = 1;
  _1->bridges[1].x2 = 1;
  _1->bridges[1].y1 = 0;
  _1->bridges[1].y2 = 8;

  _1->bridges[2].x1 = 0;
  _1->bridges[2].x2 = 2;
  _1->bridges[2].y1 = 8;
  _1->bridges[2].y2 = 8;

}

void make_2_Font()
{
  Character_t* _2 = (Character_t*)kmalloc((sizeof(Bridge_t)*6) + 12);
  char_2 = _2;
  _2->length = 6;
  _2->font_multiplier = 1.60;

  _2->bridges[0].x1 = 0;
  _2->bridges[0].x2 = 1;
  _2->bridges[0].y1 = 1;
  _2->bridges[0].y2 = 0;

  _2->bridges[1].x1 = 1;
  _2->bridges[1].x2 = 2;
  _2->bridges[1].y1 = 0;
  _2->bridges[1].y2 = 0;

  _2->bridges[2].x1 = 2;
  _2->bridges[2].x2 = 3;
  _2->bridges[2].y1 = 0;
  _2->bridges[2].y2 = 1;

  _2->bridges[3].x1 = 3;
  _2->bridges[3].x2 = 3;
  _2->bridges[3].y1 = 1;
  _2->bridges[3].y2 = 2;

  _2->bridges[4].x1 = 3;
  _2->bridges[4].x2 = 0;
  _2->bridges[4].y1 = 2;
  _2->bridges[4].y2 = 5;

  _2->bridges[5].x1 = 0;
  _2->bridges[5].x2 = 3;
  _2->bridges[5].y1 = 5;
  _2->bridges[5].y2 = 5;
}

void make_3_Font()
{
  Character_t* _3 = (Character_t*)kmalloc((sizeof(Bridge_t)*9) + 12);
  char_3 = _3;
  _3->length = 9;
  _3->font_multiplier = 1;

  _3->bridges[0].x1 = 0;
  _3->bridges[0].x2 = 3;
  _3->bridges[0].y1 = 0;
  _3->bridges[0].y2 = 0;

  _3->bridges[1].x1 = 3;
  _3->bridges[1].x2 = 4;
  _3->bridges[1].y1 = 0;
  _3->bridges[1].y2 = 1;

  _3->bridges[2].x1 = 4;
  _3->bridges[2].x2 = 4;
  _3->bridges[2].y1 = 1;
  _3->bridges[2].y2 = 3;

  _3->bridges[3].x1 = 4;
  _3->bridges[3].x2 = 3;
  _3->bridges[3].y1 = 3;
  _3->bridges[3].y2 = 4;

  _3->bridges[4].x1 = 3;
  _3->bridges[4].x2 = 4;
  _3->bridges[4].y1 = 4;
  _3->bridges[4].y2 = 5;

  _3->bridges[5].x1 = 4;
  _3->bridges[5].x2 = 4;
  _3->bridges[5].y1 = 5;
  _3->bridges[5].y2 = 7;

  _3->bridges[6].x1 = 4;
  _3->bridges[6].x2 = 3;
  _3->bridges[6].y1 = 7;
  _3->bridges[6].y2 = 8;

  _3->bridges[7].x1 = 1;
  _3->bridges[7].x2 = 3;
  _3->bridges[7].y1 = 4;
  _3->bridges[7].y2 = 4;

  _3->bridges[8].x1 = 0;
  _3->bridges[8].x2 = 3;
  _3->bridges[8].y1 = 8;
  _3->bridges[8].y2 = 8;
}

void make_4_Font()
{
  Character_t* _4 = (Character_t*)kmalloc((sizeof(Bridge_t)*3) + 12);
  char_4 = _4;
  _4->length = 3;
  _4->font_multiplier = 1.04;

  _4->bridges[0].x1 = 4;
  _4->bridges[0].x2 = 4;
  _4->bridges[0].y1 = 0;
  _4->bridges[0].y2 = 8;

  _4->bridges[1].x1 = 0;
  _4->bridges[1].x2 = 4;
  _4->bridges[1].y1 = 6;
  _4->bridges[1].y2 = 0;

  _4->bridges[2].x1 = 0;
  _4->bridges[2].x2 = 5;
  _4->bridges[2].y1 = 6;
  _4->bridges[2].y2 = 6;

}

void make_5_Font()
{
  Character_t* _5 = (Character_t*)kmalloc((sizeof(Bridge_t)*7) + 12);
  char_5 = _5;
  _5->length = 7;
  _5->font_multiplier = 0.9;

  _5->bridges[0].x1 = 0;
  _5->bridges[0].x2 = 4;
  _5->bridges[0].y1 = 0;
  _5->bridges[0].y2 = 0;

  _5->bridges[1].x1 = 0;
  _5->bridges[1].x2 = 0;
  _5->bridges[1].y1 = 0;
  _5->bridges[1].y2 = 4;

  _5->bridges[2].x1 = 0;
  _5->bridges[2].x2 = 2;
  _5->bridges[2].y1 = 4;
  _5->bridges[2].y2 = 4;

  _5->bridges[3].x1 = 2;
  _5->bridges[3].x2 = 4;
  _5->bridges[3].y1 = 4;
  _5->bridges[3].y2 = 5;

  _5->bridges[4].x1 = 4;
  _5->bridges[4].x2 = 4;
  _5->bridges[4].y1 = 5;
  _5->bridges[4].y2 = 8;

  _5->bridges[5].x1 = 4;
  _5->bridges[5].x2 = 3;
  _5->bridges[5].y1 = 8;
  _5->bridges[5].y2 = 9;

  _5->bridges[6].x1 = 3;
  _5->bridges[6].x2 = 0;
  _5->bridges[6].y1 = 9;
  _5->bridges[6].y2 = 9;
/*
  _5->bridges[2].x1 = 0;
  _5->bridges[2].x2 = 2;
  _5->bridges[2].y1 = 8;
  _5->bridges[2].y2 = 8;*/
}

void make_6_Font()
{
  Character_t* _6 = (Character_t*)kmalloc((sizeof(Bridge_t)*3) + 12);
//  char_6 = _6;
  _6->length = 3;
  _6->font_multiplier = 1;
/*
  _4->bridges[0].x1 = 0;
  _4->bridges[0].x2 = 1;
  _4->bridges[0].y1 = 1;
  _4->bridges[0].y2 = 0;

  _4->bridges[1].x1 = 1;
  _4->bridges[1].x2 = 1;
  _4->bridges[1].y1 = 0;
  _4->bridges[1].y2 = 8;

  _4->bridges[2].x1 = 0;
  _4->bridges[2].x2 = 2;
  _4->bridges[2].y1 = 8;
  _4->bridges[2].y2 = 8;
*/
}
/*
void make_ _Font()
{
  Character_t* _4 = (Character_t*)kmalloc((sizeof(Bridge_t)*3) + 12);
  char_4 = _4;
  _4->length = 3;
  _4->font_multiplier = 1;

  _4->bridges[0].x1 = 0;
  _4->bridges[0].x2 = 1;
  _4->bridges[0].y1 = 1;
  _4->bridges[0].y2 = 0;

  _4->bridges[1].x1 = 1;
  _4->bridges[1].x2 = 1;
  _4->bridges[1].y1 = 0;
  _4->bridges[1].y2 = 8;

  _4->bridges[2].x1 = 0;
  _4->bridges[2].x2 = 2;
  _4->bridges[2].y1 = 8;
  _4->bridges[2].y2 = 8;

}
*/
