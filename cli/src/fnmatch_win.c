/*
 * Windows-compatible fnmatch implementation using PathMatchSpecA
 */

#include "fnmatch_win.h"
#include <shlwapi.h>

int fnmatch(const char *pattern, const char *string, int flags)
{
    // PathMatchSpecA returns TRUE on match, fnmatch returns 0 on match
    return PathMatchSpecA(string, pattern) ? 0 : FNM_NOMATCH;
}
