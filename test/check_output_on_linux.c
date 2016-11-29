/* -*- indent-tabs-mode: nil -*-
 *
 * printf_base - base function to make printf-like functions
 * https://github.com/kubo/printf_base
 *
 * Copyright (C) 2016 Kubo Takehiro <kubo@jiubao.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of the authors.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <gnu/libc-version.h>
#define PFB_PRINTF_NO_COMPAT
#include "pfb_asprintf.h"

static char flag_characters[] = "#0- +";
static int field_widths[] = {0, 5, 10, 40};
static int precisions[] = {0, -1, 5, 9, 40};
static const char *length_modifilers[] = {
    "",
    "hh",
    "h",
    "l",
    "ll",
    "L",
    "j",
    "z",
    "t",
};

#define NUM_OF_FIELD_WIDTHS (sizeof(field_widths)/sizeof(field_widths[0]))
#define NUM_OF_PRECISIONS (sizeof(precisions)/sizeof(precisions[0]))
#define NUM_OF_LENGTH_MODIFILERS (sizeof(length_modifilers)/sizeof(length_modifilers[0]))

static int check_cnt = 0;
static int error_cnt = 0;

static void set_field_width(char *format);
static void set_precision(char *format);
static void set_length_modifier(char *format);
static void set_conversion_specifier(char *format);
static void check_int_argument(char *format);
static void check_dbl_argument(char *format);
static void check_chr_argument(char *format);
static void check_str_argument(char *format);
static void check_no_argument(char *format);

static void check(const char *format, ...);

int main()
{
    char format[40];
    int i, j;

    setlocale(LC_ALL, "");
    printf("Comparing printf_maker and printf in glibc %s...\n", gnu_get_libc_version());

    strcpy(format, "%");
    set_field_width(format);
    for (i = 0; flag_characters[i] != 0; i++) {
        format[1] = flag_characters[i];
        format[2] = '\0';
        set_field_width(format);
    }
    for (i = 0; flag_characters[i] != 0; i++) {
        format[1] = flag_characters[i];
        for (j = 0; flag_characters[j] != 0; j++) {
            format[2] = flag_characters[j];
            format[3] = '\0';
            set_field_width(format);
        }
    }

    check("%c%c%c%c%c", 'h', 'e', 'l', 'l', 'o');
    check("%*s, 30", 30, "Hello World");
    check("%*s, -30", -30, "Hello World");
    check("%*.*s, 30, 0", 30, 0, "Hello World");
    check("%*.*s, -30, 0", -30, 0, "Hello World");
    check("%*.*s, 30, 5", 30, 5, "Hello World");
    check("%*.*s, -30, 5", -30, 5, "Hello World");
    check("%*.*s, 5, 10", 5, 10, "Hello World");
    check("%*.*s, -15, 10", -5, 10, "Hello World");
    check("%3$*1$.*2$s, -30, 10", -30, 10, "Hello World");
    check("%1$*2$.*3$s, -30, 10", "Hello World", -30, 10);
    check("%1$d, %3$d", 1, 2, 3);
    check("%*.*%, %d", 1, 2, 3);

    switch (error_cnt) {
    case 0:
        printf("All %d tests were passed!\n", check_cnt);
        return 0;
    case 1:
        printf("One test in %d was failed\n", check_cnt);
        return 1;
    default:
        printf("%d tests in %d were failed\n", error_cnt, check_cnt);
        return 1;
    }
}

static void set_field_width(char *format)
{
    size_t offset = strlen(format);
    int i;

    for (i = 0; i < NUM_OF_FIELD_WIDTHS; i++) {
        if (field_widths[i] == 0) {
            format[offset] = '\0';
        } else {
            sprintf(format + offset, "%d", field_widths[i]);
        }
        set_precision(format);
    }
    format[offset] = '\0';
}

static void set_precision(char *format)
{
    size_t offset = strlen(format);
    int i;

    for (i = 0; i < NUM_OF_PRECISIONS; i++) {
        if (precisions[i] < 0) {
            strcpy(format + offset, ".");
        } else {
            sprintf(format + offset, ".%d", precisions[i]);
        }
        set_length_modifier(format);
    }
    format[offset] = '\0';
}

static void set_length_modifier(char *format)
{
    size_t offset = strlen(format);
    int i;

    for (i = 0; i < NUM_OF_LENGTH_MODIFILERS; i++) {
        strcpy(format + offset, length_modifilers[i]);
        set_conversion_specifier(format);
    }
    format[offset] = '\0';
}

static void set_conversion_specifier(char *format)
{
    size_t offset = strlen(format);

    strcpy(format + offset, "d");
    check_int_argument(format);

    strcpy(format + offset, "i");
    check_int_argument(format);

    strcpy(format + offset, "o");
    check_int_argument(format);

    strcpy(format + offset, "u");
    check_int_argument(format);

    strcpy(format + offset, "x");
    check_int_argument(format);

    strcpy(format + offset, "X");
    check_int_argument(format);

    strcpy(format + offset, "e");
    check_dbl_argument(format);

    strcpy(format + offset, "E");
    check_dbl_argument(format);

    strcpy(format + offset, "f");
    check_dbl_argument(format);

    strcpy(format + offset, "F");
    check_dbl_argument(format);

    strcpy(format + offset, "g");
    check_dbl_argument(format);

    strcpy(format + offset, "G");
    check_dbl_argument(format);

    strcpy(format + offset, "a");
    check_dbl_argument(format);

    strcpy(format + offset, "A");
    check_dbl_argument(format);

    strcpy(format + offset, "c");
    check_chr_argument(format);

    strcpy(format + offset, "s");
    check_str_argument(format);

    strcpy(format + offset, "C");
    check_chr_argument(format);

    strcpy(format + offset, "S");
    check_str_argument(format);

    strcpy(format + offset, "p");
    check_int_argument(format);

    strcpy(format + offset, "%");
    check_no_argument(format);

    format[offset] = '\0';
}

static void check_int_argument(char *format)
{
    check(format, (int32_t)-1234567);
    check(format, (int32_t)1234567);
    check(format, INT32_MAX);
    check(format, INT32_MIN);
    check(format, UINT32_MAX);
    check(format, INT32_MAX);
    check(format, INT32_MIN);
    check(format, UINT32_MAX);
    check(format, (uint64_t)0);
}

static void check_dbl_argument(char *format)
{
    size_t len = strlen(format);
    int use_longdouble = 0;
    static const char *longdouble_suffix[] = {
        "L",
        "ll",
        NULL,
    };
    int i;

    for (i = 0; longdouble_suffix[i] != NULL; i++) {
        size_t slen = strlen(longdouble_suffix[i]);
        if (len > slen + 1 && strncmp(&format[len - slen - 1], longdouble_suffix[i], slen) == 0) {
            use_longdouble = 1;
            break;
        }
    }

    if (use_longdouble) {
        check(format, (long double)0.0);
        check(format, (long double)1.0/8.0); /* 0.125 */
        check(format, (long double)1.0/2048.0); /* 0.00048828125 */
        check(format, (long double)100000000.0/8.0); /* 12500000.0 */
        check(format, (long double)100000000.0/2048.0); /* 48828.125 */
        check(format, (long double)-0.0);
        check(format, (long double)-1.0/8.0); /* -0.125 */
        check(format, (long double)-1.0/2048.0); /* -0.00048828125 */
        check(format, (long double)-100000000.0/8.0); /* -12500000.0 */
        check(format, (long double)-100000000.0/2048.0); /* -48828.125 */
        check(format, (long double)HUGE_VAL); /* +infinity */
        check(format, (long double)-HUGE_VAL); /* -infinity */
        check(format, (long double)NAN); /* NaN */
        check(format, (long double)-NAN); /* -NaN */
    } else {
        check(format, 0.0);
        check(format, 1.0/8.0); /* 0.125 */
        check(format, 1.0/2048.0); /* 0.00048828125 */
        check(format, 100000000.0/8.0); /* 12500000.0 */
        check(format, 100000000.0/2048.0); /* 48828.125 */
        check(format, -0.0);
        check(format, -1.0/8.0); /* -0.125 */
        check(format, -1.0/2048.0); /* -0.00048828125 */
        check(format, -100000000.0/8.0); /* -12500000.0 */
        check(format, -100000000.0/2048.0); /* -48828.125 */
        check(format, HUGE_VAL); /* +infinity */
        check(format, -HUGE_VAL); /* -infinity */
        check(format, NAN); /* NaN */
        check(format, -NAN); /* -NaN */
    }
}

static void check_chr_argument(char *format)
{
    check(format, '0');
    check(format, 'A');
    check(format, 'a');
}

static void check_str_argument(char *format)
{
    size_t len = strlen(format);
    int use_wchar = 0;
    static const char *wchar_suffix[] = {
        "S",
        "ls",
        "ts",
        "zs",
        "js",
        "Ls",
        NULL,
    };
    int i;

    for (i = 0; wchar_suffix[i] != NULL; i++) {
        size_t slen = strlen(wchar_suffix[i]);
        if (len > slen && strcmp(&format[len - slen], wchar_suffix[i]) == 0) {
            use_wchar = 1;
            break;
        }
    }

    check(format, NULL);
    if (use_wchar) {
        check(format, L"foo");
        check(format, L"Hello World");
    } else {
        check(format, "foo");
        check(format, "Hello World");
    }
}

static void check_no_argument(char *format)
{
    check(format);
}

static void check(const char *format, ...)
{
    int libc_rv, pf_rv;
    char *libc_str = "", *pf_str = "";
    va_list ap;

    check_cnt++;
    va_start(ap, format);
    libc_rv = vasprintf(&libc_str, format, ap);
    va_end(ap);
    va_start(ap, format);
    pf_rv = pfb_vasprintf(&pf_str, format, ap);
    va_end(ap);
    if (strcmp(libc_str, pf_str) != 0) {
        error_cnt++;
        fprintf(stderr, "ERROR: '%s'\n", format);
        fprintf(stderr, " libc: '%s'\n pf  : '%s'\n", libc_str, pf_str);
        fprintf(stderr, " libc_rv = %d, pf_rv = %d\n\n", libc_rv, pf_rv);
    } else if (libc_rv != pf_rv) {
        error_cnt++;
        fprintf(stderr, "ERROR: '%s'\n", format);
        fprintf(stderr, " out: '%s'\n libc_rv = %d, pf_rv = %d\n\n", libc_str, libc_rv, pf_rv);
    }
    if (libc_rv != -1) {
        free(libc_str);
    }
    if (pf_rv != -1) {
        free(pf_str);
    }
}
