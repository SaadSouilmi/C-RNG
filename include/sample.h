#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdio.h>
#include <stdint.h>
#include "ran.h"
#include "args.h"

Distrib parse_dist(const char *dist);
void sample(Ran *rng, ParsedArgs args, FILE *out);
void sample_unif(Ran *rng, uint64_t n, double a, double b, FILE *out);
void sample_uint(Ran *rng, uint64_t n, uint64_t a, FILE *out);
void sample_zigg_normal(Ran *rng, uint64_t n, double mean, double stddev, FILE *out);
void sample_zigg_exp(Ran *rng, uint64_t n, double lambda, FILE *out);

#endif
