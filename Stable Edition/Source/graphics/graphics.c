#include <vesa.c>
#include <stddef.h>
#include <stdint.h>
#include <graphics.h>

void Init()
{
    //RectL(0,0,1022,767,90,90,90);
}


//void Clear()
//{
 //   for(int i=0;i<(1020*(depthVESA/8)+760*(widthVESA*(depthVESA/8)));i++)
  //      vga_mem[i]=NULL;
//}
void Pixel_VESA_BUFF(int x, int y, int C1,int C2, int C3)
{
  /*calculates the offset for a specific point on screen*/
    int offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));

    buff[offset+1]=C1& 0xff;
    buff[offset+2]=C2& 0xff;
    buff[offset+3]=C3& 0xff;

}

void Pixel_VESA(int x, int y, int C1,int C2, int C3)
{
  /*calculates the offset for a specific point on screen*/
    offset = x * (depthVESA / 8) + y * (widthVESA * (depthVESA / 8));
    vga_mem[offset + 0] = C1 & 0xff;           //BLUE
    vga_mem[offset + 1] = C2 & 0xff;    //GREEN
    vga_mem[offset + 2] = C3 & 0xff;   //RED*/

   // buffer1[offset] = C1;           //BLUE
  //  buffer2[offset] = C2;    //GREEN
   // buffer3[offset] = C3;   //RED
    buff[offset+1]=C1& 0xff;
    buff[offset+2]=C2& 0xff;
    buff[offset+3]=C3& 0xff;
    //*/
}

void RectD(int x, int y, int width, int height, int C1,int C2, int C3)
{
  for(int i = y; i < height + y; i++)
    for(int j = x; j < width + x; j++)
      Pixel_VESA_BUFF(j, i, C1,C2,C3);
}

void DBuff()
{
    memcpy(vga_mem,buff,1024*768*2);
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
    {
        offset = j * (depthVESA / 8) + i * (widthVESA * (depthVESA / 8));
        if(buff[offset]!=buffer1)
           buff[offset]=buffer1;
        if(buff[offset+1]!=buffer2)
           buff[offset+1]=buffer2;
        if(buff[offset+2]!=buffer3)
           buff[offset+2]=buffer3;
    }
}

void RectL(int x, int y, int width, int height, int C1,int C2, int C3)
{
  for(int i = y; i < height + y; i++)
    for(int j = x; j < width + x; j++)
      Pixel_VESA(j, i, C1,C2,C3);
}

void Creater(int i,int j)
{
    Pixel_VESA(i,i+j,1000,1000,1000);
}

void Mouse_Plot(int x,int y)
{
    for(int i=0;i <20;i++)
    {
        if(i<13)
            for (int j=0;j <i;j++)
            {
                Pixel_VESA_BUFF(x+j,y+i,500,10000,10000);
            }
        else
            for(float j=0;j<19-i;j=j+0.6)
            {
                Pixel_VESA_BUFF(x+j,y+i,500,10000,10000);
            }
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
        RectD(x+40*i,y,20,20,1+100*(b/d),1+100*(b/d),1+100*(b/d));
        b=b%d;
        d=d/10;
    }
}
