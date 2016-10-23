
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include <common.h>
#include <vesa.h>
#include <graphics.h>
#include <mouse.h>

#define wVESA     1024
#define hVESA     768
#define dVESA     32

//int32 runs a bios interrupt, located at v86.asm
extern void int32(uint8_t intnum, regs16_t *regs);

/*Sets the bank if the Linear Frame Buffer is not supported or enabled*/
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
  vesa_alloc_base += 0x2000;
  return ab;
}

//sets up VESA for mode
void setVesa(uint32_t mode)
{

  VESA_INFO info; //VESA information
  MODE_INFO vbeModeInfo; //VESA mode information

  regs16_t regs;

  /**Gets VESA information**/

  uint32_t buffer = (uint32_t)vesa_malloc();//kmalloc(sizeof(VESA_INFO)) & 0xFFFFF; //sets the address for the buffer

  memcpy(buffer, "VBE2", 4);
  memset(&regs, 0, sizeof(regs)); //clears the registers typedef struct

  regs.ax = 0x4f00; //mode that gets VESA information
  regs.di = buffer & 0xF;
  regs.es = (buffer>>4) & 0xFFFF;
  int32(0x10, &regs); //calls v86 interupt
  memcpy(&info, buffer, sizeof(VESA_INFO)); //copies info from the buffer to the info typedef struct

  //print VESA information
  //~ k_printf("\n\nVesa Signature: %s\n", info.VESASignature);
  //~ k_printf("\n\nVesa Version: %h\n", info.VESAVersion);
  //~ k_printf("\n\nVesa Video Modes: %h\n", info.VideoModePtr);

  /**Gests VESA mode information**/

  //allocates memory for the buffer that stores the MODE_INFO for the VESA mode
  uint32_t modeBuffer = (uint32_t)vesa_malloc();//kmalloc(sizeof(MODE_INFO)) & 0xFFFFF;

  memset(&regs, 0, sizeof(regs)); //clears the registers typedef struct

  regs.ax = 0x4f01; //mode the gets the VESA mode information
  regs.di = modeBuffer & 0xF;
  regs.es = (modeBuffer>>4) & 0xFFFF;
  regs.cx = mode; //mode to get the information for
  int32(0x10, &regs);
  memcpy(&vbeModeInfo, modeBuffer, sizeof(MODE_INFO));

  widthVESA = vbeModeInfo.XResolution;
  heightVESA = vbeModeInfo.YResolution;
  depthVESA = vbeModeInfo.BitsPerPixel;
  if(depthVESA == 24) depthVESA = 32;

  //print VESA mode information
//  printf("\nBase Pointer: %x\n", (uint32_t)vbeModeInfo.PhysBasePtr);
//  printf("\nXRes: %x\n", (uint32_t)vbeModeInfo.XResolution);
//  printf("\nYRes: %x\n", (uint32_t)vbeModeInfo.YResolution);
//  printf("\nBits per pixel: %x\n", (uint32_t)vbeModeInfo.BitsPerPixel);
//  printf("\nExits status: %x\n", (uint32_t)regs.ax);

  /*Sets the Linear Frame Buffer address tp vga_mem and lfb variables*/
  vga_mem = (uint8_t*)vbeModeInfo.PhysBasePtr;
  general_buff = kmalloc(4*1024*768);
  buff=(uint8_t*)kmalloc(1024*768*4);
  vesa_buff = vga_mem;
//  general_buff = kmalloc(2*1024*768);
  mouse_buff = kmalloc(4*1024*768);
  faster_memset(mouse_buff, 0, 1024*768);
//  memset(general_buff, 0, 2*1024*768);
//  faster_memset(vga_mem, 0xffffffff, 1024*768);
  memset(buff, 90, 2*1024*768);
  refresh_area(0,0,1024,768);
  uint32_t lfb = (uint32_t)vbeModeInfo.PhysBasePtr;

  /**Sets up the VESA mode**/
  regs.ax = 0x4f02; //mode the sets up VESA graphics

  /*sets up mode with a linear frame buffer, the logical or (| 0x4000) tells
   * VESA VBE that we want to use a linear frame buffer*/
  regs.bx = (mode | 0x4000);
  int32(0x10, &regs);

  //reenable the interupts and the tasks to run
  //asm volatile("sti");

}
