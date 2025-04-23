#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>

typedef enum {
    DIST_UNIF,
    DIST_UINT,
    DIST_NORMAL,
} Distrib;

typedef enum {
    ARG_DISTRIB,
    ARG_SEED,
    ARG_COUNT,
    ARG_DIR,
    ARG_HELP,
    ARG_UNKNOWN,
} ArgType;

typedef struct {
    Distrib dist;
    uint64_t seed;
    int provided_seed;
    uint64_t n;
    char *output_path;
    int provided_out;
} ParsedArgs;


ArgType parse_arg_type(const char *arg);
void parse_args(int argc, char **argv, ParsedArgs *args);
void print_help(const char *prog_name);

#endif
