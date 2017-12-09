#include "posixAbst.h"

void create_fdTable()
{
    glb_fd_tbl[0] = (uint32_t)stdin;
}

uint32_t fdTable_Entry(FILE* f)
{
    glb_fd_tbl[glb_fd] = (uint32_t)f;
    ++glb_fd;
    return (glb_fd - 1);
}

FILE* fdTable_Get(uint32_t fd)
{
    return (FILE*)glb_fd_tbl[fd];
}
/*
char* PlaceStr_on_KernelMap(char* str)
{
    int n = strlen(str);
    char* dest = &KernelMessageSpace[1];
    dest += KernelMessageSpace[0];
    KernelMessageSpace[0] += n + 2;
    strncpy(dest, str, n);
    return dest;
}*/