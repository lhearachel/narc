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

#include "strvec.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef intptr_t ssize_t;

struct strvec *strvec_new(size_t capacity)
{
    struct strvec *vec = malloc(sizeof(struct strvec));

    vec->s = malloc(capacity * sizeof(char *));
    vec->count = 0;
    vec->capacity = capacity;

    return vec;
}

void strvec_del(struct strvec *strvec)
{
    for (size_t i = 0; i < strvec->count; i++) {
        free(strvec->s[i]);
    }

    free(strvec->s);
    free(strvec);
}

int strvec_push(struct strvec *strvec, char *s)
{
    if (strvec->count + 1 > strvec->capacity) {
        char **p = realloc(strvec->s, strvec->capacity * sizeof(char *));
        if (p == NULL) {
            return -1;
        }

        strvec->s = p;
        strvec->capacity *= 2;
    }

    strvec->s[strvec->count] = s;
    strvec->count++;
    return 0;
}

static ssize_t read_line(char **lineptr, size_t *n, FILE *stream)
{
    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    int c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }

        *n = 128;
    }

    size_t pos = 0;
    do {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }

            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }

            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos++] = c;
        if (c == '\n') {
            break;
        }
    } while ((c = fgetc(stream)) != EOF);

    (*lineptr)[pos] = '\0';
    return pos;
}

int strvec_from_file(struct strvec *strvec, const char *fname)
{
    if (fname == NULL) {
        return 0;
    }

    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        return -1;
    }

    char *buf = NULL;
    size_t buf_size = 0;
    ssize_t read_size;

    while ((read_size = read_line(&buf, &buf_size, f)) != -1) {
        // Skip over empty lines
        if (buf[0] == '\n' || (buf[0] == '\r' && buf[1] == '\n') || buf[0] == '\0') {
            continue;
        }

        char *line = malloc(read_size);
        strncpy(line, buf, read_size - 1);
        line[read_size] = '\0';
        if (strvec_push(strvec, line)) {
            return -1;
        }
    }

    free(buf);
    return 0;
}