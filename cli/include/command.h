#ifndef NARC_COMMAND_H
#define NARC_COMMAND_H

struct command {
    const char *abbrev;
    const char *name;
    int (*main)(int argc, const char **argv);
};

int create(int argc, const char **argv);
int extract(int argc, const char **argv);
int yank(int argc, const char **argv);
int info(int argc, const char **argv);

#endif // NARC_COMMAND_H
