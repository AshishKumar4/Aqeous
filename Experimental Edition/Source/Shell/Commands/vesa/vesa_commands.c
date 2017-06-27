#include "vesa.h"
#include "graphics.h"

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
    vesa_getPM_info();
  }
  else if(!strcmp(arg, "test"))
  {
    printf("\nJust Testing");
    delay1(1);
  }
}

void Start_vesa()
{
   printf("\n Entering VESA SVGA");
   vesa_GetMode(nMode);
   setVesa(nMode);
   vga_mem = (uint8_t*)vbeModeInfo.PhysBasePtr;
#ifdef VESA_SOFTWARE_DBUFF
   vga_mem1 = pmem_4k(768);//vga_mem;
#else
   vga_mem1 = vga_mem;
#endif
   vga_mem2 = vga_mem + (1024*768);
   //vga_mem2 = kmalloc(4*1024*1024);

   main_buff = vga_mem1;
   vesa_buff = vga_mem;
   faster_memset((void*)main_buff, 0xffffffff, 1024*768);
   refresh_area(0,0,1024,768);

   /*Task_sleep(Shell_Ostream_task);
   Shell_Ostream_task = create_task("Vesa_test_buf", Gfx_DbuffSync, 1, 0x202, Shell_proc);
   Task_wakeup(Shell_Ostream_task);
  // Activate_task_direct(Shell_Ostream_task);//*/

   //Switch_back_from_System();
   /*vesa_lfb();//*/
   //RectD(0,0,1024,768, 0x0092ff);
   //Activate_task_direct(create_task("Mouse_Pointer", Mouse_Plot, 2, 0x202, Shell_proc));

   //Activate_task_direct(create_task("Mouse_Updater", Mouse_updater, 0, 0x202, Shell_proc));

   bgnd = create_window_basic("Background", Shell_proc, 0, 0, 0, 1024, 768, 0);
   main_wlist.window = bgnd;
   faster_memset((void*)bgnd->screen_buff, 0xffbfbfbf, 1024*768);

   mouse_w = create_window_basic("mouse_pointer", Shell_proc, bgnd, 512, 384, 15, 23, 0);
   wlist_end = (window_list_t*)kmalloc(sizeof(window_list_t));
   main_wlist.next = wlist_end;
   wlist_end->window = mouse_w;
   wlist_end->next = NULL;
   wlist_end->back = &main_wlist;
   mouseinit();
   //faster_memset(mouse_w->screen_buff, 0x00000000, 15*23);
   memcpy_rep((uint32_t)mouse_w->screen_buff, (uint32_t)gliph_cursorArrow, 15*23);

   window_t* tmpw = create_window_autoMap("test", Shell_proc, bgnd, 100, 200, 400, 400, 0);
   faster_memset((void*)tmpw->screen_buff, 0xb0d0d0ff, 400*400);

   window_t* tmpw2 = create_window_autoMap("test2", Shell_proc, bgnd, 150, 220, 100, 100, 0);
   faster_memset((void*)tmpw2->screen_buff, 0x80a0ff2f, 100*100);

   createDefaultWindowObjects(tmpw);
   createDefaultWindowObjects(tmpw2);

   //drawRect_window(tmpw, 0,0,70,50,0x900000ff);

   drawRect_window((uint32_t*)bgnd, 400,300,50,10,0xffff0000);
   drawRect_b((uint32_t*)bgnd->screen_buff, 500,300,50,10,0xff00ff00);
   drawRect_b((uint32_t*)bgnd->screen_buff, 600,300,50,10,0xff0000ff);


    Mouse_Plot = Mouse_Normal_Handler;
    Screen_BuffSync = Gfx_DbuffSync;

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
