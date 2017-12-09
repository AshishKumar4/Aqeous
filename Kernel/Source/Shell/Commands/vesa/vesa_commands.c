#include "vesa.h"
#include "graphics.h"
#include "lodepng/lodepng.h"
#include "stdio.h"
#include "imagelib/imagelib.h"
#include "multiboot.h"
#include "Interpolation/Interpolator.h"
//#include "upng/upng.h"

int nMode = 0;
void Start_vesa();

void Command_vesa()
{
  asm volatile("cli");
  char* arg = (char*)CSI_Read(1);
  if(!strcmp(arg, "start"))
  {
    Start_vesa();
  }
  else if(!strcmp(arg, "set"))
  {
    nWidth = StrToInt((char*)CSI_Read(2));
    nHeight = StrToInt((char*)CSI_Read(3));
    nDepth = StrToInt((char*)CSI_Read(4));
    nMode = vesa_FindMode(nWidth, nHeight, nDepth);
    if(nMode)
    {
      printf("\nMode Found: %d; hex: %x",nMode, nMode);
    }
    else
    {
      printf("\nMode Not Found!");
    }
  }
  else if(!strcmp(arg, "show"))
  {
    //print VESA information
    printf("\n\nVesa Signature: %s\n", info.VESASignature);
    printf("\n\nVesa Version: %d.%d\n", (uint32_t)info.VESAVersion[1], (uint32_t)info.VESAVersion[0]);
    printf("\n\nVesa Video Modes: %x\n", (uint32_t)info.VideoModePtr[1]*16 + info.VideoModePtr[0]);
    printf("\nVesa Video Total Memory: %d\n", info.TotalMemory);
  }
  else if(!strcmp(arg, "mode"))
  {
    int mode = StrToInt((char*)CSI_Read(2));
    vesa_ShowMode(vesa_GetMode(mode));
    if(!strcmp((char*)CSI_Read(3), "s"))
      nMode = mode;
  }
  else if(!strcmp(arg, "info"))
  {
  //  setVesaFull(0x117);
    Init_Vesa();
    //vesa_getPM_info();
  }
  else if(!strcmp(arg, "test"))
  {
       Image_t* img = ImageLoad_file(CSI_Read(2));
       printf("\n=> %d %d", img->width, img->height);
  }
}

void Start_vesa()
{
  printf("\n Entering VESA SVGA");
  vesa_GetMode(nMode);
  setVesa(nMode);
  vga_mem = (uint8_t*)vbeModeInfo.PhysBasePtr;
   
#ifdef VESA_SOFTWARE_DBUFF
  vga_mem1 = pmem_4k((widthVESA*heightVESA)/1024);//vga_mem;
#else
  vga_mem1 = vga_mem;
  vga_mem2 = vga_mem + (widthVESA*heightVESA);
#endif
  main_buff = vga_mem1;
  vesa_buff = vga_mem;
  VGA_cp_sz = (widthVESA*heightVESA)/32;

  faster_memset((void*)main_buff, 0xffffffff, widthVESA*heightVESA);
  refresh_area(0,0,widthVESA,heightVESA);

  bgnd = create_window_basic("Background", Shell_proc, 0, 0, 0, widthVESA, heightVESA, 0);
  main_wlist.window = bgnd;
  faster_memset((void*)bgnd->screen_buff, 0xffbfbfbf, widthVESA*heightVESA);

  mouse_w = create_window_basic("mouse_pointer", Shell_proc, bgnd, 512, 384, 15, 23, 0);
  wlist_end = (window_list_t*)kmalloc(sizeof(window_list_t));
  main_wlist.next = wlist_end;
  wlist_end->window = mouse_w;
  wlist_end->next = NULL;
  wlist_end->back = &main_wlist;
  mouseinit();
  //faster_memset(mouse_w->screen_buff, 0x00000000, 15*23);
  memcpy_rep((uint32_t)mouse_w->screen_buff, (uint32_t)gliph_cursorArrow, 15*23);

  window_t* tmpw = create_window_autoMap("test", Shell_proc, bgnd, 10, 10, 400, 400, 0);
  faster_memset((void*)tmpw->screen_buff, 0xd0d0d0ff, 400*400);

  window_t* tmpw2 = create_window_autoMap("test2", Shell_proc, bgnd, 150, 220, 100, 100, 0);
  faster_memset((void*)tmpw2->screen_buff, 0xd0a0ff2f, 100*100);

  createDefaultWindowObjects(tmpw);
  createDefaultWindowObjects(tmpw2);

  Image_t* Wallpaper = ImageLoad_file("t3.png");
  if(Wallpaper)
    memcpy_rep((void*)bgnd->screen_buff, Wallpaper->buffer, 1024*768);//*/

  //Interpolate_Test();

  Mouse_Plot = Mouse_Normal_Handler;
  PIT_Task = Gfx_DbuffSync;
  
  //_kill(Shell_task);
  /*_kill(Shell_Ostream_task);
  Shell_Ostream_task = create_task("Shell_GfxOstream", Gfx_Dbuff, 1, 0x202, Shell_proc);
  Activate_task_direct(Shell_Ostream_task);
  
/*
   uint32_t NeedToRefresh = dirtyRegions;
   WindowComposter(main_buff, &main_wlist);
   memcpy_rep((uint32_t)vga_mem,(uint32_t)main_buff, (1024*768));
   while(1);
   return;*/

   //Image_t* test = ImageLoad_file("t2.png");
   //memcpy_rep((void*)tmpw->screen_buff, test->buffer, 800*700);

   //drawRect_window(tmpw, 0,0,70,50,0x900000ff);

   /*font_maker();
   font_renderer(char_B, 100, 100, 0xf000, 10, 3);
   font_renderer(char_C, 160, 100, 0xf0ff, 10, 3);
   font_renderer(char_E, 240, 100, 0x0f00, 10, 3);
   font_renderer(char_1, 320, 100, 0xff0f, 10, 3);
   font_renderer(char_2, 380, 100, 0x0f0f, 10, 3);
   font_renderer(char_3, 460, 100, 0xfff0, 10, 3);
   font_renderer(char_4, 520, 100, 0xfa0f, 10, 3);
   font_renderer(char_5, 580, 100, 0xfa0f, 10, 3);
//   Pixel_VESA_BUFF( 500, 500, 0xff00ff);
//   line_fast(100,400,100,500,0xffff, 1);
//   line_fast(130,400,130,500,0xffff, 10);
//   random_plotter();
  // prime_diff_graph();
   //RectD(510, 510, 500,500,0xff00ff);
   //RectD(100,300,50,10,7,7,7);
   /*while(1)
   {
     //Mouse_Plot(mousex,mousey);
     DBuff();
   }//*/
}
