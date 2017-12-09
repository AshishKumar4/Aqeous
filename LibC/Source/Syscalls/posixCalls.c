#include "syscalls.h"
#include "posixAbst.c"
#include "library.h"

void __attribute__((optimize("O0"))) exit(int code)
{
    _syscall_push(code);
    _syscall_(__SYS_EXIT__);
    //asm volatile("hlt");
    while(1)  
      asm volatile("int $50");
    while(1);
}

int fork(void)
{
    
}


size_t write(int fd, const void *buf, size_t nbytes)
{
    if(fd < 3)
    {
        switch(fd)
        {
            case 0: 
                    asm volatile("hlt");
                break;
            case 1:
                    STD_OUT_sz = 0;
                    STD_OUT_sz4 = 0;
                    _stdprint((char*)buf, nbytes);
                    _stdprint("\0", 1);
                    _syscall_push((uint32_t)STD_OUT_ptr);
                    _syscall_push((uint32_t)STD_OUT_sz);
                    _syscall_(__SYS_STDO__);
                break;
            case 2:
                    STD_OUT_sz = 0;
                    STD_OUT_sz4 = 0;
                    _stdprint((char*)buf, nbytes);
                    _stdprint("\0", 1);
                    _syscall_push((uint32_t)STD_OUT_ptr);
                    _syscall_push((uint32_t)STD_OUT_sz);
                    _syscall_(__SYS_STDO__);
                break;
        }
        return nbytes;
    }
    FILE* f = fdTable_Get(fd);
    fwrite(buf, 1, nbytes, f);
    return nbytes;
}

size_t read(int fd, void *buf, size_t count)
{
    if(fd < 3)
    {
        switch(fd)
        {
            case 0: 
                    asm volatile("hlt");
                break;
            case 1:
                    STD_OUT_sz = 0;
                    STD_OUT_sz4 = 0;
                    _stdprint((char*)buf, count);
                    _stdprint("\0", 1);
                    _syscall_push((uint32_t)STD_OUT_ptr);
                    _syscall_push((uint32_t)STD_OUT_sz);
                    _syscall_(__SYS_STDO__);
                break;
            case 2:
                    STD_OUT_sz = 0;
                    STD_OUT_sz4 = 0;
                    _stdprint((char*)buf, count);
                    _stdprint("\0", 1);
                    _syscall_push((uint32_t)STD_OUT_ptr);
                    _syscall_push((uint32_t)STD_OUT_sz);
                    _syscall_(__SYS_STDO__);
                break;
        }
        return count;
    }
    FILE* f = fdTable_Get(fd);
    fread(buf, 1, count, f);
    return count;
}
/*
int open(const char *pathname, int flags)
{

}*/

int open(const char *pathname, int flags)
{
    FILE *f = fopen(pathname, flags);
    return fdTable_Entry(f);
}
/*
int creat(const char *pathname, mode_t mode)
{

}
/*
int wait(void)
{

}

int exec(char *path, char **argv)
{

}

*/

char* getenv(const char* name)
{
    return NULL;
}