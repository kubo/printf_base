/* License: public domain */
#ifndef PFB_PRINTF_H
#define PFB_PRINTF_H 1
#include <stdarg.h>

int pfb_printf(const char *format, ...);
int pfb_vprintf(const char *format, va_list ap);

#ifndef PFB_PRINTF_NO_COMPAT
#define printf pfb_printf
#define vprintf pfb_vprintf
#endif

#endif
