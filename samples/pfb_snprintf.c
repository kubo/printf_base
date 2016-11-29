/* License: public domain */
#include <stdio.h>
#include "printf_base.h"
#include "pfb_snprintf.h"

int pfb_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    int rv;

    va_start(ap, format);
    rv = pfb_vsnprintf(str, size, format, ap);
    va_end(ap);
    return rv;
}

typedef struct {
    char *str;
    char *end;
} arg_t;

static int put_char(char chr, void *handle)
{
    arg_t *arg = (arg_t *)handle;
    if (arg->str < arg->end) {
        *(arg->str++) = chr;
    }
    return 0;
}

int pfb_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    arg_t arg;
    int rv;

    arg.str = str;
    arg.end = str + size;
    rv = printf_base((pfb_putc_t)put_char, &arg, format, ap);
    if (rv == -1) {
        return -1;
    }
    if (arg.str < arg.end) {
        *arg.str = '\0';
    }
    return rv;
}
