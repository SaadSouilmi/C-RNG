// RNG from Numerical recipes book : https://numerical.recipes/book.html
#include "ran.h"
#include <math.h>
#include <stdint.h>

static const int ZIGG_C = 128;
static const double ZIGG_R = 3.442619855899;
static const double ZIGG_V = 9.91256303526217e-3;
static double ZIGG_X[ZIGG_C+1], ZIGG_XX[ZIGG_C];

// RNG ref: https://numerical.recipes/book.html
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

uint32_t ran_uint32(Ran* rng)
{
    return (uint32_t)ran_uint64(rng);
}

// Sample uniformly [0, 1]
double ran_doub(Ran* rng)
{
    return 5.42101086242752217E-20 * ran_uint64(rng); 
}


static void initialize_zigg_params_(void)
{
    double f = exp(-0.5 * ZIGG_R * ZIGG_R);
    ZIGG_X[0] = ZIGG_V / f;
    ZIGG_X[1] = ZIGG_R;
    ZIGG_X[ZIGG_C] = 0;

    for (int i = 2; i < ZIGG_C; i++)
    {
        ZIGG_X[i] = sqrt(-2 * log(f + ZIGG_V / ZIGG_X[i - 1]));
        f = exp(-0.5 * ZIGG_X[i] * ZIGG_X[i]);
    }
    for (int i = 0; i < ZIGG_C; i++){
        ZIGG_XX[i] = ZIGG_X[i+1] / ZIGG_X[i];
    }
}

void initialize_zigg_params(void)
{
    static int zigg_inited = 0;
    if (!zigg_inited) {
        initialize_zigg_params_();
        zigg_inited = 1;
    }
    
}

// Ref: https://apps.dtic.mil/sti/tr/pdf/AD0423993.pdf
double ran_normal_tail(Ran* rng, double a, int sign)
{
    double u, v, x;
    do
    {
        u = ran_doub(rng);
        v = ran_doub(rng);
        x = sqrt(a*a - 2*log(u));
    } while (v >= a / x);

    // sign == 1 for positive and 0 for negative
    return sign ? x: -x;
}

// Ref: https://www.doornik.com/research/ziggurat.pdf
double ran_normal_ziggurat(Ran* rng)
{   
    double u, x, f0, f1;
    uint8_t i;

    for (;;)
    {
        u = 2 * ran_doub(rng) - 1;
        i = ran_uint64(rng) & 0x7F;
        x = u * ZIGG_X[i];

        if (fabs(u) < ZIGG_XX[i])
        {
            return x;
        }
        if (i == 0)
        {
            return ran_normal_tail(rng, ZIGG_R, u > 0);
        }
        
        f0 = exp(-0.5 * (ZIGG_X[i] * ZIGG_X[i] - x * x));
        f1 = exp(-0.5 * (ZIGG_X[i+1] * ZIGG_X[i+1] - x * x));

        if (f1 + ran_doub(rng) * (f0 - f1) < 1.0)
        {
            return x;
        }
     }
}



