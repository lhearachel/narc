#ifndef NARC_COMMAND_H
#define NARC_COMMAND_H

#include <stdbool.h>

struct command {
    const char *abbrev;
    const char *name;
    int (*main)(int argc, const char **argv);
};

int create(int argc, const char **argv);
int extract(int argc, const char **argv);
int yank(int argc, const char **argv);
int info(int argc, const char **argv);
int help(int argc, const char **argv);

bool match_either(const char *s, const char *a, const char *b);

extern const struct command handlers[];

#endif // NARC_COMMAND_H
