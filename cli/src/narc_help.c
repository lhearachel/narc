#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "utils.h"

// clang-format off
static const char *tag_line = "narc - create, extract, and explore Nitro Archive virtual filesystems\n";

static const char *short_usage = "Usage: narc [-h | --help] [-v | --version] [command] [arguments]\n";

static const char *commands = ""
    "Commands:\n"
    "  c, create    Create a NARC from a folder of physical files\n"
    "  x, extract   Extract virtual files from the NARC to a folder\n"
    "  y, yank      Yank individiaul files from the NARC to disk\n"
    "  i, info      Print metadata for a NARC\n"
    "  h, help      Print help-text for a particular command"
    "";
// clang-format on

int help(int argc, const char **argv)
{
    if (argc < 0) {
        goto error;
    }

    if (argc == 0) {
        printf("%s\n%s\n%s\n", tag_line, short_usage, commands);
        return EXIT_SUCCESS;
    }

    for (size_t i = 0; handlers[i].abbrev != NULL; i++) {
        if (match_either(*argv, handlers[i].abbrev, handlers[i].name)) {
            return handlers[i].main(0, argv);
        }
    }

error:
    fprintf(stderr, "narc: Unrecognized command “%s”\n", *argv);
    fprintf(stderr, "%s\n%s\n", short_usage, commands);
    return EXIT_FAILURE;
}
