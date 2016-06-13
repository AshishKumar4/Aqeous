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
	VGA_buffer = (uint16_t*)(500*1024*1024);
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

void print64int(uint64_t in)
{
	console_write_dec(in);
}

/*************************************/


void _console_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	VGA_buffer[index] = make_vgaentry(c, color);
}

int _putchar(int ic)
{
	char c = (char) ic;

	char a=c;
	if(c=='\n') c=' ';
	else if(c=='\t') c=' ';
	_console_putentryat(c, console_color, consolecolumn, consolerow);
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
										buff[index] = VGA_buffer[index];
								}
						}
						for ( size_t y = 0; y < VGA_HEIGHT; y++ )
						{
								for ( size_t x = 0; x < VGA_WIDTH; x++ )
								{
										const size_t index = y * VGA_WIDTH + x;
										const size_t backindex = (y+1) * VGA_WIDTH + x;
										VGA_buffer[index] = buff[backindex];
								}
						}
		}
	}
	else if(a=='\t')
	{
			consolecolumn+=5;
	}
	return ic;
}

void _console_write_dec(uint32_t in)
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
    _printf(a);
}

void backspace()
{
		--consolecolumn;
		if(!consolecolumn)
		{
			--consolerow;
			consolecolumn = VGA_WIDTH;
		}
		_console_putentryat(' ', console_color, consolecolumn, consolerow);
}

void _printint(uint32_t in)
{
    if(in<=999999999)
        _console_write_dec(in);
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
        _console_write_dec(in/d);
        _console_write_dec(in%d);
    }
}

static void _print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		_putchar((int) ((const unsigned char*) data)[i]);
}

int _printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			_print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			_print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			_print(s, strlen(s));
		}
        else if(*format == 'i' ||*format == 'd')
        {
            format++;
            int c = va_arg (parameters, int);
            _printint(c);
        }
        else if(*format == 'l'||*format == 'x') //uint32_t
        {
            format++;
            uint32_t c = va_arg (parameters, uint32_t);
            _printint(c);
        }
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}
