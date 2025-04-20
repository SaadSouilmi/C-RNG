#ifndef RNG_H
#define RNG_H

#include <stdint.h>

// RNG state
typedef struct {
    uint64_t u;
    uint64_t v;
    uint64_t w;
} Ran;

// Function declarations
void ran_init(Ran* rng, uint64_t seed);
uint64_t ran_int64(Ran* rng);
uint32_t ran_int32(Ran* rng);
double ran_doub(Ran* rng);
double ran_normal_tail(Ran* rng, double a);
double ran_normal_ziggurat(Ran* rng);


#endif // RNG_H
