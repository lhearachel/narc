#ifndef NARC_COMMAND_H
#define NARC_COMMAND_H

#include <stdbool.h>

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
