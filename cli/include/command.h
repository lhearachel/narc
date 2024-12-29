#ifndef NARC_COMMAND_H
#define NARC_COMMAND_H

#include <stdbool.h>
#include <string.h>

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

static inline bool match_either(const char *s, const char *a, const char *b)
{
    return (a != NULL && strcmp(s, a) == 0)
        || (b != NULL && strcmp(s, b) == 0);
}

#endif // NARC_COMMAND_H
