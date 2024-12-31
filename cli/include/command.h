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
#ifndef NARC_COMMAND_H
#define NARC_COMMAND_H

#include <stdbool.h>

// Standard failure macro. `fail` should be implemented for all commands
// as an end-state where any potentially-allocated memory is freed and the
// routine returns EXIT_FAILURE.
#define FAIL(msgfmt, ...)                     \
    {                                         \
        fprintf(stderr, msgfmt, __VA_ARGS__); \
        goto fail;                            \
    }

struct command {
    const char *abbrev;
    const char *name;
    int (*main)(int argc, const char **argv);
};

extern const struct command handlers[];

int create(int argc, const char **argv);
int extract(int argc, const char **argv);
int yank(int argc, const char **argv);
int info(int argc, const char **argv);
int help(int argc, const char **argv);

#endif // NARC_COMMAND_H
