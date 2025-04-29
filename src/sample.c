#include "sample.h"
#include "args.h"
#include "ran.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h> 

Distrib parse_dist(const char *dist)
{
    if (strcmp(dist, "unif") == 0) return DIST_UNIF;
    if (strcmp(dist, "uint") == 0) return DIST_UINT; 
    if (strcmp(dist, "normal") == 0) return DIST_NORMAL;
    if (strcmp(dist, "exp") == 0) return DIST_EXP;
    fprintf(stderr, "Invalid distribution, see docs for more info.\n");
    exit(EXIT_FAILURE);
}

static void sample_unif_(Ran *rng, uint64_t n, double a, double b, char *buffer, size_t *offset);
static void sample_uint_(Ran *rng, uint64_t n, uint64_t a, char *buffer, size_t *offset);
static void sample_zigg_normal_(Ran *rng, uint64_t n, double mean, double stddev, char *buffer, size_t *offset);
static void sample_zigg_exp_(Ran *rng, uint64_t n, double lambda, char *buffer, size_t *offset);


void sample(Ran *rng, const ParsedArgs *args, FILE *out)
{
    Ran *rngs = malloc(args->n_threads * sizeof(Ran));
    if (rngs == NULL)
    {
        fprintf(stderr, "Mayday Mayday, the heap has failed I repeat the heap has failed.\n");
        exit(EXIT_FAILURE);
    }
    // Again poor man's split, need to change that later 
    for (size_t i = 0; i < args->n_threads; i++)
    {
        ran_split(rng, &rngs[i]);
    }
    
    uint64_t samples_per_thread = args->n / args->n_threads;
    uint64_t remainder = args->n % args->n_threads; 

    switch (args->dist)
    {
        case DIST_NORMAL:
            initialize_zigg_params();
            break;
        case DIST_EXP:
            initialize_zigg_exp_params();
            break;
        default:
            break;
    }
        
    #pragma omp parallel num_threads(args->n_threads)
    {
        int thread_id = omp_get_thread_num();
        Ran thread_rng = rngs[thread_id];
        uint64_t n_samples = samples_per_thread;
        if (thread_id < remainder) n_samples++;

        char *buffer = malloc(n_samples * 32);
        if (buffer == NULL)
        {
            fprintf(stderr, "Mayday Mayday, the heap has failed I repeat the heap has failed.\n");
            exit(EXIT_FAILURE);
        }
        size_t offset = 0;
        switch (args->dist)
        {
            case DIST_UNIF:
                sample_unif_(&thread_rng, n_samples, args->params.unif.a, args->params.unif.b, buffer, &offset);
                break;
            case DIST_UINT:
                sample_uint_(&thread_rng, n_samples, args->params.uint.a, buffer, &offset);
                break;
            case DIST_NORMAL:
                sample_zigg_normal_(&thread_rng, n_samples, args->params.normal.mean, args->params.normal.stddev, buffer, &offset);
                break;
            case DIST_EXP:
                sample_zigg_exp_(&thread_rng, n_samples, args->params.exp.lambda, buffer, &offset);
                break;
        }
        #pragma omp critical
        fwrite(buffer, 1, offset, out);
        free(buffer);
    }

    free(rngs);

}

static void sample_unif_(Ran *rng, uint64_t n, double a, double b, char *buffer, size_t *offset)
{
    for (size_t i = 0; i < n; i++) 
        *offset += snprintf(buffer + *offset, 32, "%.10f\n", a + (b - a) * ran_doub(rng)); 
}

static void sample_uint_(Ran *rng, uint64_t n, uint64_t a, char *buffer, size_t *offset)
{
    for (size_t i= 0; i < n; i++)
        *offset += snprintf(buffer + *offset, 32, "%llu\n", ran_range(rng, a));
}

static void sample_zigg_normal_(Ran *rng, uint64_t n, double mean, double stddev, char *buffer, size_t *offset)
{
    for (uint64_t i = 0; i < n; i++)
    {
      *offset += snprintf(buffer + *offset, 32, "%.10f\n", ran_normal_ziggurat(rng) * stddev + mean);
    }
}

static void sample_zigg_exp_(Ran *rng, uint64_t n, double lambda, char *buffer, size_t *offset)
{
    for (uint64_t i = 0; i < n; i++)
    {
        *offset += snprintf(buffer + *offset, 32, "%.10f\n", ran_exp_ziggurat(rng) * lambda);
    }
}
