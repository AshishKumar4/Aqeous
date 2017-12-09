#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <console.h>
#include <common.h>
#include "vfs.h"

#include "IO_Handling/IO_Devices/Console/Console_handle.h"


int putchar(char ic)
{
	console_putchar(ic);
	//console_updateCursor();
}

static void printnum(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint32_t x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    console_putchar(buf[i]);
}

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		console_putchar((int) ((const unsigned char*) data)[i]);
}

void _stdprintint(uint32_t in)
{
  char tmp[16];
	itoa(in, tmp, 10);
	_stdprint(tmp, strlen(tmp));
}

void printint(uint32_t in)
{
  char tmp[16];
	itoa(in, tmp, 10);
	print(tmp, strlen(tmp));
}

void print64int(uint64_t in)
{
  char tmp[16];
	itoa(in, tmp, 10);
	print(tmp, strlen(tmp));
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
			print(format, amount);
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

int _stdprintf(const char* restrict format, ...)
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
			_stdprint(format, amount);
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
			_stdprint(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			_stdprint(s, strlen(s));
		}
		else if(*format == 'i' ||*format == 'd')
		{
				format++;
				int c = va_arg (parameters, int);
				if(c < 0)
				{
					_stdprint("-", 1);
					c = -c;
				}
				_stdprintint(c);
		}
		else if(*format == 'l' || *format == 'u') //uint32_t
		{
				format++;
				uint32_t c = va_arg (parameters, uint32_t);
				_stdprintint(c);
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
					_stdprint("-", 1);
					c = -c;
				}
				uint32_t in = (uint32_t)(int)c;
				double d = (double)c;
				d -= in;
				d *= 10000000;
				_stdprintint(in);
				_stdprint(".", 1);
				in = (int)d;
				_stdprintint(in);
		}
		else if(*format == 'x') //uint32_t
		{
				format++;
				_stdprint("0x", 2);
				uint32_t c = va_arg (parameters, uint32_t);
				char tmp[10];
				itoa(c, tmp, 16);
				_stdprint(tmp, strlen(tmp));
		}
		else
		{
			goto _incomprehensible_conversion;
		}
	}

	va_end(parameters);
	plock = 0;
	;//UNLOCK(printlock);
	return written;
}

int printf(const char* restrict format, ...)
{
	;//LOCK(printlock);
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
			print(format, amount);
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
				if(c < 0)
				{
					print("-", 1);
					c = -c;
				}
				printnum(c, 10, 1);
		}
		else if(*format == 'l' || *format == 'u') //uint32_t
		{
				format++;
				uint32_t c = va_arg (parameters, uint32_t);
				printnum(c, 10, 0);
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
					print("-", 1);
					c = -c;
				}
				uint32_t in = (uint32_t)(int)c;
				double d = (double)c;
				d -= in;
				d *= 10000000;
				printnum(in, 10, 0);
				print(".", 1);
				in = (int)d;
				printnum(in, 10, 0);
		}
		else if(*format == 'x') //uint32_t
		{
				format++;
				print("0x", 2);
				uint32_t c = va_arg (parameters, uint32_t);
				/*char tmp[10];
				itoa(c, tmp, 16);
				_print(tmp, strlen(tmp));*/
				printnum(c, 16, 0);
		}
		else
		{
			goto _incomprehensible_conversion;
		}
	}

	va_end(parameters);
	plock = 0;
	//console_updateCursor();
	;//UNLOCK(printlock);
	return written;
}

char _tbuf[32];
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
	_tbuf[pos] = bchars[i % base];
      pos++;
      i /= base;
   }
   top=pos--;
   for (opos=0; opos<top; pos--,opos++)
	 {
      buf[opos] = _tbuf[pos];
   }
   buf[opos] = 0;
}

int puts(const char* string)
{
	return printf("%s\n", string);
}

/******************** FILE STREAM *********************/


FILE* fopen(const char* filename, const char* mode)
{
	uintptr_t handle = (uintptr_t)(Current_Partition->FS.load((char*)filename));
	if(!handle)
	{
		printf("\n\"%s\" File dosent exist", filename);
		if(stroccr(mode, 'w'))
		{
			// Create the file!
			Current_Partition->FS.mkfl(filename, NULL);
			handle = (uintptr_t)(Current_Partition->FS.load((char*)filename));
			if(!handle)
			{
				printf("\nStill not possible!");
				return NULL;
			}
		}
	}
	FILE* stream = (FILE*)kmalloc(sizeof(FILE));
	strcpy(stream->mode, mode);
	stream->handle = handle;
	stream->fsize = Current_Partition->FS.getSize(handle);
	//printf("\n[%d]", stream->fsize);
	stream->fstream_ptr = 0;
	return stream;
}

void fclose(FILE* stream)
{
	//file_closeOGP((char*)stream->handle->full_path);
	Current_Partition->FS.close(stream->handle);
}

int fseek(FILE* stream, int offset, int whence)
{
	switch(whence)
	{
		case SEEK_CUR:
				stream->fstream_ptr = offset;
			break;
		case SEEK_END:
				stream->fstream_ptr = stream->fsize - offset;
			break;
		case SEEK_SET:
				stream->fstream_ptr += offset;
			break;
		default:;
	}
	return 0;//stream->fstream_ptr;
}

int ftell(FILE* stream)
{
	return stream->fstream_ptr;
}

void rewind(FILE* stream)
{
	stream->fstream_ptr = 0;
}

int fsize(FILE* stream)
{
	return stream->fsize;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	Current_Partition->FS.read((uint32_t*)ptr, (uint32_t)stream->fstream_ptr, (uint32_t)(nmemb*size), stream->handle);
	stream->fstream_ptr += size*nmemb;
	return size*nmemb;
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{/*
	if(!stream->fsize)
		printf("\n <%d>", file_writeAppend((uint32_t*)ptr, size*count, (char*)stream->handle->full_path));
	else
		file_editFM((uint32_t)stream->fstream_ptr, 0, (uint32_t*)ptr, (uint32_t)(size*count), (char*)stream->handle->full_path);*/
	Current_Partition->FS.write((uint32_t*)ptr, (uint32_t)stream->fstream_ptr, size*count, stream->handle, 0);
	stream->fstream_ptr += size*count;
	return size*count;
}

/*

FILE* fopen(const char* filename, const char* mode)
{
	if(!file_loadOGP((char*)filename))
	{
		printf("\n\"%s\" File dosent exist", filename);
		if(stroccr(mode, 'w'))
		{
			// Create the file!
			Current_Partition->FS.mkfl(filename, NULL);
			if(!file_loadOGP((char*)filename))
			{
				printf("\nStill not possible!");
				return NULL;
			}
		}
	}
	FILE* stream = (FILE*)kmalloc(sizeof(FILE));
	strcpy(stream->mode, mode);
	stream->handle = file_searchOGP((char*)filename);
	stream->fsize = file_size((char*)filename);//((File_t*)(stream->handle->file))->sz;//ftell(stream);
	stream->fstream_ptr = 0;
	return stream;
}

void fclose(FILE* stream)
{
	file_closeOGP((char*)stream->handle->full_path);
}

int fseek(FILE* stream, int offset, int whence)
{
	switch(whence)
	{
		case SEEK_CUR:
				stream->fstream_ptr = offset;
			break;
		case SEEK_END:
				stream->fstream_ptr = stream->fsize - offset;
			break;
		case SEEK_SET:
				stream->fstream_ptr += offset;
			break;
		default:;
	}
	return 0;//stream->fstream_ptr;
}

int ftell(FILE* stream)
{
	return stream->fstream_ptr;
}

void rewind(FILE* stream)
{
	stream->fstream_ptr = 0;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	file_readTM((uint32_t*)ptr, (uint32_t)stream->fstream_ptr, (uint32_t)(nmemb*size), (char*)stream->handle->full_path);
	stream->fstream_ptr += size*nmemb;
	return size*nmemb;
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{
	if(!stream->fsize)
		printf("\n <%d>", file_writeAppend((uint32_t*)ptr, size*count, (char*)stream->handle->full_path));
	else
		file_editFM((uint32_t)stream->fstream_ptr, 0, (uint32_t*)ptr, (uint32_t)(size*count), (char*)stream->handle->full_path);
	stream->fstream_ptr += size*count;
	return size*count;
}

*/
