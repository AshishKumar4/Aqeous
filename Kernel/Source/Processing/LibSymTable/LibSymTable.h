#ifndef LIBSYMTABLE_H
#define LIBSYMTABLE_H

#include "LIBRARY.h"

uint32_t* LIBSYMTABLE;

void init_libsym();
void LibSym_MapPdir(PageDirectory_t* pdir);

#endif
