#include <stddef.h>

void console_init(void);
void console_putch(char c);
void console_write(const char* data, size_t size);
void console_writestring(const char* data);
