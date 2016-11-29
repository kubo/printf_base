/* License: public domain */
#ifndef PFB_FPRINTF_H
#define PFB_FPRINTF_H 1
#include <stdarg.h>

int pfb_fprintf(FILE *stream, const char *format, ...);
int pfb_vfprintf(FILE *stream, const char *format, va_list ap);

#ifndef PFB_PRINTF_NO_COMPAT
#define fprintf pfb_fprintf
#define vfprintf pfb_vfprintf
#endif

#endif
