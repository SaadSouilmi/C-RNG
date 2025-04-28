#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdio.h>
#include <stdint.h>
#include "ran.h"
#include "args.h"

Distrib parse_dist(const char *dist);
void sample(Ran *rng, ParsedArgs args, FILE *out);
void sample_unif(Ran *rng, uint64_t n, FILE *out);
void sample_uint(Ran *rng, uint64_t n, FILE *out);
void sample_zigg_normal(Ran *rng, uint64_t n, FILE *out);
void sample_zigg_exp(Ran *rng, uint64_t n, FILE *out);

#endif
