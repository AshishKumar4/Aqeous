#include "library.h"
#include "posixAbst.h"

uintptr_t openFile(const char* fname, const char* mode)
{
    //fname = PlaceStr_on_KernelMap(fname);
    _syscall_push((uint32_t)fname);
   // mode = PlaceStr_on_KernelMap(mode);
    _syscall_push((uint32_t)mode);
   // printf("[%s %s]", fname, mode);
    return _syscall_(__SYS_AQ_FOPEN__);
}

void closeFile(uintptr_t stream)
{
    _syscall_push((uint32_t)stream);
    _syscall_(__SYS_AQ_FCLOSE__);
}

int fileSeek(uintptr_t stream, int off, int whence)
{
    _syscall_push((uint32_t)stream);
    _syscall_push((uint32_t)off);
    _syscall_push((uint32_t)whence);
    return _syscall_(__SYS_AQ_FSEEK__);
}

int fileTell(uintptr_t stream)
{
    _syscall_push((uint32_t)stream);
    return _syscall_(__SYS_AQ_FTELL__);
}

int readFile(uintptr_t ptr, size_t size, size_t nmemb, uintptr_t stream)
{
    _syscall_push((uint32_t)ptr);
    _syscall_push((uint32_t)size);
    _syscall_push((uint32_t)nmemb);
    _syscall_push((uint32_t)stream);
    return _syscall_(__SYS_AQ_FREAD__);
}

int writeFile(uintptr_t ptr, size_t size, size_t count, uintptr_t stream)
{
    _syscall_push((uint32_t)ptr);
    //printf("<<%x>>", ptr);
    _syscall_push((uint32_t)size);
    _syscall_push((uint32_t)count);
    _syscall_push((uint32_t)stream);
    return _syscall_(__SYS_AQ_FWRITE__);
}

int cloneThread(char* name)
{
    _syscall_push((uint32_t)name);
    return _syscall_(__SYS_AQ_CLONETHREAD__);
}

int cloneProcess(char* name)
{
    _syscall_push((uint32_t)name);
    return _syscall_(__SYS_AQ_CLONEPROCESS__);
}