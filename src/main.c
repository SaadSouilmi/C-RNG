#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <stdio.h>
#include "ran.h"
#include "state.h"
#include "sample.h"

void print_help(const char *prog_name);
void parse_args(int argc, char* argv[], Distrib *dist, uint64_t *seed, int *provided_seed, uint64_t *n, char **output_path, int *provided_out);
    
int main(int argc, char **argv)
{
    char state_file[512];
    int path_found = get_state_file_path(state_file, sizeof(state_file));
    
    Distrib dist; 
    uint64_t seed, n; 
    int provided_seed, provided_out;
    char *output_path;
    parse_args(argc, argv, &dist, &seed, &provided_seed, &n, &output_path, &provided_out);
        
    Ran rng;
    if (provided_seed || !load_rng_state(&rng, state_file))
    {
        ran_init(&rng, seed);
    }

    FILE *out = stdout;
    if (provided_out){
        out = fopen(output_path, "w");
        if (!out){
            perror("Failed to open output file.");
            return 1;
        }
    }
    sample(&rng, n, out, dist);

    if (provided_out)
    {
       fclose(out);
    }

    if (path_found){
        save_rng_state(&rng, state_file);
    }

    return 0; 
}

void parse_args(int argc, char* argv[], Distrib *dist, uint64_t *seed, int *provided_seed, uint64_t *n, char **output_path, int *provided_out)
{
    // default values 
    *dist = DIST_UNIF;
    *seed = 1337;
    *n = 1;
    *provided_seed = 0;
    *provided_out = 0;
    *output_path = NULL;

    for (int i=1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") ==0)
        {
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        }
        if (strcmp(argv[i], "--dist") == 0 && i + 1 < argc)
        {
          *dist = parse_dist(argv[i+1]);
          i++;
        }
        if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc)
        {
            *seed = strtoull(argv[i+1], NULL, 10);
            *provided_seed = 1;
            i++;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            *n = strtoul(argv[i+1], NULL, 10);
            i++;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            *output_path = argv[i+1];
            *provided_out = 1;
            i++;
        }
    }
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


