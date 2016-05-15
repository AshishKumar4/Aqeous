#ifndef STDIO_H
#define STDIO_H

#include <stdio.h>
#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdarg.h>
#include <keyboard.h>
#include <console.h>
#include <string.h>

extern void backspace();
extern int putchar(int ic);
extern void printint(uint32_t in);
extern void print64int(uint64_t in);

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

int printf(const char* restrict format, ...)
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
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}

char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned i,unsigned base,char* buf) {
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


uint8_t getch()
{
	asm volatile("sti");
	while(!call);
	asm volatile("cli");
	uint8_t a = *kb_buf;
	--kb_buf;
	printf("%x", a);
	asm volatile("sti");
	return a;
}

char getchar()
{
	volatile char key;
	volatile uint8_t a;
	back:
	asm volatile("sti");
	while(!call);
	asm volatile("cli");
	a = call;// *kb_buf;
	key = (char)keyboard_scancodes(a);

	if(key!='\r' && key!='\0' && key!='\b')
      printf("%c",key);
    call=0;
    if(key=='\b')
    {
      backspace();
  	  --kb_buf;
      goto back;
    }
	--kb_buf;
	asm volatile("sti");
    return key;
}

void getline(char* string)
{
  volatile uint8_t buff;
  call = 0;
  for(int i=0;;i++)
  {
  	asm volatile("sti");
    buff=getchar();
	asm volatile("cli");
    if(buff=='\r')
    {
      string[i]='\0';
      break;
    }
    else
		{
			string[i]=buff;
			//printf("a %c ",string[i]);
		}
  }
  printf("\n");
}

#endif // STDIO_H
