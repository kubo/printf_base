printf_base - base function to make printf-like functions
=========================================================

This is a base function to make printf-like functions.

Functions compatible with `printf`, `fprintf`, `dprintf`, `snprintf`, `asprintf`,
`vprintf`, `vfprintf`, `vdprintf`, `vsnprintf` and `vasprintf` are included in
the `samples` directory.

The supported format is same with [glibc printf][] except `'` and `I` flags.
This is compatible with glibc printf as possible as it can. However floating
point numbers may be slightly differently formatted because of rounding.

If `printf_base.c` is compiled with `-DPFB_NO_EXTERNAL_FUNC` compiler option,
it doesn't depend on any external function, though floating point formats
(`%e`, `%E`, `%f, `%F`, `%g`, `%G`, `%a` and `%A) and wide character formats
(`%ls`, `%S`, `%lc` and `%C`) are not supported by the option.

Usage
-----

Printf_base provides two functions: `printf_base()` and `printf_base_with_buffering()`.

You need to implement a callback function to use `printf_base()`. The
prototype is same with [fputc][] except the second argument.
It is invoked for each byte. If it cannot output a byte, it must return -1
and `printf_base()` stops. Otherwise, it must return any integer except -1.

Here is an example of printf-like function outputted to a socket by send().

```c
#ifndef WIN32
typedef int SOCKET;
#endif
static int fputc_cb(char c, void *handle)
{
    SOCKET sock = (SOCKET)handle;
    if (send(sock, &c, 1, 0) == 1) {
        return 0; /* any integer except -1 on success */
    } else {
        return -1; /* -1 on error */
    }
}

int sock_printf(SOCKET sock, const char format, ...)
{
    va_list ap;
    int rv;
    va_start(ap, format);
    rv = printf_base(fputc_cb, (void*)sock, format, ap);
    va_end(ap);
    return rv;
}
```

This example may not be efficient because it calls `send()` for each byte.
You can use `printf_base_with_buffering()` to reduce the number of `send()` calls.

You need to implement a callback function to use `printf_base_with_buffering()`.
The prototype is same with [write][] except the first argument. It is invoked
when an internal buffer (4096 bytes) is full and at last to flush remaining
bytes in the buffer. It must returns the number of bytes written. If the
returned number isn't equal to the third argument, `printf_base_with_buffering()`
stops and returns -1.

Here is another example of printf-like function outputted to a socket by send().

```c
#ifndef WIN32
typedef int SOCKET;
#endif
static int write_cb(void *handle, void *buf, size_t count)
{
    SOCKET sock = (SOCKET)handle;
    return send(sock, buf, count, 0); /* returns the number by bytes written */
}

int sock_printf(SOCKET sock, const char format, ...)
{
    va_list ap;
    int rv;
    va_start(ap, format);
    rv = printf_base_with_buffering(write_cb, (void*)sock, format, ap);
    va_end(ap);
    return rv;
}
```

License
-------

2-clause BSD-style license.

Public domain for files in the `samples` directory

[glibc printf]: http://man7.org/linux/man-pages/man3/fprintf.3.html
[fputc]: http://man7.org/linux/man-pages/man3/fputc.3.html
[write]: http://man7.org/linux/man-pages/man2/write.2.html
