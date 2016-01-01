
#include <common.h>
#include <mem.h>
#include <console.h>
#include <paging.h>

#define wVESA     1024
#define hVESA     768
#define dVESA     32


typedef struct __attribute__ ((packed)) {
  unsigned short di, si, bp, sp, bx, dx, cx, ax;
  unsigned short gs, fs, es, ds, eflags;
} regs16_t;

int widthVESA, heightVESA, depthVESA;

unsigned char *vga_mem;

unsigned char *buff;

u32int *vga;

typedef struct MODE_INFO
{
  unsigned short ModeAttributes       __attribute__ ((packed));
  unsigned char  WinAAttributes;
  unsigned char  WinBAttributes;
  unsigned short WinGranularity       __attribute__ ((packed));
  unsigned short WinSize              __attribute__ ((packed));
  unsigned short WinASegment          __attribute__ ((packed));
  unsigned short WinBSegment          __attribute__ ((packed));
  unsigned long  WinFuncPtr           __attribute__ ((packed));
  unsigned short BytesPerScanLine     __attribute__ ((packed));
  unsigned short XResolution          __attribute__ ((packed));
  unsigned short YResolution          __attribute__ ((packed));
  unsigned char  XCharSize;
  unsigned char  YCharSize;
  unsigned char  NumberOfPlanes;
  unsigned char  BitsPerPixel;
  unsigned char  NumberOfBanks;
  unsigned char  MemoryModel;
  unsigned char  BankSize;
  unsigned char  NumberOfImagePages;
  unsigned char  Reserved_page;
  unsigned char  RedMaskSize;
  unsigned char  RedMaskPos;
  unsigned char  GreenMaskSize;
  unsigned char  GreenMaskPos;
  unsigned char  BlueMaskSize;
  unsigned char  BlueMaskPos;
  unsigned char  ReservedMaskSize;
  unsigned char  ReservedMaskPos;
  unsigned char  DirectColorModeInfo;
  unsigned long  PhysBasePtr          __attribute__ ((packed));
  unsigned long  OffScreenMemOffset   __attribute__ ((packed));
  unsigned short OffScreenMemSize     __attribute__ ((packed));
  unsigned char  Reserved[206];
} MODE_INFO;

typedef struct VESA_INFO
{
  unsigned char  VESASignature[4];//     __attribute__ ((packed));
  unsigned short VESAVersion          __attribute__ ((packed));
  unsigned long  OEMStringPtr         __attribute__ ((packed));
  unsigned char  Capabilities[4];//      __attribute__ ((packed));
  unsigned long  VideoModePtr         __attribute__ ((packed));
  unsigned short TotalMemory          __attribute__ ((packed));
  unsigned short OemSoftwareRev       __attribute__ ((packed));
  unsigned long  OemVendorNamePtr     __attribute__ ((packed));
  unsigned long  OemProductNamePtr    __attribute__ ((packed));
  unsigned long  OemProductRevPtr     __attribute__ ((packed));
  unsigned char  Reserved[222];//        __attribute__ ((packed));
  unsigned char  OemData[256];//         __attribute__ ((packed));
} VESA_INFO;

MODE_INFO *vbeModeInfo;
 void int32(u8int intnum, regs16_t *regs);

void setBank(int bankNo)
{
  regs16_t regs;

  regs.ax = 0x4f05;
  regs.bx = 0x0;
  regs.dx = bankNo;

  int32(0x10, &regs);
}
extern u32int placement_address;
void setVesa(u32int mode)
{
  //VESA_INFO info; //VESA information //VESA mode information

  regs16_t *regs;

  /**Gets VESA information**/
/*
  u32int buffer = (u32int)kmalloc(sizeof(VESA_INFO)) & 0xFFFFF;

  memcpy(buffer, "VBE2", 4);
  memset(&regs, 0, sizeof(regs));

  regs.ax = 0x4f00;
  regs.di = buffer & 0xF;
  regs.es = (buffer>>4) & 0xFFFF;
  int32(0x10, &regs);
  memcpy(&info, buffer, sizeof(VESA_INFO));
//*/
  vbeModeInfo=kmalloc(sizeof(MODE_INFO)) & 0xFFFFF;
  u32int modeBuffer = vbeModeInfo;
  regs=kmalloc(sizeof(regs)) & 0xFFFFF;
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
  vga_mem = (u8int*)4244635648;//(u8int*)vbeModeInfo->PhysBasePtr;
  widthVESA=vbeModeInfo->XResolution;
  heightVESA=vbeModeInfo->YResolution;
  depthVESA=vbeModeInfo->BitsPerPixel;
  buff=(u8int*)kmalloc(1024*768*2);
  //paging();
  asm volatile("sti");

}

void vesa_lfb()
{
	vga_mem=(u8int*)vga;
    widthVESA=vga[0];
    heightVESA=vga[1];
    depthVESA=vga[2];
    buff=(u8int*)kmalloc(1024*768*4);
}
