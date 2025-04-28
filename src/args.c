#include "args.h"
#include "sample.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


ArgType parse_arg_type(const char *arg)
{
    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) return ARG_HELP;
    if (strcmp(arg, "--dist") == 0) return ARG_DISTRIB;
    if (strcmp(arg, "--seed") == 0) return ARG_SEED;
    if (strcmp(arg, "-n") == 0) return ARG_COUNT;
    if (strcmp(arg, "-f") == 0) return ARG_DIR;
    if (strcmp(arg, "--mean") == 0) return ARG_MEAN;
    if (strcmp(arg, "--stddev") == 0) return ARG_STDDEV;
    if (strcmp(arg, "--lambda") == 0) return ARG_LAMBDA;
    if (strcmp(arg, "--a") == 0) return ARG_A;
    if (strcmp(arg, "--b") == 0) return ARG_B;
    return ARG_UNKNOWN;
}

void print_help(const char *prog_name)
{
    printf("Usage: %s [--seed <uint64_t>] [-n <uint32_t>] [--help]\n", prog_name);
    printf("  --dist               Probability distribution to be sampled {'unif', 'normal', 'exp'}\n");
    printf("  --seed <uint64_t>    Set the seed for the RNG (default: 1337)\n");
    printf("  -n <uint32_t>        Number of random numbers to generate (default: 1)\n");
    printf("  -f <string>          output file\n");
    printf("  --help or -h         Display this help message\n");
}

void parse_args(int argc, char **argv, ParsedArgs *args)
{
    args->dist = DIST_UNIF;
    args->seed = 1337;
    args->n = 1;
    args->provided_seed = 0;
    args->output_path = NULL;
    args->provided_out = 0;

    for (int i = 1; i < argc; i++)
    {
        if (parse_arg_type(argv[i]) == ARG_DISTRIB)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "Provide a value of --dist, see help for more info.\n");
                exit(EXIT_FAILURE);
            }
            args->dist = parse_dist(argv[++i]);
            break;
        }
    }

    // Setting defaults for distribution parameters 

    switch (args->dist)
    {
        case DIST_UNIF:
            args->params.unif.a = 0.;
            args->params.unif.b = 1.;
            break;
        case DIST_UINT:
            args->params.uint.a = UINT64_MAX;
            break;
        case DIST_NORMAL:
            args->params.normal.mean = 0.;
            args->params.normal.stddev = 1.;
            break;
        case DIST_EXP:
            args->params.exp.lambda = 1.;
            break;
    }

    for (int i = 1; i < argc; i++)
    {
        switch (parse_arg_type(argv[i]))
        {
            case ARG_DISTRIB:
                i++;
                break;
            case ARG_HELP:
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            case ARG_SEED:
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Provide a value for --seed, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->seed = strtoull(argv[++i], NULL, 10);
                args->provided_seed = 1;
                break;
            case ARG_COUNT:
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Provide a value for -n, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->n = strtoull(argv[++i], NULL, 10);
                break;
            case ARG_DIR:
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Provide a value for -f, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->output_path = argv[++i];
                args->provided_out = 1;
                break;
            case ARG_MEAN:
                if (args->dist != DIST_NORMAL)
                {
                    fprintf(stderr, "--mean only valid for normal distribution\n");
                    exit(EXIT_FAILURE);
                }
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Missing value after --mean\n");
                    exit(EXIT_FAILURE);
                }
                args->params.normal.mean = atof(argv[++i]);
                break;
            case ARG_STDDEV:
                if (args->dist != DIST_NORMAL)
                {
                    fprintf(stderr, "--stddev only valid for normal distribution\n");
                    exit(EXIT_FAILURE);
                }
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Missing value after --stddev\n");
                    exit(EXIT_FAILURE);
                }
                args->params.normal.stddev = atof(argv[++i]);
                break;
            case ARG_LAMBDA:
                if (args->dist != DIST_EXP)
                {
                    fprintf(stderr, "--lambda only valid for exponential distribution\n");
                    exit(EXIT_FAILURE);
                }
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Missing value after --lambda\n");
                    exit(EXIT_FAILURE);
                }
                args->params.exp.lambda = atof(argv[++i]);
                break;
            case ARG_A:
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Missing value after --a\n");
                    exit(EXIT_FAILURE);
                }
                if (args->dist == DIST_UNIF)
                {
                    args->params.unif.a = atof(argv[++i]);
                } else if (args->dist == DIST_UINT)
                {
                    args->params.uint.a = strtoull(argv[++i], NULL, 10);
                } else
                {
                    fprintf(stderr, "--a only valid for uniform and uint distributions\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case ARG_B:
                if (args->dist != DIST_UNIF)
                {
                    fprintf(stderr, "--b only valid for uniform distributions\n");
                    exit(EXIT_FAILURE);
                }
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "Missing value after --b\n");
                    exit(EXIT_FAILURE);
                }
                args->params.unif.b = atof(argv[++i]);
                break;
            case ARG_UNKNOWN:
            default:
               fprintf(stderr, "%s is not a valid flag, see help for more info.\n", argv[i]);
               exit(EXIT_FAILURE);
        }
    }
}

