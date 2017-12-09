#ifndef _UNISTD_H_
#define _UNISTD_H_ 

ssize_t write(int fd, const void *buf, size_t nbytes);
ssize_t read(int fd, void *buf, size_t count);
//int open(const char *pathname, int flags); 
int open(const char *pathname, int flags, mode_t mode); 
int creat(const char *pathname, mode_t mode); 

#endif