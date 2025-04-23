#include "args.h"
#include "sample.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


ArgType parse_arg_type(const char *arg)
{
    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) return ARG_HELP;
    if (strcmp(arg, "--dist") == 0) return ARG_DISTRIB;
    if (strcmp(arg, "--seed") == 0) return ARG_SEED;
    if (strcmp(arg, "-n") == 0) return ARG_COUNT;
    if (strcmp(arg, "-f") == 0) return ARG_DIR;
    return ARG_UNKNOWN;
}

void print_help(const char *prog_name)
{
    printf("Usage: %s [--seed <uint64_t>] [-n <uint32_t>] [--help]\n", prog_name);
    printf("  --dist               Probability distribution to be sampled {'unif', 'normal'}\n");
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
        switch (parse_arg_type(argv[i]))
        {
            case ARG_HELP:
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            case ARG_DISTRIB:
                if (i + 1 >= argc)
                {
                    perror("Provide a value of --dist, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->dist = parse_dist(argv[i+1]);
                i++;
                break;
            case ARG_SEED:
                if (i + 1 >= argc)
                {
                    perror("Provide a value for --seed, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->seed = strtoull(argv[i+1], NULL, 10);
                args->provided_seed = 1;
                i++;
                break;
            case ARG_COUNT:
                if (i + 1 >= argc)
                {
                    perror("Provide a value for -n, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->n = strtoull(argv[i+1], NULL, 10);
                i++;
                break;
            case ARG_DIR:
                if (i + 1 >= argc)
                {
                    perror("Provide a value for -f, see help for more info.\n");
                    exit(EXIT_FAILURE);
                }
                args->output_path = argv[i+1];
                args->provided_out = 1;
                i++;
                break;
            case ARG_UNKNOWN:
            default:
               printf("%s is not a valid flag, see help for more info.\n", argv[i]);
               exit(EXIT_FAILURE);
        }
    }
}

