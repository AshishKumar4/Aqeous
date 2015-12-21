#include <graphics.c>
#include <console.c>
void kernel_early()
{
    kmalloc(1);
    setVesa(0x117);
}

void kernel_start()
{
}

void kernel_main()
{
    RectD(0,0,1023,767,90,90,90);
    while(1)
    {
        RectD(500,500,10,20,1000,1000,1000);
        DBuff();
    }
}
