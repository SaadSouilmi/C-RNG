#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <stdio.h>
#include "ran.h"
#include "state.h"
#include "sample.h"
#include "args.h"


int main(int argc, char **argv)
{
    ParsedArgs args;
    parse_args(argc, argv, &args);

    char state_file[512];
    int path_found = get_state_file_subpath(state_file, sizeof(state_file), "state.bin");
        
    Ran rng;
    if (args.provided_seed || !load_rng_state(&rng, state_file))
    {
        ran_init(&rng, args.seed);
    }

    FILE *out = stdout;
    if (args.provided_out){
        out = fopen(args.output_path, "w");
        if (!out){
            perror("Failed to open output file.");
            return 1;
        }
    }
    sample(&rng, &args, out);

    if (args.provided_out)
    {
       fclose(out);
    }

    if (path_found){
        save_rng_state(&rng, state_file);
    }

    return 0; 
}
