#include <vesa.h>
#include <stddef.h>
#include <stdint.h>
#include <graphics.h>
#include <common.h>
#include <math.h>
#include <string.h>
#include <mouse.h>

void Init()
{
    //RectL(0,0,1022,767,90,90,90);
}

//void Clear()
//{
 //   for(int i=0;i<(1020*(depthVESA/8)+760*(widthVESA*(depthVESA/8)));i++)
  //      vga_mem[i]=NULL;
//}

void __attribute__((optimize("O0"))) *custom_memcpy(void * _dest, const void *_src, uint32_t _n)
{
    uint_fast32_t *sp = (uint_fast32_t*) _src;
    uint_fast32_t *dp = (uint_fast32_t*) _dest;
	//	_n /= sizeof(uint_fast32_t);
    for(; _n != 0; _n--) {
		*dp++ = *sp++;
	}
	return _dest;
}

inline void refresh_area(int x0, int y0, int x1, int y1)
{
  cx0 = MIN(cx0,x0);
  cx1 = MAX(cx1,x1);
  cy0 = MIN(cy0,y0);
  cy1 = MAX(cy1,y1);
}


inline void Pixel_VESA_BUFF(int x, int y, int C)
{
  /*calculates the offset for a specific point on screen*/
    int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));
/*
    buff[offset]=C1& 0xff;
    buff[offset+1]=C2& 0xff;
    buff[offset+2]=C3& 0xff;
    buff[offset+3]=C3& 0xff;
*/
    refresh_area(x-1,y-1,x+1,y+1);
    //buff[offset] = C;
    *((uint16_t*)&buff[offset]) = C;//(C&0xffffff00);//|((*((uint32_t*)&buff[offset]))&0x000000ff);
}


inline void Pixel_VESA_BUFF_M(int x, int y, int C, char* cb)
{
  /*calculates the offset for a specific point on screen*/
    int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));
    *((uint32_t*)&cb[offset]) = C;

    refresh_area(x-1,y-1,x+1,y+1);
}

inline void Pixel_VESA(int x, int y, int C)
{
  /*calculates the offset for a specific point on screen*/
    offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));
    *((uint32_t*)&vesa_buff[offset]) = C;
//    *((uint32_t*)&buff[offset]) = C;
    refresh_area(x-1,y-1,x+1,y+1);
}

void RectD(int x, int y, int width, int height, int C)
{
  int offset;
  for(int i = y; i < height + y; i++)
  {
      for(int j = x; j < width + x; j++)
    {
      offset = j * (depthVESA / 8) + i * (widthVESA * (depthVESA / 8));
      *((uint32_t*)&buff[offset]) = C;
    }
  }
  refresh_area(x,y,x+width,y+height);
}

extern void mouse_handler();

uint32_t xa = 0,xb=0;
unsigned char buffer = 90;

void memcpy_rep2(uint32_t s, uint32_t d, uint32_t sz)
{
  asm volatile("mov %0, %%eax;\
                mov %%eax, %%edi;\
                mov %1, %%eax;\
                mov %%eax, %%esi;\
                mov %2, %%ecx;\
                cld;\
                rep movsd;"::"r"(d), "r"(s), "r"(sz));
}

void __attribute__((optimize("O0"))) DBuff()
{
  //const uint32_t szn = (1024*768);
  uint_fast32_t *sp = (uint_fast32_t*) buff;
  uint_fast32_t *dp = (uint_fast32_t*) vga_mem;
  //uint_fast32_t *mp = (uint_fast32_t*) mouse_buff;
  uint32_t offset = 0;
  uint32_t dv = depthVESA/8;
  Enable_SSE();
  while(1)
  {
    asm volatile("cli");
    /*
    for(uint32_t _n = szn; _n != 0; _n--)
    {
      *dp++ = (*sp++)*(!*mp++) + *mp;
    //  mp++;
  }//*/
//   /*
    offset = (cx0*dv + (cy0*dv*widthVESA))/4;
    cx0/=2;
    cx1/=2;
    /*
    cy0*=2;
    cy1*=2;*/
    sp = (uint_fast32_t*) buff + offset;
    dp = (uint_fast32_t*) vga_mem + offset;
    //mp = (uint_fast32_t*) mouse_buff + offset;
    /*
    for(uint32_t _i = cy0; _i < cy1; _i++)
    {
      for(uint32_t _j = cx0; _j < cx1; _j++)
      {
        *dp++ = *sp++*!*mp + *mp++;
      //  *mp++ = 0;
      }
      dp += 512 - (cx1-cx0);
      sp += 512 - (cx1-cx0);
      mp += 512 - (cx1-cx0);
    }*/
    //a++;
    //Pixel_VESA(a,768-a,20);

    for(uint32_t _i = (cy1 - cy0); _i != 0; _i--)
    {
      asm volatile("mov %%eax, %%edi;\
                    mov %%ebx, %%esi;\
                    rep movsd;"::"a"(dp), "b"(sp), "c"(cx1-cx0));
      dp += 512;
      sp += 512;
    //  mp += 512;
    }
    cx0 = 512;
    cx1 = 512;
    cy0 = 384;
    cy1 = 384;
    //memcpy_rep(dp,sp,1024*768);
    asm volatile("int $50");
  }
}

void RectL(int x, int y, int width, int height, int C)
{
  for(int i = y; i < height + y; i++)
    for(int j = x; j < width + x; j++)
      Pixel_VESA(j, i, C);
}

void Creater(int i,int j)
{
    Pixel_VESA(i,i+j,1000);
}
/*
void Func_Plot(int x1, int y1, int x2, int y2, int C, int sz, intfunc2_t func)
{
    uint32_t slope = 1;
  //  slope = ((MAX(x1,x2) - MIN(x1,x2))/(MAX(y1,y2)-MIN(y1,y2)));
    for(int x_t = MIN(x1, x2); x_t <= MAX(x1, x2); x_t++)
    {
      for(int y_t = MIN(y1, y2); y_t <= MAX(y1, y2); y_t++)
      {
        if((y_t - MIN(y1, y2)) == (x_t - MIN(x1, x2)))
        {
          Pixel_VESA(x_t, y_t, C);
        }
      }
    }
}

*/
void line_plot(int x0, int y0, int x1, int y1, int color, int sz)
{
  //    y1 = (SCREEN_HEIGHT)-y1;
  //    y0 = (SCREEN_HEIGHT)-y0;

      int deltax = x1-x0;
      int deltay = y1-y0;

      int y=0;
      int x=0;

      int sdx = (deltax < 0) ? -1 : 1; // sign of deltax (e.g. +ve or -ve)
      int sdy = (deltay < 0) ? -1 : 1;
      // sdx is the line direction... for x or y... e.g. if the x value is going left to
      // right or right to left... e.g
      // if deltax >0  ...then of course sdx=1
      // so our line is going from left to right    ------>x +ve
      // and alternatively
      // if deltax <0 ... we get sdx= -1;
      // and our line is     x<-------     -ve

      // We only want our delta's to be positive....keep with positive numbers of
      // incrment...so using our sdx/sdy value we set our delta values to +ve numbers
      deltax = sdx * deltax + 1;
      deltay = sdy * deltay + 1; // (-1*-6)+1) = 7

      int px = x0; // starting point (x0,y0)
      int py = y0;

      //float ynum = deltax/2; // num stands for the numerator starting value

      if( deltax >= deltay )
      {
            for (x = 0; x < deltax; x++)
            {

                  Pixel_VESA_BUFF( px, py,  color);
                  for(int j = 1; j<sz; j++)
                  {
                    Pixel_VESA_BUFF( px, py+j,  color);
                    Pixel_VESA_BUFF( px, py-j,  color);
                  }
                  y += deltay;     // y=mx... but if y starts at y0
                                   // then we can do y+=m
                  if (y >= deltax) // m=deltax/deltay  and py+=m
                  {            // if the numberator is greator than the denomiator
                        y -= deltax; // we increment, and subract from the numerator.
                        py += sdy;
                  }
                  px += sdx; // x is going from x0 to x1... we just increment as we
                             // move along
            }
      }
      else
      {
            for( y=0; y<deltay; y++)
            {
                  for(int j = 1; j<sz; j++)
                  {
                    Pixel_VESA_BUFF( px + j, py,  color);
                    Pixel_VESA_BUFF( px - j, py,  color);
                  }
                  Pixel_VESA_BUFF( px, py,  color);
                  x+=deltax;
                  if( x >= deltay )
                  {
                        x-= deltay;
                        px+=sdx;
                  }
                  py+=sdy;
            }
      }
}

void line_fast(int x1, int y1, int x2, int y2, int color, int thickness)
{
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

  //VGA[(py<<8)+(py<<6)+px]=color;
  //Pixel_VESA_BUFF( px, py,  color);

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      Pixel_VESA_BUFF( px, py,  color);
      for(int j = 1; j<thickness; j++)
      {
        Pixel_VESA_BUFF( px, py+j,  color);
        Pixel_VESA_BUFF( px, py-j,  color);
      }
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;

      for(int j = 1; j<thickness; j++)
      {
        Pixel_VESA_BUFF( px + j, py,  color);
        Pixel_VESA_BUFF( px - j, py,  color);
      }
      Pixel_VESA_BUFF( px, py,  color);
    }
  }
}

/*
void draw_c_curve (plPlotter *plotter, double dx, double dy, int order)
{
  if (order >= MAXORDER)
    // continue path along (dx, dy)
    pl_fcontrel_r (plotter, dx, dy);
  else
    {
      draw_c_curve (plotter,
                    0.5 * (dx - dy), 0.5 * (dx + dy), order + 1);
      draw_c_curve (plotter,
                    0.5 * (dx + dy), 0.5 * (dy - dx), order + 1);
    }
}

*/
void Mouse_Plot()
{
  int offset = 0;
  int dv = depthVESA/8;
  uint16_t* tmp = (uint16_t*)mouse_buff;
  //uint32_t* tmp32 = mouse_buff;
  uint16_t* mbf = (uint16_t*)buff;
  //uint32_t* mbf32 = buff;
  while(1)
  {
    mouse_handler();
    asm volatile("int $50");
    asm volatile("cli");
/*
    for(int i=mousey + 20;i > mousey - 6 ;i--)
    {
        offset = (mousex-7) * dv + i * widthVESA * dv;
        tmp32 = mouse_buff + offset;
        mbf32 = buff + offset;
      //  if(i < 13 + mousey)
        for (int j=0;j < i-mousey + 5;j++)
        {
        //   *tmp32++=0;
           *mbf32++=*tmp32++;
           //offset += dv;
        }
    }//*/
    for(int i=20;i > 0 ;i--)
    {
        offset = mousex * dv + (mousey+i) * widthVESA * dv;
        tmp = (uint16_t*)(mouse_buff + offset);
        mbf = (uint16_t*)(buff + offset);
        if(i < 13)
            for (int j=0;j <  i ;j++)
            {
               *mbf++ = *tmp++;
            }
        else
            for(int j=0;j<18 - i;j++)
            {
              *mbf++ = *tmp++;
            }
    }

    refresh_area(mousex-5,mousey-5,25+mousex,25+mousey);
  //  if(mousex < -mousedeltax)  mousex = 0;
  //  else mousex += mousedeltax;
  //  if(mousey < mousedeltay) mousey = 0;
  //  else mousey -= mousedeltay;
    mousex += mousedeltax;
    mousey -= mousedeltay;
    mousex = MIN(MAX(mousex,0), widthVESA);
    mousey = MIN(MAX(mousey,0), heightVESA);
    refresh_area(mousex-5,mousey-5,25+mousex,25+mousey);

    for(int i=20;i > 0 ;i--)
    {
        offset = mousex * dv + (mousey+i) * widthVESA * dv;
        tmp = (uint16_t*)(mouse_buff + offset);
        mbf = (uint16_t*)(buff + offset);
        if(i < 13)
            for (int j=0;j <  i ;j++)
            {
              *tmp++ = *mbf;
              *mbf++ = 0xffff;
            }
        else
            for(int j=0;j<18 - i;j++)
            {
              *tmp++ = *mbf;
              *mbf++ = 0xffff;
            }
    }

    mousedeltax = 0;
    mousedeltay = 0;
    asm volatile("int $50");
  }
}


inline void write_vesa(unsigned long int in,int x,int y)
{
    unsigned long int d=1,ln=0,b=in;
    for(unsigned long int i=0;b;i++)
    {
        b=b/10;
        ++ln;
    }
    b=in;
    for(unsigned long int i=0;i<ln;i++) d=d*10;
    d=d/10;
    unsigned long int i;
    for(i=0;i<ln;i++)
    {
        RectD(x+40*i,y,20,20,1+100*(b/d));
        b=b%d;
        d=d/10;
    }
}

uint32_t* prime_list;
uint32_t len_plist = 1;

int is_prime(uint32_t t)
{
  for(uint32_t i = 0; i<len_plist; i++)
  {
    if(!(t%prime_list[i])) return 0;
  }
  prime_list[len_plist] = t;
  ++len_plist;
  return 1;
}

void graph_plot(x,y);

void prime_diff_graph()
{
  prime_list = kmalloc(8192);
  prime_list[0] = 2;
  uint32_t b = 0;
  uint32_t j=1;
  for(uint32_t i = 3; b<1000; i++)
  {
    if(is_prime(i))
    {
      graph_plot(b++,(i-j)*4);
      j=i;
    }
  }
}

void graph_plot(x,y)
{
  //0,768 = 0,0
  Pixel_VESA_BUFF(x,760-y,0xfff0);
}
