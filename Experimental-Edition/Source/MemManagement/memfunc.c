
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"

inline void *memcpy_faster(void * _dest, const void *_src, uint32_t _n)
{
    uint_fast32_t *sp = (uint_fast32_t*) _src;
    uint_fast32_t *dp = (uint_fast32_t*) _dest;
	//	_n /= sizeof(uint_fast32_t);
    for(; _n != 0; _n--) {
		*dp++ = *sp++;
	}
	return _dest;
}

inline void memset_faster(uint32_t *_s, int _c, uint32_t _n)
{
    for ( ; _n != 0; _n--) {
		*_s++ = _c;
	}
}
