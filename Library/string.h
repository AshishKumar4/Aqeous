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

void memset(void *_s, int _c, size_t _n)
{
    char *temp = (char*) _s;
    for ( ; _n != 0; _n--) {
		*temp++ = _c;
	}
}
