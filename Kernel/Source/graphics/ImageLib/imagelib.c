#include "imagelib.h"
#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "lodepng/lodepng.h"

Image_t* ImageLoad_file(char* filename)
{
  unsigned error;
  unsigned char* image;
  unsigned width, height;
  unsigned char* png = 0;
  size_t pngsize;
  error = lodepng_load_file(&png, &pngsize, filename);
  if(!error)
  {
    error = lodepng_decode32(&image, &width, &height, png, pngsize);
  }
  if(error)
  {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return 0;
  }
  Image_t* img = (Image_t*)kmalloc(sizeof(Image_t));
  img->buffer = (uintptr_t)image;
  img->width = width;
  img->height = height;
  img->size = pngsize;
  img->bpp = 4;//pngsize/(width*height);
  return img;
}
