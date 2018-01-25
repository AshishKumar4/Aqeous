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
volatile int console_Dchange = 0;

void (*mdbug)();
void (*vesa)(uint32_t mode);

void console_init(void);
void console_updateCursor();
inline void console_setcolor(uint8_t color);
inline void console_putentryat(char c, uint8_t color, size_t x, size_t y);
int console_putcharF(char ic);
int console_putcharK(char c);

int (*console_putchar)(char);

void set_ConsoleCursor(int row, int col);
void backspace();
void clrline();

#endif // CONSOLE_H
