#include "utils.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *basename_extend(const char *path, const char *ext)
{
    char *p = strrchr(path, '/');
    if (p == NULL) {
        p = (char *)path;
    }

    char *buf = malloc(strlen(p) + strlen(ext) + 2);
    sprintf(buf, "%s.%s", p, ext);
    return buf;
}

bool match_either(const char *s, const char *a, const char *b)
{
    return (a != NULL && strcmp(s, a) == 0)
        || (b != NULL && strcmp(s, b) == 0);
}
