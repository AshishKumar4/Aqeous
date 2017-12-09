#ifndef _STRING_H_
#define _STRING_H_

#include "stdint.h"
#include <stddef.h>


#ifdef _AQ_LIBC_Compile_

#else 
#endif 

void *faster_memcpy(void * _dest, const void *_src, uint32_t _n);

void faster_memset(void *_s, uint32_t _c, size_t _n);

int memcmp(const void *_s1, const void *_s2, size_t _n);

void *memcpy(void * _dest, void *_src, size_t _n);

void* memmove(void* dstptr, const void* srcptr, size_t size);


void memset(void *_s, int _c, size_t _n);

int memcmp_fast(const void *_s1, const void *_s2, size_t _n);

void *memcpy_fast(void * _dest, const void *_src, size_t _n);

void* memmove_fast(void* dstptr, const void* srcptr, size_t size);

void memset_fast(void *_s, int _c, size_t _n);

size_t strlen(const char *_s);

int strcmp(const char *_s1, const char *_s2);

int strcmpx(const char *_s1, const char *_s2);

int strncmp(const char *_s1, const char *_s2, size_t _n);

char *strcpy(char *_dest, const char *_src);

char *strncpy(char *_dest, const char *_src, int size);

char *strcat(char *_dest, const char *_src);

int strcmpy(const char *c1, const char *c2);

char *strchr(const char *_s, int _c);

size_t strspn(const char *_s, const char *_accept);

size_t strcspn(const char *_s, const char *_reject);

char *strtok(char *_s, const char *_delim);

char *strstr(const char *_haystack, const char *_needle);

uint32_t stroccr(char* str, const char c);

#endif // STRING_H
