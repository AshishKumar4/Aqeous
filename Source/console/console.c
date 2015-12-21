#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <console.h>

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
	console_putentryat(c, console_color, consolecolumn, consolerow);
	if ( ++consolecolumn == VGA_WIDTH )
	{
		consolecolumn = 0;
		if ( ++consolerow == VGA_HEIGHT )
		{
			consolerow = 0;
		}
	}
}

void console_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		console_putchar(data[i]);
}

void console_writestring(const char* data)
{
	console_write(data, strlen(data));
}
int putchar(int ic)
{
	char c = (char) ic;
	console_write(&c, sizeof(c));
	return ic;
}
