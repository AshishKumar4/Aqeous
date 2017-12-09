#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <console.h>
#include <acpi.h>
#include <vesa.h>

#include <vga.h>


void console_init(void)
{
	consolerow = 0;
	consolecolumn = 0;
	console_color = make_color(COLOR_WHITE, COLOR_BLACK);
	default_console_color = console_color;
	console_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			console_buffer[index] = make_vgaentry(' ', console_color);
		}
	}
	console_dbuffer = console_buffer;
	console_updateCursor();
	console_putchar = &console_putcharK;
}

void set_ConsoleCursor(int row, int col)
{
   unsigned short position=(row*80) + col;

   // cursor LOW port to vga INDEX register
   outb(0x3D4, 0x0F);
   outb(0x3D5, ((unsigned char)(position&0xFF)));
   // cursor HIGH port to vga INDEX register
   outb(0x3D4, 0x0E);
   outb(0x3D5, ((unsigned char )((position>>8)&0xFF)));
}

void console_updateCursor()
{
	set_ConsoleCursor(consolerow, consolecolumn);
}

inline void console_setcolor(uint8_t color)
{
	console_color = color;
}

inline void console_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	console_dbuffer[index] = make_vgaentry(c, color);
	console_Dchange = 1;
}

int console_putcharF(char ic)
{
	if(ic == '\n')
	{
		consolecolumn = 0;
		++consolerow;
		if(consolerow >= VGA_HEIGHT)
		{
			//--consolerow;
			++console_rows_skipped;
		//	++console_skip;
			console_dbuffer += (VGA_WIDTH);
			--consolerow;
		}
		return (int)ic;
	}
	else if(ic == '\t')
	{
		consolecolumn += 5;
	}
	else
	{
		console_putentryat(ic, console_color, consolecolumn, consolerow);
	}
	if(++consolecolumn >= VGA_WIDTH)
	{
		consolecolumn = 0;
		++consolerow;
		if(consolerow >= VGA_HEIGHT)
		{
			//--consolerow;
			++console_rows_skipped;
		//	++console_skip;
			console_dbuffer += (VGA_WIDTH);
			--consolerow;
		}
		return (int)ic;
	}
	return (int)ic;
}

int console_putcharK(int c)
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
                    buff[index] = console_dbuffer[index];
                }
            }
            for ( size_t y = 0; y < VGA_HEIGHT; y++ )
            {
                for ( size_t x = 0; x < VGA_WIDTH; x++ )
                {
                    const size_t index = y * VGA_WIDTH + x;
                    const size_t backindex = (y+1) * VGA_WIDTH + x;
                    console_dbuffer[index] = buff[backindex];
                }
            }
		}
	}
	else if(a=='\t')
    {
        consolecolumn+=5;
    }
}

/*************************************/

void backspace()
{
		--consolecolumn;
		if(!consolecolumn)
		{
			--consolerow;
			consolecolumn = VGA_WIDTH;
		}
		console_putentryat(' ', console_color, consolecolumn, consolerow);
}

void clrline()
{

}

