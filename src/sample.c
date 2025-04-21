#include "sample.h"
#include "ran.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Distrib parse_dist(const char *dist)
{
    if (strcmp(dist, "unif") == 0) return DIST_UNIF;
    if (strcmp(dist, "uint") == 0) return DIST_UINT; 
    if (strcmp(dist, "normal") == 0) return DIST_NORMAL;
    fprintf(stderr, "Invalid distribution, see docs for more info.\n");
    exit(EXIT_FAILURE);
}

void sample(Ran *rng, uint64_t n, FILE *out, Distrib dist)
{
    switch (dist)
    {
        case DIST_UNIF:
            sample_unif(rng, n, out);
            break;
        case DIST_UINT:
            sample_uint(rng, n, out);
            break;
        case DIST_NORMAL:
            sample_zigg(rng, n, out);
            break;
    }
}

void sample_unif(Ran *rng, uint64_t n, FILE *out)
{
    for (uint64_t i = 0; i < n; i++)
    {
        fprintf(out, "%.10f\n", ran_doub(rng));
    }
}

void sample_uint(Ran *rng, uint64_t n, FILE *out)
{
    for (uint64_t i = 0; i < n; i++)
    {
        fprintf(out, "%llu\n", ran_uint64(rng));
    }
}

void sample_zigg(Ran *rng, uint64_t n, FILE *out)
{
    initialize_zigg_params();
    for (uint64_t i = 0; i < n; i++)
    {
      fprintf(out, "%.10f\n", ran_normal_ziggurat(rng));
    }
}

