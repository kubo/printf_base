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
#include <string.h>
#include <assert.h>
#include "pfb_snprintf.h"

int main()
{
    char buf[30];

    memset(buf, 0x01, sizeof(buf));
    assert(pfb_snprintf(buf, 20, "%d", 12345) == 5);
    assert(strcmp(buf, "12345") == 0);
    assert(pfb_snprintf(buf, 20, "%s", "1234567890") == 10);
    assert(strcmp(buf, "1234567890") == 0);
    assert(pfb_snprintf(buf, 20, "%s", "1234567890123456789") == 19);
    assert(memcmp(buf, "1234567890123456789\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01", 30) == 0);
    assert(pfb_snprintf(buf, 20, "%s", "12345678901234567890") == 20);
    assert(memcmp(buf, "12345678901234567890\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01", 30) == 0);
    assert(pfb_snprintf(buf, 20, "%s", "123456789012345678901") == 21);
    assert(memcmp(buf, "12345678901234567890\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01", 30) == 0);
    return 0;
}
