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

#include "strutil.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

char *basename(const char *path)
{
    char *p = strrchr(path, '/');
    return p ? p : (char *)path;
}

size_t nrtrim(const char *s)
{
    size_t end = strlen(s);
    char *p = (char *)&s[end - 1];
    while (isspace(*p)) {
        p--;
    }

    return (size_t)(p - s) + 1;
}

char *basename_extend(const char *path, const char *ext)
{
    char *p = strrchr(path, '/');
    if (p == NULL) {
        p = (char *)path;
    } else {
        p++;
    }

    char *buf = malloc(strlen(p) + strlen(ext) + 2);
    sprintf(buf, "%s.%s", p, ext);
    return buf;
}

char *basename_stem_extend(const char *path, const char *ext)
{
    char *p = strrchr(path, '/');
    if (p == NULL) {
        p = (char *)path;
    } else {
        p++;
    }

    char *s = strrchr(p, '.');
    size_t stem_len = (s == NULL) ? strlen(p) : (size_t)(s - p); // cast to make compiler happy

    char *buf = malloc(stem_len + strlen(ext) + 2);
    strncpy(buf, p, stem_len);
    sprintf(buf + stem_len, ".%s", ext);
    return buf;
}

char *strcpy_fext(const char *path, const char *ext)
{
    char *p = strrchr(path, '.');
    if (p == NULL) {
        p = (char *)path;
    }

    int len = p - path;
    char *buf = malloc(len + strlen(ext) + 2);
    sprintf(buf, "%.*s.%s", len, path, ext);
    return buf;
}

bool match_either(const char *s, const char *a, const char *b)
{
    return (a != NULL && strcmp(s, a) == 0)
        || (b != NULL && strcmp(s, b) == 0);
}

bool isdir(const char *parent, const char *file)
{
    char target[256];
    size_t parent_len = strlen(parent);
    strcpy(target, parent);

    if (file != NULL) {
        char *p = target + parent_len;
        *p = '/';
        strcpy(p + 1, file);
    }

    struct stat stbuf;
    if (stat(target, &stbuf) != 0) {
        return false;
    }

    return S_ISDIR(stbuf.st_mode);
}

int strcmp_q(const void *s1, const void *s2)
{
    char *const *a = s1;
    char *const *b = s2;
    return strcmp(*a, *b);
}
