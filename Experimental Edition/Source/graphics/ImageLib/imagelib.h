#ifndef IMAGELIB_H
#define IMAGELIB_H

#include "common.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct Image
{
  uintptr_t buffer;
  uint32_t width;
  uint32_t height;
  uint32_t bpp;
  uint32_t size;
  uint32_t format;
  uint32_t type;
}Image_t;

#endif
