#ifndef GRAPHICS_h
#define GRAPHICS_h

#include <common.h>

extern int widthVESA, heightVESA, depthVESA;
unsigned int background=90;
unsigned char *tempbuff;
extern uint32_t *vga_mem; //pointer where we assign our vga address
int offset;
int width=1022,height=766;
unsigned char *general_buff;

volatile int cx0 = 512, cx1 = 512, cy0 = 384, cy1 = 384;

void line_fast(int x1, int y1, int x2, int y2, int color, int thickness);

extern void copy_gbuffer();

inline void refresh_area(int x0, int y0, int x1, int y1);

inline void Pixel_VESA_BUFF(int x, int y, int C);

inline void Pixel_VESA_BUFF_M(int x, int y, int C, char* cb);

inline void Pixel_VESA(int x, int y, int C);

void RectD(int x, int y, int width, int height, int C);

void DBuff();

void RectL(int x, int y, int width, int height, int C);

void Creater(int i,int j);

void Mouse_Plot();

inline void write_vesa(unsigned long int in,int x,int y);

//void Func_Plot(int x1, int y1, int x2, int y2, int C, int sz, intfunc2_t func);

void line_plot(int x0, int y0, int x1, int y1, int color, int sz);

#endif
