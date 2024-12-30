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

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "api/pack.h"
#include "defs/narc.h"
#include "defs/vfs.h"

#include "utils.h"

// clang-format off
static const char *tag_line = "narc create - create a NARC from a folder of physical files\n";

static const char *short_usage = "Usage: narc create [-h | --help] [options] DIRECTORY\n";

static const char *notes = ""
    "Files within DIRECTORY will be added to the NARC's virutal filesystem (VFS)\n"
    "in lexicographical order.\n"
    "";

static const char *options = ""
    "Options:\n"
    "  -o, --output <FILE>  Persist the output to FILE. By default, output is\n"
    "                       written to “DIRECTORY.narc”.\n"
    "  --naix               Create a Nitro Archive Index (NAIX) header. The output\n"
    "                       NAIX will have the same file-stem as the output NARC.\n"
    "  --order <FILE>       Read lines from FILE as filepaths in DIRECTORY which\n"
    "                       should be added to the NARC's VFS in non-lexicographical\n"
    "                       order. These files will be added to the NARC before any\n"
    "                       other members within DIRECTORY.\n"
    "  --ignore <FILE>      Read lines from FILE as a list of filepaths and glob-\n"
    "                       patterns which may be present in DIRECTORY but should\n"
    "                       not be added to the output."
    "";
// clang-format on

struct options {
    bool naix;
    const char *input;
    const char *output;
    const char *order;
    const char *ignore;
};

static int parse_opts(int *argc, const char ***argv, struct options *opts);

int create(int argc, const char **argv)
{
    DIR *dir = NULL;
    FILE *fout = NULL;
    char *cwd = NULL;
    struct narc *narc = NULL;
    struct vfs_pack_ctx *ctx = NULL;

    if (argc == 0 || match_either(*argv, "-h", "--help")) {
        printf("%s\n%s\n%s\n%s\n", tag_line, short_usage, notes, options);
        return EXIT_SUCCESS;
    }

    struct options opts = {
        .input = NULL,
        .output = NULL,
        .naix = false,  // TODO: Construct NAIX header during packing
        .order = NULL,  // TODO: Read entries from order file
        .ignore = NULL, // TODO: Read entries from ignore file
    };
    parse_opts(&argc, &argv, &opts);

    if (argc < 1) {
        fprintf(stderr, "narc create: missing required positional argument DIRECTORY\n");
        fprintf(stderr, "%s\n%s\n", short_usage, options);
        return EXIT_FAILURE;
    }

    opts.input = *argv;
    if (opts.output == NULL) {
        opts.output = basename_extend(opts.input, "narc");
    }

    dir = opendir(opts.input);
    if (dir == NULL) {
        FAIL("narc create: could not open DIRECTORY “%s”: %s\n", opts.input, strerror(errno));
    }

    cwd = getcwd(cwd, 0);
    if (cwd == NULL) {
        FAIL("narc create: could not get current working directory: %s\n", strerror(errno));
    }

    chdir(opts.input);
    errno = 0;
    struct dirent *entry;
    ctx = narc_pack_start();
    while (dir && (entry = readdir(dir)) != NULL) {
        if (match_either(entry->d_name, ".", "..")) {
            continue;
        }

        FILE *f = fopen(entry->d_name, "rb");
        if (f == NULL) {
            FAIL("narc create: error while opening file “%s” for reading: %s\n", entry->d_name, strerror(errno));
        }

        fseek(f, 0, SEEK_END);
        size_t fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        unsigned char *image = malloc(fsize);
        if (image == NULL) {
            FAIL("narc create: error while reading file “%s”: %s\n", entry->d_name, strerror(errno));
        }

        fread(image, 1, fsize, f);
        narc_pack_file(ctx, image, fsize);
    }

    narc = narc_pack(ctx);

    chdir(cwd);
    fout = fopen(opts.output, "wb");
    if (fout == NULL) {
        FAIL("narc create: error while opening file “%s” for writing: %s\n", opts.output, strerror(errno));
    }

    fwrite(narc, narc->size, 1, fout);

    free(cwd);
    free(narc);
    fclose(fout);
    closedir(dir);
    return EXIT_SUCCESS;

fail:
    if (fout) {
        fclose(fout);
    }

    if (dir) {
        closedir(dir);
    }

    free(cwd);
    free(narc);
    return EXIT_FAILURE;
}

static int parse_opts(int *argc, const char ***argv, struct options *opts)
{
    const char **orig_argv = *argv;

    while (*argc > 1) {
        const char *opt = **argv;
        if (opt[0] != '-' || (opt[1] == '-' && opt[2] == '\0')) {
            break;
        }

        (*argc)--;
        (*argv)++;

        if (match_either(opt, NULL, "--naix")) {
            opts->naix = true;
            continue;
        }

        const char *arg = **argv;
        (*argc)--;
        (*argv)++;

        if (match_either(opt, "-o", "--output")) {
            opts->output = arg;
        } else if (match_either(opt, NULL, "--order")) {
            opts->order = arg;
        } else if (match_either(opt, NULL, "--ignore")) {
            opts->ignore = arg;
        }
    }

    return orig_argv - *argv;
}
