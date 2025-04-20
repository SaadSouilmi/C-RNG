// RNG from Numerical recipes book : https://numerical.recipes/book.html
#include <math.h>
#include <stdint.h>



static const int ZIGG_C = 128;
static const double ZIGG_R = 3.442619855899;
static const double ZIGG_V = 9.91256303526217e-3;

// RNG state 
typedef struct {
    uint64_t u;
    uint64_t v;
    uint64_t w;
} Ran;

uint64_t ran_uint64(Ran* rng);

// Initialise with integer seed 
void ran_init(Ran* rng, uint64_t seed)
{
    uint64_t x;
    rng->v = 4101842887655102017LL;
    rng->w = 1;
    rng->u = seed ^ rng->v;
    x = ran_uint64(rng);
    rng->v = rng->u;
    x = ran_uint64(rng);
    rng->w = rng->v;
    x = ran_uint64(rng);
}

// Sample uniformly from 64bit unsingned ints
uint64_t ran_uint64(Ran* rng)
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

// Sample uniformly from 32bit unsigned ints
uint32_t ran_uint32(Ran* rng)
{
    return (uint32_t)ran_uint64(rng);
}

// Sample uniformly from doubles in [0, 1]
double ran_doub(Ran* rng)
{
    return 5.42101086242752217E-20 * ran_uint64(rng); 
}



// Initialising Ziggurat parameters 

static double ZIGG_X[ZIGG_C+1];

// Ref: https://apps.dtic.mil/sti/tr/pdf/AD0423993.pdf
double ran_normal_tail(Ran* rng, double a)
{
    double u, v, x;
    do
    {
        u = ran_doub(rng);
        v = ran_doub(rng);
        x = sqrt(a*a - 2*log(u));
    } while (v >= a / x);

    return x;
}


// Ref: https://www.doornik.com/research/ziggurat.pdf
double ran_normal_ziggurat(Ran* rng)
{   
    double sign;
    sign = (ran_doub(rng) > 0.5) ? 1. : -1.;


    return 0.;
}



