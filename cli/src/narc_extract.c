#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api/dump.h"
#include "api/error.h"
#include "api/load.h"

#include "defs/error.h"
#include "defs/vfs.h"
#include "utils.h"

// clang-format off
static const char *tag_line = "narc extract - extract virtual files from the NARC to a folder\n";

static const char *short_usage = "Usage: narc extract [-h | --help] [options] FILE\n";

static const char *options = ""
    "Options:\n"
    "  -o, --output <DIRECTORY>   Persist extracted file images beneath DIRECTORY.\n"
    "                             By default, the target directory will be set to\n"
    "                             the basename of FILE, suffixed with “.d”, and\n"
    "                             located in the current working directory."
    "";
// clang-format on

#define FAIL(msgfmt, ...)                     \
    {                                         \
        fprintf(stderr, msgfmt, __VA_ARGS__); \
        goto fail;                            \
    }

static int parse_opts(int *argc, const char ***argv, char **output);

int extract(int argc, const char **argv)
{
    if (argc == 0 || match_either(*argv, "-h", "--help")) {
        printf("%s\n%s\n%s\n", tag_line, short_usage, options);
        return EXIT_SUCCESS;
    }

    char *output = NULL;
    parse_opts(&argc, &argv, &output);

    if (argc < 1) {
        fprintf(stderr, "narc extract: missing required positional argument FILE\n");
        fprintf(stderr, "%s\n%s\n", short_usage, options);
        return EXIT_FAILURE;
    }

    const char *input = *argv;
    if (output == NULL) {
        output = basename_extend(input, "d");
    }

    struct narc *narc = NULL;
    struct vfs_ctx vfs_ctx = {0};
    enum narc_error err = narc_load(input, &narc, &vfs_ctx);
    if (err != NARCERR_NONE) {
        FAIL("narc extract: error while loading FILE “%s”: %s", input, narc_strerror(err));
    }

    err = narc_dump(narc, &vfs_ctx, output);
    if (err != NARCERR_NONE) {
        FAIL("narc extract: error while dumping NARC to DIRECTORY “%s”: %s", output, narc_strerror(err));
    }

    return EXIT_SUCCESS;

fail:
    free(output);
    return EXIT_FAILURE;
}

static int parse_opts(int *argc, const char ***argv, char **output)
{
    const char **orig_argv = *argv;

    while (*argc > 1) {
        const char *opt = **argv;
        if (opt[0] != '-' || (opt[1] == '-' && opt[2] == '\0')) {
            break;
        }

        (*argc)--;
        (*argv)++;

        const char *arg = **argv;
        (*argc)--;
        (*argv)++;

        if (match_either(opt, "-o", "--output")) {
            *output = malloc(strlen(arg) + 1);
            strcpy(*output, arg);
            (*output)[strlen(arg)] = '\0';
        }
    }

    return orig_argv - *argv;
}
