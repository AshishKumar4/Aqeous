#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>

size_t consolerow;
size_t consolecolumn;
uint8_t console_color;
uint16_t* console_buffer;
uint16_t* VGA_buffer;

void (*mdbug)();
void (*vesa)(uint32_t mode);

void console_init(void);
void console_putch(char c);
void console_write(const char* data, size_t size);
void console_writestring(const char* data);
void console_write_dec(unsigned long int in);
void printint(uint32_t in);
uint8_t console_manager(char *inst);
void print64int(uint64_t in);

#endif // CONSOLE_H
