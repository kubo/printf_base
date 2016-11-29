/* License: public domain */
#ifndef PFB_SNPRINTF_H
#define PFB_SNPRINTF_H 1
#include <stdarg.h>

int pfb_snprintf(char *str, size_t size, const char *format, ...);
int pfb_vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifndef PFB_PRINTF_NO_COMPAT
#define snprintf pfb_snprintf
#define vsnprintf pfb_vsnprintf
#endif

#endif
