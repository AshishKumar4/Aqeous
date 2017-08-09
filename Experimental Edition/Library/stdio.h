#ifndef STDIO_H
#define STDIO_H

#include "stdint-gcc.h"
#include <sys/cdefs.h>
#include <stdbool.h>
#include "vfs.h"

extern void kb_getline(char* str, uint32_t length);

uint32_t StrToInt(char *str);

extern volatile int multitasking_ON;

int putchar(int ic);
void printint(uint32_t in);
extern void print64int(uint64_t in);
void _printint(uint32_t in);
int _putchar(char ic);

int _stdprintf(const char* restrict format, ...);
int _printf(const char* restrict format, ...);
int scanf(const char* restrict format, ...);

void itoa(unsigned i,char* buf, unsigned base);


#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

FILE* fopen(const char* filename, const char* mode);
void fclose(FILE* stream);
int fseek(FILE* stream, int offset, int whence);
int ftell(FILE* stream);
void rewind(FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
#endif // STDIO_H
