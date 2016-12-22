#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <console.h>
#include <acpi.h>

#include <vga.h>

size_t consolerow;
size_t consolecolumn;
uint8_t console_color;
uint16_t* console_buffer;

void console_init(void)
{
	consolerow = 0;
	consolecolumn = 0;
	console_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	console_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			console_buffer[index] = make_vgaentry(' ', console_color);
		}
	}
}

void console_setcolor(uint8_t color)
{
	console_color = color;
}

void console_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	console_buffer[index] = make_vgaentry(c, color);
}

void console_putchar(char c)
{
    char a=c;
    if(c=='\n') c=' ';
    else if(c=='\t') c=' ';
	console_putentryat(c, console_color, consolecolumn, consolerow);
	if ( ++consolecolumn == VGA_WIDTH||a=='\n')
	{
		consolecolumn = 0;
		if ( ++consolerow == VGA_HEIGHT )
		{
		    --consolerow;
		    uint16_t buff[((VGA_HEIGHT+1)*VGA_WIDTH)+VGA_WIDTH+1];
		    for(size_t x=0;x<(((VGA_HEIGHT+1)*VGA_WIDTH)+VGA_WIDTH+1);x++)
            {
                buff[x] = 0;
            }
		    for ( size_t y = 0; y < VGA_HEIGHT; y++ )
            {
                for ( size_t x = 0; x < VGA_WIDTH; x++ )
                {
                    const size_t index = y * VGA_WIDTH + x;
                    buff[index] = console_buffer[index];
                }
            }
            for ( size_t y = 0; y < VGA_HEIGHT; y++ )
            {
                for ( size_t x = 0; x < VGA_WIDTH; x++ )
                {
                    const size_t index = y * VGA_WIDTH + x;
                    const size_t backindex = (y+1) * VGA_WIDTH + x;
                    console_buffer[index] = buff[backindex];
                }
            }
		}
	}
	else if(a=='\t')
    {
        consolecolumn+=5;
    }
}

void console_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		console_putchar(data[i]);
}

void console_writestring(const char* data)
{
    printf(data);
	//console_write(data, strlen(data));
}
int putchar(int ic)
{
	char c = (char) ic;
	console_write(&c, sizeof(c));
	return ic;
}

void console_write_dec(uint32_t in)
{
    uint32_t d=1,ln=0,b=in;
    char a[40];
    for(uint32_t i=0;b;i++)
    {
        b=b/10;
        ++ln;
    }
    b=in;
    for(uint32_t i=0;i<ln;i++) d=d*10;
    d=d/10;
    uint32_t i;
    for(i=0;i<ln;i++)
    {
        a[i]=48+b/d;
        b=b%d;
        d=d/10;
    }
    a[i]='\0';
    console_writestring(a);
}

void printint(uint32_t in)
{
    if(in<=999999999)
        console_write_dec(in);
    else
    {
        uint32_t d=1,b=in,c=0;
        for(uint32_t i=0;b;i++)
        {
            b=b/10;
            ++c;
        }
        c=c-9;
        for(uint32_t i=0;i<c;i++) d=d*10;
        console_write_dec(in/d);
        console_write_dec(in%d);
    }
}

void backspace()
{
		--consolecolumn;
		console_putentryat(' ', console_color, consolecolumn, consolerow);
}
