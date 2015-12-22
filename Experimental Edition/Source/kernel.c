#include <mouse.c>
#include <console.c>

void kernel_early()
{
    kmalloc(1);
    console_init();
    console_writestring("HELLO WORLD");
    init_descriptor_tables();
   // init_timer(5000);
    mouseinit();
    setVesa(0x117);
}

void kernel_start()
{
}

void kernel_main()
{
    RectD(0,0,1023,767,150,150,150);
    while(1)
    {
        RectD(500,500,10,20,1000,1000,1000);
        mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
}
