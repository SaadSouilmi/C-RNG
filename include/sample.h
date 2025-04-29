#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdio.h>
#include <stdint.h>
#include "ran.h"
#include "args.h"

Distrib parse_dist(const char *dist);
void sample(Ran *rng, const ParsedArgs *args, FILE *out);

#endif
