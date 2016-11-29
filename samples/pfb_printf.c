/* License: public domain */
#include <stdio.h>
#include "printf_base.h"
#include "pfb_printf.h"

int pfb_printf(const char *format, ...)
{
    va_list ap;
    int rv;

    va_start(ap, format);
    rv = printf_base((pfb_putc_t)fputc, stdout, format, ap);
    va_end(ap);
    return rv;
}

int pfb_vprintf(const char *format, va_list ap)
{
    return printf_base((pfb_putc_t)fputc, stdout, format, ap);
}
