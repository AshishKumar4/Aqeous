#ifndef VESA_h
#define VESA_h

#include "common.h"
#include "graphics.h"

#define VESA_SOFTWARE_DBUFF

// define our structure
typedef struct __attribute__ ((packed)) {
  unsigned short di, si, bp, sp, bx, dx, cx, ax;
  unsigned short gs, fs, es, ds, eflags;
} regs16_t;

int widthVESA, heightVESA, depthVESA;

uint32_t *vga_mem, *vga_mem1, *vga_mem2; //pointer where we assign our vga address
uint32_t* vga_pt1,* vga_pt2;
uint32_t buff_index = 0;
uint16_t* VESAmodes;

uint32_t nWidth, nHeight, nDepth;
//unsigned char buff[1024*768*8];
/*Typedef for VESA mode information*/
typedef struct  __attribute__ ((packed)) mode_info
{
  unsigned short ModeAttributes;//       __attribute__ ((packed));
  unsigned char  WinAAttributes;//       __attribute__ ((packed));
  unsigned char  WinBAttributes;//       __attribute__ ((packed));
  unsigned short WinGranularity;//        __attribute__ ((packed));
  unsigned short WinSize ;//              __attribute__ ((packed));
  unsigned short WinASegment ;//          __attribute__ ((packed));
  unsigned short WinBSegment ;//          __attribute__ ((packed));
  unsigned long  WinFuncPtr;//            __attribute__ ((packed));
  unsigned short BytesPerScanLine;//      __attribute__ ((packed));
  unsigned short XResolution   ;//        __attribute__ ((packed));
  unsigned short YResolution   ;//        __attribute__ ((packed));
  unsigned char  XCharSize;//            __attribute__ ((packed));
  unsigned char  YCharSize;//            __attribute__ ((packed));
  unsigned char  NumberOfPlanes;//       __attribute__ ((packed));
  unsigned char  BitsPerPixel;//         __attribute__ ((packed));
  unsigned char  NumberOfBanks;//        __attribute__ ((packed));
  unsigned char  MemoryModel;//          __attribute__ ((packed));
  unsigned char  BankSize;//             __attribute__ ((packed));
  unsigned char  NumberOfImagePages;//   __attribute__ ((packed));
  unsigned char  Reserved_page;//        __attribute__ ((packed));
  unsigned char  RedMaskSize;//          __attribute__ ((packed));
  unsigned char  RedMaskPos;//           __attribute__ ((packed));
  unsigned char  GreenMaskSize;//        __attribute__ ((packed));
  unsigned char  GreenMaskPos;//         __attribute__ ((packed));
  unsigned char  BlueMaskSize;//         __attribute__ ((packed));
  unsigned char  BlueMaskPos;//          __attribute__ ((packed));
  unsigned char  ReservedMaskSize;//     __attribute__ ((packed));
  unsigned char  ReservedMaskPos;//      __attribute__ ((packed));
  unsigned char  DirectColorModeInfo;//  __attribute__ ((packed));
  unsigned long  PhysBasePtr    ;//       __attribute__ ((packed));
  unsigned long  OffScreenMemOffset ;//   __attribute__ ((packed));
  unsigned short OffScreenMemSize ;//     __attribute__ ((packed));
  unsigned char  Reserved[206];//        __attribute__ ((packed));
} MODE_INFO;

/*Typedef for VESA information*/
typedef struct __attribute__ ((packed)) vesa_info
{
  char  VESASignature[4];//     __attribute__ ((packed));
  uint8_t VESAVersion[2];//          __attribute__ ((packed));
  uint32_t  OEMStringPtr;//         __attribute__ ((packed));
  uint8_t  Capabilities[4];//      __attribute__ ((packed));
  uint16_t  VideoModePtr[2];//          __attribute__ ((packed));
  uint16_t TotalMemory;//           __attribute__ ((packed));
  uint16_t OemSoftwareRev;//        __attribute__ ((packed));
  uint32_t  OemVendorNamePtr;//      __attribute__ ((packed));
  uint32_t  OemProductNamePtr;//     __attribute__ ((packed));
  uint32_t  OemProductRevPtr;//      __attribute__ ((packed));
  char  Reserved[222];//        __attribute__ ((packed));
  char  OemData[256];//         __attribute__ ((packed));
} VESA_INFO;

volatile VESA_INFO info; //VESA information
volatile MODE_INFO vbeModeInfo; //VESA mode information

typedef struct vbe_PModeInterface
{

}vbe_PModeInterface_t;

vbe_PModeInterface_t* vbePM_Interface;

void Init_Vesa();

MODE_INFO* vesa_GetMode(int mode);

int vesa_FindMode(int width, int height, int depth);

void vesa_ShowMode(MODE_INFO* inf);

void setBank(int bankNo);

void setVesa(uint32_t mode);

void vesa_getPM_info();

void vesa_SetBuffStart(int x, int y);
//void setVesaFull(uint32_t mode);
#endif
