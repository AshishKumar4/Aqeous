#include "stdio.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include "string.h"
#include "syscalls.h"
#include "aqlib.h"

void _stdprint(const char* data, size_t data_length)
{
	char* stdo = (char*)STD_OUT_ptr;
	char* dl = (char*)data;
	for(size_t i = 0; i < data_length; i++)
	{
		stdo[STD_OUT_sz] = dl[i];
		++STD_OUT_sz;
	}
}

void _stdprintint(uint32_t in)
{
  char tmp[16];
	itoa(in, tmp, 10);
	_stdprint(tmp, strlen(tmp));
}

char* _stdgetin(uint32_t* stream, int size, char dlimiter)
{
	uint32_t* st = stream;
	char* sts = (char*)st;

	char* fst = &sts[*st];
	int i = 0;
	for(; i < size; i++)
	{
		if(fst[i] == dlimiter)
		{
			fst[i] = '\0';
			break;
		}
	}
	st[1] = i;
	st[0] += st[1];
	return fst;
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	STD_OUT_sz = 0;
	STD_OUT_sz4 = 0;

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
			char c = (char) va_arg(parameters, int);
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
	_stdprint("\0", 1);
	va_end(parameters);
	_syscall_push((uint32_t)STD_OUT_ptr);
	_syscall_push((uint32_t)STD_OUT_sz);
	STD_OUT_sz = 0;
	STD_OUT_sz4 = 0;
	return _syscall_(__SYS_STDO__);
}


int scanf(const char* restrict format, ...)
{
	_syscall_push((uint32_t)STD_IN_ptr);
	_syscall_push((uint32_t)STD_IN_normal);
//	printf("\n<(%x)>", format);
	_syscall_(__SYS_STDI__);
//	printf("\n<(%x)>", format);
	asm volatile("int $50");
//	printf("\n<(%x)>", format);
	
	uint32_t size = STD_IN_ptr[1];

	va_list parameters;
	va_start(parameters, format);
/*
	STD_OUT_sz = 0;
	STD_OUT_sz4 = 0;
*/
	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

//	printf("\n<%x>", format);
//	printf("%s", format);
//	printf("%c", *format);
	while ( *format != '\0' )
	{
		//printf("Y");
		if ( *format != '%' )
		{
		_print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
		//	_stdgetin(format, amount);
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
			char* c = (char*) va_arg(parameters, char*);
			*c = (_stdgetin(std_in, 1, ' '))[0];
		}
		else if ( *format == 's' )
		{
			format++;
			char* s = (char*)va_arg(parameters, char*);
			char* o = _stdgetin(std_in, size, ' ');
			if(s) 
			{
				strcpy(s, o);
			}
		}
		else if(*format == 'i' ||*format == 'd')
		{
				format++;
				int* c = (int*)va_arg (parameters, uintptr_t);
				*c = StrToInt(_stdgetin(std_in, size, ' '));
		}
		else if(*format == 'l' || *format == 'u') //uint32_t
		{
				format++;
				uint32_t* c = (uint32_t*)va_arg (parameters, uintptr_t);
				*c = StrToInt(_stdgetin(std_in, size, ' '));
		}
		else if(*format == 'p'||*format == 'g') //color
		{
				format++;
				int c = (int)va_arg (parameters, int);
				if(!c)
					console_color = default_console_color;
				else
					console_color = c;
		}
		else if(*format == 'f') //float
		{
				format++;
				/*double c = va_arg (parameters, double);
				if(c < 0)
				{
					_stdgetin("-", 1);
					c = -c;
				}
				uint32_t in = (uint32_t)(int)c;
				double d = (double)c;
				d -= in;
				d *= 10000000;
				_stdgetint(in);
				_stdgetin(".", 1);
				in = (int)d;
				_stdgetint(in);*/
		}
		else if(*format == 'x') //uint32_t
		{
				format++;
			/*	_stdgetin("0x", 2);
				uint32_t c = va_arg (parameters, uint32_t);
				char tmp[10];
				itoa(c, tmp, 16);
				_stdgetint(tmp, strlen(tmp));*/
		}
		else
		{
			goto _incomprehensible_conversion;
		}
	}
	
	
	va_end(parameters);
	return size;
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
   for (opos=0; opos<top; pos--,opos++)
	 {
      buf[opos] = tbuf[pos];
   }
   buf[opos] = 0;
}

void fflush(FILE *ostream)
{

}

/******************** FILE STREAM *********************/


FILE* fopen(const char* filename, const char* mode)
{
	return (FILE*)openFile(filename, mode);
}

void fclose(FILE* stream)
{
	closeFile((uintptr_t)stream);
}

int fseek(FILE* stream, int offset, int whence)
{
	return fileSeek((uintptr_t)stream, offset, whence);
}

int ftell(FILE* stream)
{
	return fileTell((uintptr_t)stream);
}

void rewind(FILE* stream)
{
//	stream->fstream_ptr = 0;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	return (size_t)readFile((uintptr_t)ptr, size, nmemb, (uintptr_t)stream);
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{
	return (size_t)writeFile((uintptr_t)ptr, size, count, (uintptr_t) stream);
}
/*/


FILE* fopen(const char* filename, const char* mode)
{
	if(!file_loadOGP((char*)filename))
	{
		printf("\n\"%s\" File dosent exist", filename);
	}
	FILE* stream = (FILE*)malloc(sizeof(FILE));
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

//*/