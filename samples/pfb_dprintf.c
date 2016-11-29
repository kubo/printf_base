/* License: public domain */
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#define write _write
#else
#include <unistd.h>
#endif
#include "printf_base.h"
#include "pfb_dprintf.h"

int pfb_dprintf(int fd, const char *format, ...)
{
    va_list ap;
    int rv;

    va_start(ap, format);
    rv = printf_base_with_buffering((pfb_write_t)write, (void*)(size_t)fd, format, ap);
    va_end(ap);
    return rv;
}

int pfb_vdprintf(int fd, const char *format, va_list ap)
{
    return printf_base_with_buffering((pfb_write_t)write, (void*)(size_t)fd, format, ap);
}
