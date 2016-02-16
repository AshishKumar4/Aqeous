#ifndef STRING_H
#define STRING_H
#include <string.h>
#include <sys/cdefs.h>

#include <stddef.h>

int memcmp(const void *_s1, const void *_s2, size_t _n)
{
	const unsigned char *us1 = (const unsigned char *)_s1;
	const unsigned char *us2 = (const unsigned char *)_s2;
	while(_n-- != 0) {
		if(*us1 != *us2){
			return (*us1 < *us2) ? -1 : +1;
		}
		us1++;
		us2++;
	}
	return 0;
}

inline void *memcpy(void * _dest, const void *_src, size_t _n)
{
    char *sp = (char*) _src;
    char *dp = (char*) _dest;
    for(; _n != 0; _n--) {
		*dp++ = *sp++;
	}
	return _dest;
}

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if ( dst < src )
		for ( size_t i = 0; i < size; i++ )
			dst[i] = src[i];
	else
		for ( size_t i = size; i != 0; i-- )
			dst[i-1] = src[i-1];
	return dstptr;
}


void memset(void *_s, int _c, size_t _n)
{
    char *temp = (char*) _s;
    for ( ; _n != 0; _n--) {
		*temp++ = _c;
	}
}

size_t strlen(const char *_s)
{
   size_t n;
   for(n=0; _s[n] != '\0'; n++) {
	   ;
   }
   return n;
}

int strcmp(const char *_s1, const char *_s2)
{
    for(; *_s1 == *_s2; ++_s1, ++_s2) {
        if(*_s1 == 0) {
            return 0;
		}
	}
    return *(unsigned char *)_s1 < *(unsigned char *)_s2 ? -1 : 1;
}

int strncmp(const char *_s1, const char *_s2, size_t _n)
{
	unsigned char uc1, uc2;
	if (_n==0)
		return 0;
	while (_n-- > 0 && *_s1 == *_s2) {
		if (_n==0 || *_s1 == '\0' ) {
			return 0;
		}
		_s1++;
		_s2++;
	}
	uc1 = (*(unsigned char *) _s1);
	uc2 = (*(unsigned char *) _s2);
	return ((uc1 < uc2) ? -1 : (uc1 > uc2));
}

char *strcpy(char *_dest, const char *_src)
{
    do {
      *_dest++ = *_src++;
    } while (*_src != 0);
	return _dest;
}

char *strcat(char *_dest, const char *_src)
{
   size_t n;

   n = strlen(_dest);
   strcpy(_dest + n, _src);

   return _dest;
}

int strcmpy(const char *c1, const char *c2)
{
    uint32_t buff=0;
    for(uint32_t i=0;i<=strlen(c1);i++)
    {
        for(uint32_t j=0;j<strlen(c2);j++)
        {
            if(c1[i+j]==c2[j])
            {
                buff+=1;
            }
        }
        if(buff==strlen(c2))
            return 0;
        else buff=0;
    }
    return 1;
}

char *strchr(const char *_s, int _c)
{
	while (*_s != (char)_c) {
		if (!*_s++) {
			return 0;
		}
	}
	return (char *)_s;
}

size_t strspn(const char *_s, const char *_accept)
{
	size_t ret=0;
	while(*_s && strchr(_accept,*_s++)) {
		ret++;
	}
	return ret;
}

size_t strcspn(const char *_s, const char *_reject)
{
	size_t ret=0;
	while(*_s) {
		if(strchr(_reject,*_s)) {
			return ret;
		} else {
			_s++,ret++;
		}
	}
	return ret;
}

char *strtok(char *_s, const char *_delim)
{
	static char* p=0;
	if(_s) {
		p=_s;
	} else if(!p) {
		return 0;
	}
	_s = p   + strspn(p,_delim);
	p  = _s  + strcspn(_s,_delim);
	if(p==_s) {
		return p=0;
	}
	p = *p ? *p=0,p+1 : 0;
	return _s;
}

char *strstr(const char *_haystack, const char *_needle)
{
	size_t needleLen;
	if(*_needle == '\0') {
		return (char *) _haystack;
	}
	needleLen = strlen(_needle);

	for(;(_haystack = strchr(_haystack, *_needle))!= NULL;_haystack++) {
		if (strncmp(_haystack, _needle, needleLen) == 0) {
			return (char *) _haystack;
		}
	}
	return NULL;
}

#endif // STRING_H
