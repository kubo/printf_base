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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pfb_asprintf.h"

static int check_cnt = 0;
static int error_cnt = 0;

#define CHECK(expect, ...) do { \
    char *result; \
    pfb_asprintf(&result, __VA_ARGS__); \
    if (strcmp(expect, result) != 0) { \
        printf("line:%d  expect \"%s\" but \"%s\"\n", __LINE__, expect, result); \
        error_cnt++; \
    } \
    free(result); \
    check_cnt++; \
} while (0)

int main()
{
    CHECK("123.456000",   "%f", 123.456);
    CHECK("123",          "%.0f", 123.456);
    CHECK("123.5",        "%.1f", 123.456);
    CHECK("123.46",       "%.2f", 123.456);
    CHECK("123.456",      "%.3f", 123.456);
    CHECK("123.456",      "%.*f", 3, 123.456);
    CHECK("  123.456000", "%12f", 123.456);
    CHECK("123.456000  ", "%-12f", 123.456);
    CHECK("  123.456000", "%*f", 12, 123.456);
    CHECK("123.456000  ", "%*f", -12, 123.456);
    CHECK("123.456000  ", "%-*f", 12, 123.456);
    CHECK("      123.46", "%12.2f", 123.456);
    CHECK("      123.46", "%*.*f", 12, 2, 123.456);
    CHECK(" 123.456000",  "% f", 123.456);
    CHECK("+123.456000",  "%+f", 123.456);

    CHECK("1234567890",   "%s", "1234567890");
    CHECK("1234567890",   "%5s", "1234567890");
    CHECK("1234567890",   "%-5s", "1234567890");
    CHECK("  1234567890", "%12s", "1234567890");
    CHECK("1234567890  ", "%-12s", "1234567890");
    CHECK("12345",        "%.5s", "1234567890");
    CHECK("1234567890",   "%.12s", "1234567890");
    CHECK("1234567890",   "%*s", 5, "1234567890");
    CHECK("1234567890",   "%*s", -5, "1234567890");
    CHECK("1234567890",   "%-*s", 5, "1234567890");
    CHECK("  1234567890", "%*s", 12, "1234567890");
    CHECK("1234567890  ", "%*s", -12, "1234567890");
    CHECK("1234567890  ", "%-*s", 12, "1234567890");
    CHECK("12345",        "%.*s", 5, "1234567890");
    CHECK("1234567890",   "%.*s", 12, "1234567890");

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
