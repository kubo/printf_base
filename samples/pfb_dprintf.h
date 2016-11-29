/* License: public domain */
#ifndef PFB_DPRINTF_H
#define PFB_DPRINTF_H 1
#include <stdarg.h>

int pfb_dprintf(int fd, const char *format, ...);
int pfb_vdprintf(int fd, const char *format, va_list ap);

#ifndef PFB_PRINTF_NO_COMPAT
#define dprintf pfb_dprintf
#define vdprintf pfb_vdprintf
#endif

#endif
