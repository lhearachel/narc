#include "extensions.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
static const struct { const char *magic; const char *ext; size_t cmp_n; } extensions[] = {
    { "RGCN", "NCGR", 4 },
    { "RLCN", "NCLR", 4 },
    { "RECN", "NCER", 4 },
    { "RNAN", "NANR", 4 },
    { 0 }, // must always be last!
};
// clang-format on

char *guess_extension(const char *data)
{
    char *buf = malloc(9);
    strcpy(buf, ".bin");

    if (data[0] == 0x10 && data[4] == 0x00) {
        // This should be an LZ77-encoded file; the first block of such an
        // encoding is always stored in full, which means we should see the
        // extension starting at index 5.
        //
        // If the file looks like it's LZ77-encoded, but we don't recognize
        // the extension, then fall back to the default (".bin").
        char *encoded_ext = guess_extension(data + 5);

        // We get back the leading dot by default.
        assert(strlen(encoded_ext) < 6);
        if (strcmp(encoded_ext, ".bin") != 0) {
            sprintf(buf, "%s.lz", encoded_ext);
        }

        free(encoded_ext);
        return buf;
    }

    for (size_t i = 0; extensions[i].magic != NULL; i++) {
        if (strncmp(data, extensions[i].magic, extensions[i].cmp_n) == 0) {
            sprintf(buf, ".%s", extensions[i].ext);
            break;
        }
    }

    return buf;
}
