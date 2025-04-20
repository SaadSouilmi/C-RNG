#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ran.h"
#include "stats.h"

void malloc_err() {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
}

int main()
{
    uint64_t seed = 1337;
    Ran rng;
    ran_init(&rng, seed);
    clock_t start, end;
    double cpu_time_used;

    size_t n_samples = 100000; 
    double* bm_samples = (double*)malloc(2 * n_samples * sizeof(double));
    if (bm_samples == NULL) malloc_err();

    start = clock();

    for (size_t i=0; i<2*n_samples; i+=2)
    {
        double* sample = ran_normal_bm(&rng);
        bm_samples[i] = sample[0];
        bm_samples[i+1] = sample[1];
        free(sample);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Box-Muller: Mean = %lf, Variance = %lf\n", mean_double(bm_samples, 2*n_samples), var_double(bm_samples, 2*n_samples));
    printf("Time taken: %f seconds\n", cpu_time_used);
    free(bm_samples);

    ran_init(&rng, seed);
    double* polar_samples = (double*)malloc(2 * n_samples * sizeof(double));
    if (polar_samples == NULL) malloc_err();

    start = clock();

    for (size_t i=0; i<2*n_samples; i+=2)
    {
        double* sample = ran_normal_polar(&rng);
        polar_samples[i] = sample[0];
        polar_samples[i+1] = sample[1];
        free(sample);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Marsaglia: Mean = %lf, Variance = %lf\n", mean_double(polar_samples, 2*n_samples), var_double(polar_samples, 2*n_samples));
    printf("Time taken: %f seconds\n", cpu_time_used);
    free(polar_samples);

    double lambda = 3.;
    ran_init(&rng, seed);
    double* exp_samples = (double*)malloc(n_samples * sizeof(double));
    if (exp_samples == NULL) malloc_err();

    start = clock();

    for (size_t i=0; i<n_samples; i+=1)
    {
        exp_samples[i] = ran_exp(&rng, lambda);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Exp CDF^{-1}: Mean = %lf, Variance = %lf\n", mean_double(exp_samples, n_samples), var_double(exp_samples, n_samples));
    printf("Time taken: %f seconds\n", cpu_time_used);
    free(exp_samples);

    return 0;
}