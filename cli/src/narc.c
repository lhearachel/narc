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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <narc.h>

#include "command.h"

static bool match_opt(const char *opt, const char *shortopt, const char *longopt);

// clang-format off
static const struct command handlers[] = {
    { "c", "create",  create  },
    { "x", "extract", extract },
    { "y", "yank",    yank    },
    { "i", "info",    info    },
};

static const char *version = "0.1.0";

static const char *tag_line = "narc - create, extract, and explore Nitro Archive virtual filesystems\n";

static const char *short_usage = "Usage: narc [-h | --help] [-v | --version] [command] [arguments]\n";

static const char *commands = ""
    "Commands:\n"
    "  c, create    Create a NARC from a folder of physical files\n"
    "  x, extract   Extract virtual files from the NARC to a folder\n"
    "  y, yank      Yank individiaul files from the NARC to disk\n"
    "  i, info      Print diagnostics about a NARC"
    "";
// clang-format on

int main(int argc, const char **argv)
{
    argc--;
    argv++;

    if (argc == 0 || match_opt(*argv, "-h", "--help")) {
        fprintf(stdout, "%s\n%s\n%s\n", tag_line, short_usage, commands);
        return EXIT_SUCCESS;
    }

    if (match_opt(*argv, "-v", "--version")) {
        fprintf(stdout, "%s\n", version);
        return EXIT_SUCCESS;
    }

    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        if (match_opt(*argv, handlers[i].abbrev, handlers[i].name)) {
            argc--;
            argv++;
            return handlers[i].main(argc, argv);
        }
    }

    fprintf(stderr, "narc: Unrecognized command “%s”\n", *argv);
    fprintf(stderr, "%s\n%s\n", short_usage, commands);
    return EXIT_FAILURE;
}

static bool match_opt(const char *opt, const char *shortopt, const char *longopt)
{
    return (shortopt != NULL && strcmp(opt, shortopt) == 0)
        || (longopt != NULL && strcmp(opt, longopt) == 0);
}
