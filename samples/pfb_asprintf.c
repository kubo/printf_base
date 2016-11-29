/* License: public domain */
#include <stdlib.h>
#include <string.h>
#include "printf_base.h"
#include "pfb_asprintf.h"

int pfb_asprintf(char **strp, const char *format, ...)
{
    va_list ap;
    int rv;

    va_start(ap, format);
    rv = pfb_vasprintf(strp, format, ap);
    va_end(ap);
    return rv;
}

typedef struct buf {
    struct buf *next;
    int used;
    char buf[4096 - sizeof(void*) - sizeof(int)];
} buf_t;

static int output_char(int chr, buf_t **buf)
{
    if ((*buf)->used == sizeof((*buf)->buf)) {
        (*buf)->next = malloc(sizeof(buf_t));
        if ((*buf)->next == NULL) {
            return -1;
        }
        *buf = (*buf)->next;
        (*buf)->next = NULL;
        (*buf)->used = 0;
    }
    (*buf)->buf[(*buf)->used++] = chr;
    return 0;
}

int pfb_vasprintf(char **strp, const char *format, va_list ap)
{
    buf_t buf;
    buf_t *bufp = &buf;
    buf_t *bufp_next;
    int rv;
    buf.next = NULL;
    buf.used = 0;

    rv = printf_base((pfb_putc_t)output_char, &bufp, format, ap);
    if (rv != -1) {
        char *str = *strp = malloc(rv + 1);
        for (bufp = &buf; bufp != NULL; bufp = bufp->next) {
            memcpy(str, bufp->buf, bufp->used);
            str += bufp->used;
        }
        *str = '\0';
    }
    for (bufp = buf.next; bufp != NULL; bufp = bufp_next) {
        bufp_next = bufp->next;
        free(bufp);
    }
    return rv;
}
