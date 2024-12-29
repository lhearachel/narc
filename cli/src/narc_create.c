#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "api/pack.h"
#include "defs/narc.h"
#include "defs/vfs.h"

#include "command.h"

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
    "                       written to <DIRECTORY>.narc.\n"
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
static char *narcname(const char *path);

int create(int argc, const char **argv)
{
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
        opts.output = narcname(opts.input);
    }

    DIR *dir = opendir(opts.input);
    if (dir == NULL) {
        fprintf(stderr, "narc create: could not open DIRECTORY “%s”: %s\n", opts.input, strerror(errno));
        return EXIT_FAILURE;
    }

    char *cwd = NULL;
    cwd = getcwd(cwd, 0);
    if (cwd == NULL) {
        fprintf(stderr, "narc create: could not get current working directory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    chdir(opts.input);
    errno = 0;
    struct dirent *entry;
    struct vfs_pack_ctx *ctx = narc_pack_start();
    while (dir && (entry = readdir(dir)) != NULL) {
        if (match_either(entry->d_name, ".", "..")) {
            continue;
        }

        FILE *f = fopen(entry->d_name, "rb");
        if (f == NULL) {
            fprintf(stderr, "narc create: error while opening file “%s”: %s\n", entry->d_name, strerror(errno));
            closedir(dir);
            narc_pack_halt(ctx);
            return EXIT_FAILURE;
        }

        fseek(f, 0, SEEK_END);
        size_t fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        unsigned char *image = malloc(fsize);
        if (image == NULL) {
            fprintf(stderr, "narc create: error while reading file “%s”: %s\n", entry->d_name, strerror(errno));
            fclose(f);
            closedir(dir);
            narc_pack_halt(ctx);
            return EXIT_FAILURE;
        }

        fread(image, 1, fsize, f);
        narc_pack_file(ctx, image, fsize);
    }

    struct narc *narc = narc_pack(ctx);

    chdir(cwd);
    FILE *fout = fopen(opts.output, "wb");
    if (fout == NULL) {
        fprintf(stderr, "narc create: error while reading DIRECTORY “%s”: %s\n", opts.input, strerror(errno));
        return EXIT_FAILURE;
    }

    fwrite(narc, narc->size, 1, fout);

    free(cwd);
    free(narc);
    fclose(fout);
    closedir(dir);
    return EXIT_SUCCESS;
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
            if (access(arg, F_OK) != 0) {
                fprintf(stderr, "narc create: file arg “%s” to option “--order” does not exist\n", arg);
                return EXIT_FAILURE;
            }
        } else if (match_either(opt, NULL, "--ignore")) {
            opts->ignore = arg;
            if (access(arg, F_OK) != 0) {
                fprintf(stderr, "narc create: file arg “%s” to option “--ignore” does not exist\n", arg);
                return EXIT_FAILURE;
            }
        }
    }

    return orig_argv - *argv;
}

static char *narcname(const char *path)
{
    char *p = strrchr(path, '/');
    if (p == NULL) {
        p = (char *)path;
    }

    char *buf = malloc(strlen(p) + 6);
    sprintf(buf, "%s.narc", p);
    return buf;
}
