#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>


double mean_int32(const int32_t* data, size_t length)
{
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) sum += (double)data[i] / length;
    
    return sum;
}

double var_int32(const int32_t* data, size_t length)
{
    double sum = 0.0;
    double m = mean_int32(data, length);
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) 
    {
    sum += ((double)data[i] - m) * ((double)data[i] - m) / length;
    }

    return sum;
}



double mean_int64(const int64_t* data, size_t length)
{
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) sum += (double)data[i] / length;
    
    return sum;
}

double var_int64(const int64_t* data, size_t length)
{
    double sum = 0.0;
    double m = mean_int64(data, length);
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) 
    {
    sum += ((double)data[i] - m) * ((double)data[i] - m) / length;
    }

    return sum;
}


double mean_float(const float* data, size_t length)
{
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) sum += (double)data[i] / length;
    
    return sum;
}

double var_float(const float* data, size_t length)
{
    double sum = 0.0;
    double m = mean_float(data, length);
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) 
    {
    sum += ((double)data[i] - m) * ((double)data[i] - m) / length;
    }

    return sum;
}


double mean_double(const double* data, size_t length)
{
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) sum += (double)data[i] / length;
    
    return sum;
}

double var_double(const double* data, size_t length)
{
    double sum = 0.0;
    double m = mean_double(data, length);
    #pragma omp parallel for reduction(+:sum)
    for (size_t i=0; i<length; i++) 
    {
    sum += ((double)data[i] - m) * ((double)data[i] - m) / length;
    }

    return sum;
}


