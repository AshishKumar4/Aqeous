#ifndef POSIX_ABST_H
#define POSIX_ABST_H 

#include "library.h"

void create_fdTable();
uint32_t fdTable_Entry(FILE* f);
FILE* fdTable_Get(uint32_t fd);
//char* PlaceStr_on_KernelMap(char* str);

#endif 