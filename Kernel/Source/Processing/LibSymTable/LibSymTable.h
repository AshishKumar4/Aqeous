#ifndef LIBSYMTABLE_H
#define LIBSYMTABLE_H

#include "LIBRARY.h"

uint32_t* LIBSYMTABLE;

void kill_tmp();

void _malloc();

void _stdout();

void _stdin();
void test_sys();

void LibSym_MapPdir(PageDirectory_t* pdir);

#endif
