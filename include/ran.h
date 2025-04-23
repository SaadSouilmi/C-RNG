#ifndef RNG_H
#define RNG_H

#include <stdint.h>

// RNG state
typedef struct {
    uint64_t u;
    uint64_t v;
    uint64_t w;
} Ran;


void ran_init(Ran* rng, uint64_t seed);
uint64_t ran_uint64(Ran* rng);
uint32_t ran_uint32(Ran* rng);
double ran_doub(Ran* rng);
uint64_t ran_range(Ran* rng, uint64_t a);

// Ziggurat functions 
void initialize_zigg_params(void);
double ran_normal_tail(Ran* rng, double a, int sign);
double ran_normal_ziggurat(Ran* rng);


#endif
