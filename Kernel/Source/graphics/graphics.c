#include <vesa.h>
#include <stddef.h>
#include <stdint.h>
#include <graphics.h>
#include <common.h>
#include <math.h>
#include <string.h>
#include <mouse.h>
#include "RandomLib/Random.h"
#include "WindowSystem/window.h"

#include "imagelib/imagelib.c"

inline void refresh_area(int x0, int y0, int x1, int y1)
{
  UpdateScreen(x0, y0, x1, y1);
}

void SetPixel_b(uint32_t* buffer, int x, int y, int c)
{
  int offset = (x*(depthVESA/8))+(y*(widthVESA*(depthVESA/8)));
  buffer[offset] = c;
}

void SetPixel_direct(int x, int y, int c)
{
  int offset = (x*(depthVESA/8))+(y*(widthVESA*(depthVESA/8)));
  vesa_buff[offset] = c;
}

void drawRect_b(uint32_t* buffer, int x, int y, int width, int height, int C)
{
  UpdateScreen(x, y, x+width, y+height);
  for(int i = y; i < height + y; i++)
  {
    for(int j = x; j < width + x; j++)
    {
      buffer[i*widthVESA + j] = C;
    }
  }
  //refresh_area(x,y,x+width,y+height);
}

void drawRect_direct(int x, int y, int width, int height, int C)
{
  int offset;
  for(int i = y; i < height + y; i++)
  {
      for(int j = x; j < width + x; j++)
    {
      offset = j * (depthVESA / 8) + i * (widthVESA * (depthVESA / 8));
      vesa_buff[offset] = C;
    }
  }
  //refresh_area(x,y,x+width,y+height);
}

void __attribute__((flatten)) drawRect_window(window_t* w, int x, int y, int width, int height, int C)
{
  uint32_t* tbuff = (uint32_t*)w->screen_buff;
  int window_width = w->locInfo.x_l;
  UpdateScreen(x + w->locInfo.o_x, y + w->locInfo.o_y, x+width + w->locInfo.o_x, y+height + w->locInfo.o_y);
  for(int i = y; i < height + y; i++)
  {
    for(int j = x; j < width + x; j++)
    {
      tbuff[i*window_width + j] = C;
    }
  }
}

void __attribute__((flatten)) __attribute__((optimize("O2"))) Gfx_DbuffSync()
{
  Mouse_Plot();
  uint32_t NeedToRefresh = dirtyRegions;
  WindowComposter(main_buff, &main_wlist);
  /*uint_fast32_t *sp = (uint_fast32_t*) main_buff;
  uint_fast32_t *dp = (uint_fast32_t*) vga_mem;*/

#ifdef VESA_SOFTWARE_DBUFF
  if(NeedToRefresh)
    memcpy_sse((uint32_t)vga_mem,(uint32_t)main_buff, (1024*768)/32);/*/
  memcpy_rep((uint32_t)vga_mem,(uint32_t)main_buff, (1024*768));//*/
#else
  if(main_buff == vga_mem1)
  {
    vesa_SetBuffStart(0, 0);
    main_buff = vga_mem2;
  }
  else
  {
    vesa_SetBuffStart(0, 768);
    main_buff = vga_mem1;
  }//*/
#endif
}

void Gfx_Dbuff()
{
  while(1)
  {
    Gfx_DbuffSync();
    asm volatile("int $50");
  }
}

void __attribute__((flatten)) line_plot(int x0, int y0, int x1, int y1, int color, int sz)
{
      int deltax = x1-x0;
      int deltay = y1-y0;

      int y=0;
      int x=0;

      int sdx = (deltax < 0) ? -1 : 1; // sign of deltax (e.g. +ve or -ve)
      int sdy = (deltay < 0) ? -1 : 1;      deltax = sdx * deltax + 1;
      deltay = sdy * deltay + 1; // (-1*-6)+1) = 7

      int px = x0; // starting point (x0,y0)
      int py = y0;

      //float ynum = deltax/2; // num stands for the numerator starting value

      if( deltax >= deltay )
      {
            for (x = 0; x < deltax; x++)
            {

                  SetPixel_b( main_buff, px, py,  color);
                  for(int j = 1; j<sz; j++)
                  {
                    SetPixel_b( main_buff, px, py+j,  color);
                    SetPixel_b( main_buff, px, py-j,  color);
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
                    SetPixel_b( main_buff, px + j, py,  color);
                    SetPixel_b( main_buff, px - j, py,  color);
                  }
                  SetPixel_b( main_buff, px, py,  color);
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

void __attribute__((flatten)) __attribute__((optimize("O2"))) Mouse_Drag_Window()
{
  mouse_handler();
  mousex += mousedeltax;
  mousey -= mousedeltay;
  if(mousedeltax || mousedeltay)
  {
    //WindowDrag(mouse_w, mousex, mousey);
    mouse_w->locInfo.o_x = mousex;
    mouse_w->locInfo.o_y = mousey;
    WindowDrag_relative(Mouse_Dragged_Window, mousedeltax, -mousedeltay);//mousex, mousey);
    mousedeltax = 0;
    mousedeltay = 0;
  }
  if(!mouse_left_click)
  {
    Mouse_Plot = Mouse_Normal_Handler;
  }
  mousex = MIN(MAX(mousex,0), widthVESA);
  mousey = MIN(MAX(mousey,0), heightVESA);/*/
  mousex %= widthVESA;
  mousey %= heightVESA;//*/
}

void __attribute__((flatten)) __attribute__((optimize("O2"))) Mouse_Normal_Handler()
{
  mouse_handler();
  mousex += mousedeltax;
  mousey -= mousedeltay;
  if(mousedeltax || mousedeltay)
  {
    WindowDrag(mouse_w, mousex, mousey);
    mousedeltax = 0;
    mousedeltay = 0;
  }
  if(mouse_left_click)
  {
    Mouse_LeftClick_Handler(mousex, mousey);
  }
  mousex = MIN(MAX(mousex,0), widthVESA);
  mousey = MIN(MAX(mousey,0), heightVESA);/*/
  mousex %= widthVESA;
  mousey %= heightVESA;//*/
}

void __attribute__((flatten)) __attribute__((optimize("O0"))) Mouse_Normal_Handler2()
{
  mouse_handler();
  mousex += mousedeltax;
  mousey -= mousedeltay;
  if(mousedeltax || mousedeltay)
  {
    WindowDrag(mouse_w, mousex, mousey);
  }
  if(mouse_left_click)
  {
    Mouse_LeftClick_Handler(mousex, mousey);
  }
  mousex = MIN(MAX(mousex,0), widthVESA);
  mousey = MIN(MAX(mousey,0), heightVESA);
  //mousex %= widthVESA;
  //mousey %= heightVESA;
  mousedeltax = 0;
  mousedeltay = 0;
}
