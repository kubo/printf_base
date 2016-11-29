/* License: public domain */
#include <stdio.h>
#include "printf_base.h"
#include "pfb_fprintf.h"

int pfb_fprintf(FILE *stream, const char *format, ...)
{
    va_list ap;
    int rv;

    va_start(ap, format);
    rv = printf_base((pfb_putc_t)fputc, stream, format, ap);
    va_end(ap);
    return rv;
}

int pfb_vfprintf(FILE *stream, const char *format, va_list ap)
{
    return printf_base((pfb_putc_t)fputc, stream, format, ap);
}
