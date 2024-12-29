#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "api/error.h"
#include "api/files.h"
#include "api/load.h"
#include "defs/fatb.h"
#include "defs/fimg.h"
#include "defs/fntb.h"
#include "defs/vfs.h"

#include "command.h"
#include "utils.h"

// clang-format off
static const char *tag_line = "narc info - print metadata for a NARC\n";

static const char *usage = "Usage: narc info [-h | --help] FILE\n";

static const char *notes = ""
    "The following global metadata is extracted and output to the console:\n"
    "  1. The total size of the NARC (in bytes)\n"
    "  2. The byte-offsets of each virtual filesystem sections\n"
    "  3. The count of member files in the NARC\n"
    "  4. The count of virtual directories in the NARC; NOTE: this will always\n"
    "     be at LEAST 1 for the virtual root\n"
    "\n"
    "Additionally, for each member file, the following metadata will be output:\n"
    "  1. The byte-offset of the start of the file image (its byte data)\n"
    "  2. The size of the file image\n"
    "  3. A recognized file-type (or “.bin”, if the file-type is not recognized)\n"
    "\n"
    "Due to the potential size of the output above, a user may consider piping\n"
    "this output to a file or a preferred pager program (e.g., “less”)."
    "";
// clang-format on

int info(int argc, const char **argv)
{
    if (argc == 0 || match_either(*argv, "-h", "--help")) {
        printf("%s\n%s\n%s\n", tag_line, usage, notes);
        return EXIT_SUCCESS;
    }

    struct narc *narc = NULL;
    struct vfs_ctx vfs_ctx = {0};
    enum narc_error err = narc_load(*argv, &narc, &vfs_ctx);
    if (err != NARCERR_NONE) {
        fprintf(stderr, "narc info: could not load FILE “%s”: %s\n", *argv, narc_strerror(err));
        return EXIT_FAILURE;
    }

    struct fatb_meta *fatb_meta = (struct fatb_meta *)(narc->vfs + vfs_ctx.fatb_ofs);
    struct fatb_entry *fatb = (struct fatb_entry *)(narc->vfs + vfs_ctx.fatb_ofs + sizeof(*fatb_meta));
    struct fntb_meta *fntb_meta = (struct fntb_meta *)(narc->vfs + vfs_ctx.fntb_ofs);
    struct fntb_main_entry *fntb = (struct fntb_main_entry *)(narc->vfs + vfs_ctx.fntb_ofs + sizeof(*fntb_meta));
    char *fimg = (char *)(narc->vfs + vfs_ctx.fimg_ofs + sizeof(struct fimg_meta));

    printf("Total size (bytes): %d\n", narc->size);
    printf("Section offsets:\n");
    printf("  - FATB: 0x%08X\n", vfs_ctx.fatb_ofs);
    printf("  - FNTB: 0x%08X\n", vfs_ctx.fntb_ofs);
    printf("  - FIMG: 0x%08X\n", vfs_ctx.fimg_ofs);
    printf("File count: %d\n", fatb_meta->num_files);
    printf("Directory count: %d\n", fntb->directory);
    printf("Member files:\n");

    for (size_t i = 0; i < fatb_meta->num_files; i++, fatb++) {
        char *ext = narc_files_getext(fimg + fatb->start);
        printf("  - %05ld -> { start = 0x%08X, size = 0x%08X, filetype = %-8s }\n", i, fatb->start, fatb->end - fatb->start, ext);
        free(ext);
    }

    return EXIT_SUCCESS;
}
