/*
 * Copyright 2024 <lhearachel@proton.me>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "strbuild.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct strbuild *strbuild_new(size_t capacity)
{
    struct strbuild *s = malloc(sizeof(struct strbuild));

    s->s = malloc(capacity);
    s->s[0] = '\0';
    s->len = 0;
    s->capacity = capacity;

    return s;
}

void strbuild_del(struct strbuild *strbuild)
{
    free(strbuild->s);
    free(strbuild);
}

int strbuild_cat(struct strbuild *strbuild, const char *s)
{
    size_t next_len = strlen(s);
    if (strbuild->len + next_len > strbuild->capacity) {
        size_t next_cap = (strbuild->capacity + next_len) * 2;
        char *p = realloc(strbuild->s, next_cap);
        if (p == NULL) {
            return -1;
        }

        strbuild->s = p;
        strbuild->capacity = next_cap;
    }

    strcat(strbuild->s, s);
    strbuild->len += next_len;
    return 0;
}

int strbuild_sprintf(struct strbuild *strbuild, const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    return strbuild_cat(strbuild, buf);
}
