#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "ran.h"
#include "stats.h"


int main()
{
    uint64_t seed = 1337;
    Ran rng;
    ran_init(&rng, seed);

    size_t n_samples = 100000; 
    double* bm_samples = (double*)malloc(2 * n_samples * sizeof(double));
    if (bm_samples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i=0; i<2*n_samples; i+=2){
        double* sample = ran_normal_bm(&rng);
        bm_samples[i] = sample[0];
        bm_samples[i+1] = sample[1];
        free(sample);
    }
    printf("Box-Muller: Mean = %lf, Variance = %lf\n", mean_double(bm_samples, 2*n_samples), var_double(bm_samples, 2*n_samples));
    free(bm_samples);

    ran_init(&rng, seed);
    double* polar_samples = (double*)malloc(2 * n_samples * sizeof(double));
    if (polar_samples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i=0; i<2*n_samples; i+=2){
        double* sample = ran_normal_bm(&rng);
        polar_samples[i] = sample[0];
        polar_samples[i+1] = sample[1];
        free(sample);
    }
    printf("Marsaglia: Mean = %lf, Variance = %lf\n", mean_double(polar_samples, 2*n_samples), var_double(polar_samples, 2*n_samples));
    free(polar_samples);

    return 0;
}