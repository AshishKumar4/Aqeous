#include "tasking.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include <common.h>
#include <vesa.h>
#include <graphics.h>
#include <mouse.h>
#include "MManager/mmanagerSys.h"

#define wVESA     1024
#define hVESA     768
#define dVESA     32

regs16_t* regs = 0x72000;

//int32 runs a bios interrupt, located at v86.asm
extern void int32(uint8_t intnum, regs16_t *regs);
//#define USE_VIRTUALx86_mode
void Bios_int32(uint8_t intnum, regs16_t *regs)
{
#ifndef USE_VIRTUALx86_mode
  AP_realMode_Execute(intnum, regs);
#else
  int32(intnum, regs);
#endif
  //printf("\nsTATUS: [%x]", regs->ax);
}

/*Sets the bank if the Linear Frame Buffer is not supported or enabled*/
void setBank(int bankNo)
{
  //regs16_t regs;

  regs->ax = 0x4f05;
  regs->bx = 0x0;
  regs->dx = bankNo;

  Bios_int32(0x10, regs);

}

void __attribute__((optimize("O2"))) Init_Vesa()
{
  //regs16_t regs;

  /**Gets VESA information**/

  uint32_t vesa_struct_buff = (uint32_t)0x71000;//vesa_malloc();//kmalloc(sizeof(VESA_INFO)) & 0xFFFFF; //sets the address for the buffer

  memset((void*)vesa_struct_buff, 0, 1024);
  memcpy((void*)vesa_struct_buff, (void*)"VBE2", 4);
  memset((void*)regs, 0, sizeof(regs16_t)); //clears the registers typedef struct

  regs->ax = 0x4f00; //mode that gets VESA information
  regs->di = vesa_struct_buff & 0xF;
  regs->es = (vesa_struct_buff>>4) & 0xFFFF;
  Bios_int32(0x10, regs); //calls v86 interupt
  printf("\n[%x]", regs->ax);
  memcpy((void*)&info, (void*)vesa_struct_buff, sizeof(VESA_INFO)); //copies info from the buffer to the info typedef struct

  //print VESA information
  printf("\nStatus: %x\nVesa Signature: %s\n", regs->ax, info.VESASignature);
  printf("\n\nVesa Version: %d.%d\n", (uint32_t)info.VESAVersion[1], (uint32_t)info.VESAVersion[0]);
  printf("\n\nVesa Video Modes: %x\n", (uint32_t)info.VideoModePtr[1]*16 + info.VideoModePtr[0]);
  printf("\nVesa Video Total Memory: %d\n", info.TotalMemory);
  /**Gets VESA mode information**/

  VESAmodes = (info.VideoModePtr[1]*16 + info.VideoModePtr[0]);
}

MODE_INFO* __attribute__((optimize("O2"))) vesa_GetMode(int mode)
{
  /**Gets VESA mode information**/
  //regs16_t regs;

  //allocates memory for the buffer that stores the MODE_INFO for the VESA mode
  uint32_t modeBuffer = (uint32_t)0x70000;//vesa_malloc();//kmalloc(sizeof(VESA_INFO)) & 0xFFFFF; //sets the address for the buffer

  memset((void*)modeBuffer, 0, 1024);
  memset((void*)regs, 0, sizeof(regs16_t)); //clears the registers typedef struct

  regs->ax = 0x4f01; //mode the gets the VESA mode information
  regs->di = modeBuffer & 0xF;
  regs->es = (modeBuffer>>4) & 0xFFFF;
  regs->cx = mode; //mode to get the information for
  Bios_int32(0x10, regs);
  memcpy(&vbeModeInfo, modeBuffer, sizeof(MODE_INFO));

  return &vbeModeInfo;
}

void vesa_ShowMode(MODE_INFO* inf)
{
    //print VESA mode information
    printf("\nBase Pointer: %d\n", (uint32_t)inf->PhysBasePtr);
    printf("\nXRes: %d\n", (uint32_t)inf->XResolution);
    printf("\nYRes: %d\n", (uint32_t)inf->YResolution);
    printf("\nBits per pixel: %d\n", (uint32_t)inf->BitsPerPixel);
}

int __attribute__((optimize("O2"))) vesa_FindMode(uint32_t width, uint32_t height, uint32_t depth)
{
  uint16_t* modes = VESAmodes;
  MODE_INFO* inf;
  for(int i = 0; modes[i] != 0xffff; i++)
  {
    inf = vesa_GetMode(modes[i]);
    if((uint32_t)inf->XResolution == width && (uint32_t)inf->YResolution == height && (uint32_t)inf->BitsPerPixel == depth)
    {
      vesa_ShowMode(inf);

      return modes[i];
    }
  }
  return 0;
}

//sets up VESA for mode
void __attribute__((optimize("O2"))) setVesa(uint32_t mode)
{
  //regs16_t regs;
  memset((void*)regs,(void*) 0, sizeof(regs16_t)); //clears the registers typedef struct
  /*Sets the Linear Frame Buffer address tp vga_mem and lfb variables*/

  /**Sets up the VESA mode**/
  regs->ax = 0x4f02; //mode the sets up VESA graphics

  /*sets up mode with a linear frame buffer, the logical or (| 0x4000) tells
   * VESA VBE that we want to use a linear frame buffer*/
  regs->bx = (mode | 0x4000);
  Bios_int32(0x10, regs);

  widthVESA = vbeModeInfo.XResolution;
  heightVESA = vbeModeInfo.YResolution;
  depthVESA = vbeModeInfo.BitsPerPixel/4;
}

void vesa_getPM_info()
{
  //regs16_t regs;
  memset((void*)regs, 0, sizeof(regs16_t)); //clears the registers typedef struct

  regs->ax = 0x4f0a; // VBE 2.0 Protected Mode Interface

  Bios_int32(0x10, regs);

  printf("\nGeting Protected Mode VBE2.0 Interface...\n\tStatus : %x\tES: %d DI: %d\n\tNumber of Bytes: %d", regs->ax, regs->es, regs->di, regs->cx);

  vbePM_Interface = regs->es*0x10 + regs->di;
}

void __attribute__((optimize("O0"))) vesa_SetBuffStart(int x, int y)
{
  //regs16_t regs;
  memset((void*)regs, 0, sizeof(regs16_t)); //clears the registers typedef struct

  regs->ax = 0x4f07; // VBE Set/Get Display Start control
  //regs->bl = 0x2;    // Schedule Display Start Alternate
  regs->bx = 0x80;
  regs->cx = x;
  regs->dx = y;

  Bios_int32(0x10, regs);
}
