#include "sample.h"
#include "args.h"
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
    if (strcmp(dist, "exp") == 0) return DIST_EXP;
    fprintf(stderr, "Invalid distribution, see docs for more info.\n");
    exit(EXIT_FAILURE);
}

void sample(Ran *rng, ParsedArgs args, FILE *out)
{
    switch (args.dist)
    {
        case DIST_UNIF:
            sample_unif(rng, args.n, args.params.unif.a, args.params.unif.b, out);
            break;
        case DIST_UINT:
            sample_uint(rng, args.n, args.params.uint.a, out);
            break;
        case DIST_NORMAL:
            sample_zigg_normal(rng, args.n, args.params.normal.mean, args.params.normal.stddev, out);
            break;
        case DIST_EXP:
            sample_zigg_exp(rng, args.n, args.params.exp.lambda, out);
            break;
    }
}

void sample_unif(Ran *rng, uint64_t n, double a, double b, FILE *out)
{
    for (uint64_t i = 0; i < n; i++)
    {
        fprintf(out, "%.10f\n", a + (b - a) * ran_doub(rng));
    }
}

void sample_uint(Ran *rng, uint64_t n, uint64_t a, FILE *out)
{
    for (uint64_t i = 0; i < n; i++)
    {
        fprintf(out, "%llu\n", ran_range(rng, a));
    }
}

void sample_zigg_normal(Ran *rng, uint64_t n, double mean, double stddev, FILE *out)
{
    initialize_zigg_params();
    for (uint64_t i = 0; i < n; i++)
    {
      fprintf(out, "%.10f\n", ran_normal_ziggurat(rng) * stddev + mean);
    }
}

void sample_zigg_exp(Ran *rng, uint64_t n, double lambda, FILE *out)
{
    initialize_zigg_exp_params();
    for (uint64_t i = 0; i < n; i++)
    {
        fprintf(out, "%.10f\n", ran_exp_ziggurat(rng) * lambda);
    }
}
