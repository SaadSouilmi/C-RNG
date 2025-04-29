#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>

typedef enum {
    DIST_UNIF,
    DIST_UINT,
    DIST_NORMAL,
    DIST_EXP,
} Distrib;

typedef enum {
    ARG_DISTRIB,
    ARG_SEED,
    ARG_COUNT,
    ARG_DIR,
    ARG_HELP,
    ARG_MEAN,
    ARG_STDDEV,
    ARG_LAMBDA,
    ARG_A,
    ARG_B, 
    ARG_NTHREADS,
    ARG_UNKNOWN,
} ArgType;

typedef struct {
    Distrib dist;
    uint64_t seed;
    int provided_seed;
    uint64_t n;
    char *output_path;
    int provided_out;
    // Distribution parameters 
    union {
        struct {
            double mean;
            double stddev;
        } normal;

        struct {
            double lambda;
        } exp;

        struct {
            double a;
            double b;
        } unif;
        
        struct{
            uint64_t a;
        } uint;
    } params;
    uint8_t n_threads;
} ParsedArgs;


ArgType parse_arg_type(const char *arg);
void parse_args(int argc, char **argv, ParsedArgs *args);
void print_help(const char *prog_name);

#endif
