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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api/error.h"
#include "api/files.h"
#include "api/load.h"

#include "utils.h"

// clang-format off
static const char *tag_line = "narc yank - yank individual files from the NARC to disk\n";

static const char *usage = "Usage: narc yank [-h | --help] FILE INDEX\n";

static const char *notes = ""
    "INDEX must be a zero-indexed value representing the position of the target to\n"
    "be yanked from FILE. INDEX is interpreted in base 10; any non-numeric input\n"
    "which does not parse to base-10 will result in a program error.\n"
    "";

static const char *options = ""
    "Options:\n"
    "  -o, --output <OUTFILE>   Write the image of the file at INDEX in the\n"
    "                           virtual filesystem to OUTFILE. By default, the\n"
    "                           image will be written to a file named with the\n"
    "                           basename of FILE (minus its extension) suffixed\n"
    "                           by INDEX and a best-guess extension for the\n"
    "                           underlying file image."
    "";
// clang-format on

static int parse_opts(int *argc, const char ***argv, char **output);
static char *output_name(const char *input_file, const char *fimg, const uint32_t file_idx);

int yank(int argc, const char **argv)
{
    struct narc *narc = NULL;
    char *output = NULL;
    FILE *fout = NULL;

    if (argc == 0 || match_either(*argv, "-h", "--help")) {
        printf("%s\n%s\n%s\n%s\n", tag_line, usage, notes, options);
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        fprintf(stderr, "narc yank: missing required positional argument INDEX\n");
        FAIL("%s\n%s\n%s\n", usage, notes, options);
    }

    parse_opts(&argc, &argv, &output);

    errno = 0;
    const char *input_file = argv[0];
    const uint32_t file_idx = strtol(argv[1], NULL, 10);
    if (errno != 0) {
        fprintf(stderr, "narc yank: invalid value given for INDEX “%s”: %s", argv[1], strerror(errno));
        fprintf(stderr, "%s\n%s\n%s\n", usage, notes, options);
        return EXIT_FAILURE;
    }

    struct vfs_ctx vfs_ctx = {0};
    enum narc_error err = narc_load(input_file, &narc, &vfs_ctx);
    if (err != NARCERR_NONE) {
        fprintf(stderr, "narc yank: error while loading FILE “%s” as NARC: %s\n", *argv, narc_strerror(err));
        return EXIT_FAILURE;
    }

    const unsigned char *fimg = NULL;
    uint32_t fsize = 0;
    err = narc_files_getimg(narc, &vfs_ctx, file_idx, &fimg, &fsize);
    if (err != NARCERR_NONE) {
        fprintf(stderr, "narc yank: could not yank image with INDEX “%d”: %s\n", file_idx, narc_strerror(err));
        return EXIT_FAILURE;
    }

    output = output ? output : output_name(input_file, (const char *)fimg, file_idx);
    fout = fopen(output, "wb");
    if (fout == NULL) {
        fprintf(stderr, "narc yank: error while opening file “%s” for writing: %s\n", output, strerror(errno));
        goto fail;
    }

    fwrite(fimg, 1, fsize, fout);
    fclose(fout);
    free(narc);
    free(output);
    return EXIT_SUCCESS;

fail:
    if (fout) {
        fclose(fout);
    }

    free(narc);
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
            // Don't trip over trying to free non-allocated memory on failures
            *output = malloc(strlen(arg) + 1);
            strcpy(*output, arg);
            (*output)[strlen(arg)] = '\0';
        }
    }

    return orig_argv - *argv;
}

static char *output_name(const char *input_file, const char *fimg, const uint32_t file_idx)
{
    const char *ext = narc_files_getext(fimg);
    char *narcbase = basename(input_file);
    char *narcext = strrchr(narcbase, '.');

    size_t narcstem_len = narcext ? (size_t)(narcext - narcbase) : strlen(narcbase);
    char *narcstem = malloc(narcstem_len + 1);
    if (narcstem == NULL) {
        fprintf(stderr, "narc yank: failed to allocate memory for output name: %s\n", strerror(errno));
        return NULL;
    }

    strncpy(narcstem, narcbase, narcstem_len);
    narcstem[narcstem_len] = '\0';

    // 5 characters for the file ID, 1 for an underscore between the stem
    // and the ID, 1 for the terminator.
    char *result = malloc(1 + 5 + 1 + narcstem_len + MAX_EXT_LEN);
    if (result == NULL) {
        fprintf(stderr, "narc yank: failed to allocate memory for output name: %s\n", strerror(errno));
        return NULL;
    }

    sprintf(result, "%s_%05d%s", narcstem, file_idx, ext);
    free(narcstem);
    return result;
}
