#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <stdio.h>
#include "state.h"
#include "ran.h"


void parse_args(int argc, char* argv[], uint64_t *seed, int *provided_seed, uint32_t *n);
void sample(Ran *rng, uint32_t n);
    
int main(int argc, char **argv)
{
    char state_file[512];
    int path_found = get_state_file_path(state_file, sizeof(state_file));
    
    uint64_t seed; int provided_seed; uint32_t n;
    parse_args(argc, argv, &seed, &provided_seed, &n);
        
    Ran rng;
    if (provided_seed || !path_found || !load_rng_state(&rng, state_file))
    {
        ran_init(&rng, seed);
    }

    sample(&rng, n);

    if (path_found){
        save_rng_state(&rng, state_file);
    }

    return 0; 
}

void parse_args(int argc, char* argv[], uint64_t *seed, int *provided_seed, uint32_t *n)
{
    // default values 
    *seed = 1337;
    *provided_seed = 0;
    *n = 1;

    for (int i=1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], "--seed") == 0)
        {
            *seed = strtoull(argv[i+1], NULL, 10);
            *provided_seed = 1;
            i++;
        } else if (strcmp(argv[i], "-n") == 0) {
            *n = strtoul(argv[i+1], NULL, 10);
            i++;
        }
    }
}

void sample(Ran *rng, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++){
        printf("%f\n", ran_doub(rng));
    }
}
