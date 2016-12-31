#ifndef CONSOLE_h
#define CONSOLE_h
#include <stddef.h>

uint32_t consolerow;
uint32_t consolecolumn;
uint8_t console_color;
uint16_t volatile *console_dbuffer;
uint32_t volatile console_dbuffer_original, console_dbuffer_limit;
volatile uint32_t console_rows_skipped = 0;
volatile int console_skip = 0;
uint16_t volatile *console_buffer;
uint32_t default_console_color;

void (*mdbug)();
void (*vesa)(uint32_t mode);

void console_init(void);
void console_putch(char c);
void console_write(const char* data, size_t size);
void console_writestring(const char* data);
void console_write_dec(uint32_t in);
void printint(uint32_t in);
uint8_t console_manager(char *inst);
void print64int(uint64_t in);

#endif // CONSOLE_H
