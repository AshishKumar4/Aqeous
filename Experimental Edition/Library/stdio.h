#ifndef STDIO_H
#define STDIO_H
#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
extern int putchar(int ic);
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
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}

void printint(int in)
{
    int d=1,ln=0,b=in,arr[10];
    char c[2],a[10];
    for(int i=0;b;i++)
    {
        b=b/10;
        ++ln;
    }
    b=in;
    for(int i=0;i<ln;i++) d=d*10;
    d=d/10;
    int i;
    for(i=0;i<ln;i++)
    {
        a[i]=48+b/d;
        b=b%d;
        d=d/10;
    }
    a[i]='\0';
    printf(a);
}

int puts(const char* string)
{
	return printf("%s\n", string);
}

#endif // STDIO_H
