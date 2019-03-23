#ifndef PRINTF_H
#define PRINTF_H

/* I use GCC-s stdarg.h!	*/
#include <stdarg.h>
#include <screen.h>
#include <sys/types.h>

int printf(const char* format, ...);
void putchar(char c);

#endif
