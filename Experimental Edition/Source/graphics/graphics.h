#ifndef GRAPHICS_h
#define GRAPHICS_h

#include <common.h>
#include "WindowSystem/window.h"
#include "gliphs/gliphs.h"

extern int widthVESA, heightVESA, depthVESA;

unsigned int background=90;

extern uint32_t *vga_mem; //pointer where we assign our vga address
int offset;
int width=1022,height=766;
unsigned char *general_buff;

uint32_t* main_buff;
uint32_t* vesa_buff;
uint32_t VGA_cp_sz;

uint32_t dbuff_run;

window_t* mouse_w;

window_t*  Mouse_Dragged_Window;
func_t Mouse_Plot;

volatile int cx0 = 512, cx1 = 512, cy0 = 384, cy1 = 384;

inline void refresh_area(int x0, int y0, int x1, int y1);

void SetPixel_b(uint32_t* buffer, int x, int y, int c);

void SetPixel_direct(int x, int y, int c);

void drawRect_b(uint32_t* buffer, int x, int y, int width, int height, int C);

void drawRect_direct(int x, int y, int width, int height, int C);

void drawRect_window(window_t* w, int x, int y, int width, int height, int C);

void Gfx_DbuffSync();

void line_plot(int x0, int y0, int x1, int y1, int color, int sz);

void Mouse_Drag_Window();

void Mouse_Normal_Handler();
void Mouse_Normal_Handler2();

#endif
