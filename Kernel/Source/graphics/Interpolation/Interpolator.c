#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "graphics/graphics.h"
#include "graphics/ImageLib/imagelib.h"
#include "lodepng/lodepng.h"
#include "WindowSystem/window.h"

uintptr_t Interpolate2x(uintptr_t buffer, int xl, int yl, int m_x, int m_y)
{
  uint32_t* nbuf = kmalloc(xl*yl*4*m_x*m_y), *tmp1 = (uint32_t*)nbuf, *tmp2 = (uint32_t*)buffer;
  for(uint32_t ti = 0; ti < yl; ti++)
  {
    for(uint32_t tj = 0; tj < xl; tj++)
    {
      tmp1[(ti*xl*m_y*2) + (tj*m_x)] = tmp2[(ti*xl) + tj];
    }
  }
  /*/
  for(uint32_t t1 = 0; t1 < sz; t1++)   // Create the initial enlarged image
  {
    tmp1[(t1*2) + ()] = tmp2[t1];
  }*/
  return (uintptr_t)tmp1;
}

void Interpolate_Test()
{
  Image_t* img = ImageLoad_file("test2.png");
  window_t* tmpw = create_window_autoMap("test3", Shell_proc, bgnd, 10, 10, 700, 700, 0);
  createDefaultWindowObjects(tmpw);
  Window_DrawOverWindow(tmpw, img->buffer, 50, 50, img->width, img->height);

  uintptr_t buff = Interpolate2x((uintptr_t)img->buffer, img->width, img->height, 2, 2);

  window_t* tmpw2 = create_window_autoMap("test4", Shell_proc, bgnd, 20, 20, 700, 700, 0);
  createDefaultWindowObjects(tmpw2);
  Window_DrawOverWindow(tmpw2, buff, 50, 50, img->width*2, img->height*2);

}
