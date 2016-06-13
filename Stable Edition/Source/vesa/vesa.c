#include <common.h>
#include <mem.h>
#include <paging.h>
#include <string.h>
#include "vesa.h"

void setBank(int bankNo)
{
  regs16_t regs;

  regs.ax = 0x4f05;
  regs.bx = 0x0;
  regs.dx = bankNo;

  int32(0x10, &regs);
}

uint32_t vesa_alloc_base = 0x1000;

inline uint32_t vesa_malloc()
{
  uint32_t ab = vesa_alloc_base;
  vesa_alloc_base += 0x1000;
  return ab;
}

void setVesa(uint32_t mode)
{
  asm volatile("cli");
  //VESA_INFO info; //VESA information //VESA mode information

  regs16_t *regs;

  vbeInfo = (VESA_INFO*)vesa_malloc();
//  printf("\n vbeInfo:%x %x", vbeInfo, sizeof(VESA_INFO));
  u32int buffer = (uint32_t)vbeInfo;
  regs = (regs16_t*)vesa_malloc();
//  printf("\n regs:%x %x", regs, sizeof(regs16_t));

  memcpy((void*)buffer, "VBE2", 4);

  regs->ax = 0x4f00;
  regs->di = buffer & 0xF;
  regs->es = (buffer>>4) & 0xFFFF;
  int32(0x10, regs);
  //*/
  vbeModeInfo=(MODE_INFO*)vesa_malloc();
//  printf("\n vbeModeInfo:%x %x", vbeModeInfo, sizeof(MODE_INFO));
  u32int modeBuffer = (uint32_t)vbeModeInfo;
  regs=(regs16_t*)vesa_malloc();
//  printf("\n regs:%x", regs);
  //memset(&regs, 0, sizeof(regs));
  regs->ax = 0x4f01;
  regs->di = modeBuffer & 0xF;
  regs->es = (modeBuffer>>4) & 0xFFFF;
  regs->cx = mode;
  int32(0x10, regs);
  //memcpy(vbeModeInfo, modeBuffer, sizeof(MODE_INFO));
  regs->ax = 0x4f02;
  regs->bx = (mode | 0x4000);
  int32(0x10, regs);
  vga_mem = (uint8_t*)vbeModeInfo->PhysBasePtr;
  widthVESA=vbeModeInfo->XResolution;
  heightVESA=vbeModeInfo->YResolution;
  depthVESA=vbeModeInfo->BitsPerPixel;
  //asm volatile("sti");
}

void vesa_lfb()
{
  buff=(uint8_t*)kmalloc(1024*768*2);
  map(vbeModeInfo->PhysBasePtr,1024*768*2,system_dir);
}
