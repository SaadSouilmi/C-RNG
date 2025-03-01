// RNG from Numerical recipes book : https://numerical.recipes/book.html
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> 
#include <math.h>

const double PI = 3.14159265358979323846;

// RNG state 
typedef struct {
    uint64_t u;
    uint64_t v;
    uint64_t w;
} Ran;

uint64_t ran_int64(Ran* rng);

// Initialise with integer seed 
void ran_init(Ran* rng, uint64_t seed)
{
    uint64_t x;
    rng->v = 4101842887655102017LL;
    rng->w = 1;
    rng->u = seed ^ rng->v;
    x = ran_int64(rng);
    rng->v = rng->u;
    x = ran_int64(rng);
    rng->w = rng->v;
    x = ran_int64(rng);
}

// Sample uniformly from 64bit ints
uint64_t ran_int64(Ran* rng)
{
    rng->u = rng->u * 2862933555777941757LL + 7046029254386353087LL;
    rng->v ^= rng->v >> 17;
    rng->v ^= rng->v << 31;
    rng->v ^= rng->v >> 8;
    rng->w = 4294957665U*(rng->w & 0xffffffff) + (rng->w >> 32);
    uint64_t x = rng->u ^ (rng->u << 21);
    x ^= x >> 35;
    x ^= x << 4;

    return (x + rng->v) ^ rng->w;
}

// Sample uniformly from 32bit ints
uint32_t ran_int32(Ran* rng)
{
    return (uint32_t)ran_int64(rng);
}

// Sample uniformly from doubles in [0, 1]
double ran_doub(Ran* rng)
{
    return 5.42101086242752217E-20 * ran_int64(rng); 
}


// Sample from X ~ Exp(\lambda) where E[X] = \lambda
double ran_exp(Ran* rng, double lambda)
{
    if (lambda <= 0)
    {
        fprintf(stderr, "Error: lambda must be positive.\n");
        return NAN;
    } 
    double x;
    do x = ran_doub(rng); while(x == 0.);

    return -log(x) * lambda;
}

// Sample (X, Y) ~ N(0, I_2) using Box-Muller
double* ran_normal_bm(Ran* rng)
{
    double u, v;
    do u = ran_doub(rng); while(u == 0.);
    v = ran_doub(rng);

    double r = sqrt(-2 * log(u));
    double theta = 2 * PI * v;

    double* sample = (double*)malloc(2 * sizeof(double));
    if (sample == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    sample[0] = r * cos(theta);
    sample[1] = r * sin(theta);

    return sample;
}

// Sample (X, Y) ~ N(0, I_2) using Polar method / Marsaglia
double* ran_normal_polar(Ran* rng)
{
    double u, v, r_squared;
    do
    {
        u = ran_doub(rng);
        v = ran_doub(rng);
        r_squared = u*u + v*v;
    } while ((r_squared >= 1.) || (r_squared == 0.));

    double* sample = (double*)malloc(2 * sizeof(double));
    if (sample == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    double r = sqrt(-2 * log(r_squared) / r_squared);
    sample[0] = u * r;
    sample[1] = v * r;
    
    return sample;
}

// Sample X ~ N(0, 1) using ziggurat method
double ran_ziggurat(Ran* rng)
{
    return 0.;
}

