#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>
void (*mdbug)();
void (*vesa)(uint32_t mode);
void console_init(void);
void console_putch(char c);
void console_write(const char* data, size_t size);
void console_writestring(const char* data);
void console_write_dec(unsigned long int in);
void printint(unsigned long int in);
uint8_t console_manager(char *inst);

#endif // CONSOLE_H
