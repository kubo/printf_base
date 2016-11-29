/* License: public domain */
#ifndef PFB_ASPRINTF_H
#define PFB_ASPRINTF_H 1
#include <stdarg.h>

int pfb_asprintf(char **strp, const char *format, ...);
int pfb_vasprintf(char **strp, const char *format, va_list ap);

#ifndef PFB_PRINTF_NO_COMPAT
#define asprintf pfb_asprintf
#define vasprintf pfb_vasprintf
#endif

#endif
