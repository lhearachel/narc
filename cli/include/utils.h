#ifndef NARC_UTILS_H
#define NARC_UTILS_H

#include <stdbool.h>

// NOTE: This performs a string allocation! The calling client is responsible
// for freeing the result when they are finished with it!
char *guess_extension(const char *data);

bool match_either(const char *s, const char *a, const char *b);

#endif // NARC_UTILS_H
