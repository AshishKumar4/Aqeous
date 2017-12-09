#ifndef _POSIXCALLS_H_
#define _POSIXCALLS_H_

#include "syscalls.h"
#include "posixAbst.c"
#include "library.h"

void exit(int code);
int fork(void);

size_t write(int fd, const void *buf, size_t nbytes);
size_t read(int fd, void *buf, size_t count);
int open(const char *pathname, int flags);
int creat(const char *pathname, mode_t mode);
int wait(void);
int exec(char *path, char **argv);
char* getenv(const char* name);

#endif

