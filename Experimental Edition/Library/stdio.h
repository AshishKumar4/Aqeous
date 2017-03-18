#ifndef STDIO_H
#define STDIO_H

#include <stdio.h>
#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <console.h>
#include <common.h>

void* (*kmalloc)(uint32_t);
extern void kb_getline(char* str, uint32_t length);

uint32_t StrToInt(char *str);

extern volatile int multitasking_ON;
int putchar(int ic);
void printint(uint32_t in);
extern void print64int(uint64_t in);
void _printint(uint32_t in);
int _putchar(char ic);

int _printf(const char* restrict format, ...);

int scanf(const char* restrict format, ...);

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static void _print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		_putchar((char)((const unsigned char*) data)[i]);
}

int plock = 0;

int scanf(const char* restrict format, ...)
{
	while(plock);
	plock = 1;
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while(*format != '\0')
	{
		if ( *format != '%' )
		{
		_print_c:
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
			goto _print_c;

		if ( rejected_bad_specifier )
		{
		_incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto _print_c;
		}
		if( *format == 'd')
		{
			format++;
			uint32_t* c = (uint32_t*)va_arg(parameters, uint32_t);
			char st[15];
			kb_getline(st, 15);
			*c = StrToInt(st);
		}
		else if( *format == 's')
		{
			format++;
			uint32_t* c = (uint32_t*)va_arg (parameters, uint32_t);
			char st[50];
			kb_getline(st, 50);
			strcpy((char*)c, st);
		}
		else
		{
			goto _incomprehensible_conversion;
		}
	}
	va_end(parameters);

	plock = 0;
	return written;
}


int printf(const char* restrict format, ...)
{
	/*while(plock);
	plock = 1;*/
	LOCK(printlock);
	if(multitasking_ON)
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
			_print_c:
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
				goto _print_c;

			if ( rejected_bad_specifier )
			{
			_incomprehensible_conversion:
				rejected_bad_specifier = true;
				format = format_begun_at;
				goto _print_c;
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
							if(c < 0)
							{
								_print("-", 1);
								c = -c;
							}
	            _printint(c);
	        }
	        else if(*format == 'l'||*format == 'x') //uint32_t
	        {
	            format++;
	            uint32_t c = va_arg (parameters, uint32_t);
	            _printint(c);
	        }
	        else if(*format == 'p'||*format == 'g') //color
	        {
	            format++;
	            int c = va_arg (parameters, int);
							if(!c)
								console_color = default_console_color;
							else
								console_color = c;
	        }

	        else if(*format == 'f') //float
	        {
	            format++;
	            double c = va_arg (parameters, double);
							if(c < 0)
							{
								_print("-", 1);
								c = -c;
							}
							uint32_t in = (uint32_t)(int)c;
							double d = (double)c;
							d -= in;
							d *= 10000000;
							_printint(in);
							_print(".", 1);
							in = (int)d;
							_printint(in);
	        }
			else
			{
				goto _incomprehensible_conversion;
			}
		}

		va_end(parameters);
		plock = 0;

		UNLOCK(printlock);
		return written;
	}
	else
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
				print(format, amount);
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
				print(&c, sizeof(c));
			}
			else if ( *format == 's' )
			{
				format++;
				const char* s = va_arg(parameters, const char*);
				print(s, strlen(s));
			}
	        else if(*format == 'i' ||*format == 'd')
	        {
	            format++;
	            int c = va_arg (parameters, int);
	            printint(c);
	        }
	        else if(*format == 'l'||*format == 'x') //uint32_t
	        {
	            format++;
	            uint32_t c = va_arg (parameters, uint32_t);
	            printint(c);
	        }
	        else if(*format == 'a'||*format == 'L') //uint32_t
	        {
	            format++;
	            uint64_t c = va_arg (parameters, uint64_t);
	            print64int(c);
	        }
	        else if(*format == 'p'||*format == 'g') //color
	        {
	            format++;
	            int c = va_arg (parameters, int);
							if(!c)
								console_color = default_console_color;
							else
								console_color = c;
	        }
			else
			{
				goto incomprehensible_conversion;
			}
		}

		va_end(parameters);

		plock = 0;
		UNLOCK(printlock);
		return written;
	}
}

char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned i,char* buf, unsigned base)
{
   int pos = 0;
   int opos = 0;
   int top = 0;

   if (i == 0 || base > 16) {
      buf[0] = '0';
      buf[1] = '\0';
      return;
   }

   while (i != 0) {
      tbuf[pos] = bchars[i % base];
      pos++;
      i /= base;
   }
   top=pos--;
   for (opos=0; opos<top; pos--,opos++) {
      buf[opos] = tbuf[pos];
   }
   buf[opos] = 0;
}

int puts(const char* string)
{
	return printf("%s\n", string);
}

#endif // STDIO_H
