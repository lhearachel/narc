#ifndef NARC_UTILS_H
#define NARC_UTILS_H

#include <stdbool.h>

bool match_either(const char *s, const char *a, const char *b);

// NOTE: These routines perform a string allocation! The calling client is
// responsible for freeing the result when they are finished with it!
char *guess_extension(const char *data);
char *basename_extend(const char *path, const char *ext);

#endif // NARC_UTILS_H
