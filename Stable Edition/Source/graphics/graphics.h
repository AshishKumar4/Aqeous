#ifndef GRAPHICS_h
#define GRAPHICS_h

extern int widthVESA, heightVESA, depthVESA;
unsigned int background=90;
unsigned char *tempbuff;
unsigned char buffer1=90& 0xff;
unsigned char buffer2=90& 0xff;
unsigned char buffer3=90& 0xff;
extern unsigned char *vga_mem; //pointer where we assign our vga address
int offset;
int width=1022,height=766;

void Pixel_VESA_BUFF(int x, int y, int C1,int C2, int C3);

void Pixel_VESA(int x, int y, int C1,int C2, int C3);

void RectD(int x, int y, int width, int height, int C1,int C2, int C3);

void DBuff();

void RectL(int x, int y, int width, int height, int C1,int C2, int C3);

void Creater(int i,int j);

void Mouse_Plot(int x,int y);

inline void write_vesa(unsigned long int in,int x,int y);

#endif
