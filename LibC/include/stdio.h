#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#ifdef _AQ_LIBC_Compile_

#include "library.h"

void _stdprint(const char* data, size_t data_length);
void _stdprintint(uint32_t in);

#else 

#endif  

typedef struct _FILE
{
	char        name[32];
	uint32_t    flags;
	uint32_t    fileLength;
	uint32_t    id;
	uint32_t    eof;
	uint32_t    position;
	uint32_t    currentCluster;
	uint32_t    deviceID;
}FILE;

FILE* stdin;

int printf(const char* restrict format, ...);
int scanf(const char* restrict format, ...);
void itoa(unsigned i,char* buf, unsigned base);
void fflush(FILE *ostream);

FILE* fopen(const char* filename, const char* mode);
void fclose(FILE* stream);
int fseek(FILE* stream, int offset, int whence);
int ftell(FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );

#endif
