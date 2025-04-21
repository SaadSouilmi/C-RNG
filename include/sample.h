#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdio.h>
#include <stdint.h>
#include "ran.h"

typedef enum {
    DIST_UNIF,
    DIST_UINT,
    DIST_NORMAL,
} Distrib;

Distrib parse_dist(const char *dist);
void sample(Ran *rng, uint64_t n, FILE *out, Distrib dist);
void sample_unif(Ran *rng, uint64_t n, FILE *out);
void sample_zigg(Ran *rng, uint64_t n, FILE *out);
void sample_uint(Ran *rng, uint64_t n, FILE *out);

#endif
