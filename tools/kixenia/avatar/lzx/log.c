#include <stdarg.h>
#include <stdio.h>
void xenia_log(const char* fmt, ...) { va_list a; va_start(a, fmt); vfprintf(stderr, fmt, a); fputc(10, stderr); va_end(a); }
