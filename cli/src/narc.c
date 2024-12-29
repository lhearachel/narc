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
#include "utils.h"

// clang-format off
const struct command handlers[] = {
    { "c", "create",  create  },
    { "x", "extract", extract },
    { "y", "yank",    yank    },
    { "i", "info",    info    },
    { "h", "help",    help    },
    { 0 },
};

static const char *version = "0.1.0";
// clang-format on

int main(int argc, const char **argv)
{
    argc--;
    argv++;

    if (argc == 0 || match_either(*argv, "-h", "--help")) {
        return help(0, argv);
    }

    if (match_either(*argv, "-v", "--version")) {
        fprintf(stdout, "%s\n", version);
        return EXIT_SUCCESS;
    }

    for (size_t i = 0; handlers[i].abbrev[0] != '\0'; i++) {
        if (match_either(*argv, handlers[i].abbrev, handlers[i].name)) {
            argc--;
            argv++;
            return handlers[i].main(argc, argv);
        }
    }

    // Special case in help which jumps straight to printing error text
    return help(-1, argv);
}
