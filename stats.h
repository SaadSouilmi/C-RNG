#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include <stdint.h>

double mean_int32(const int32_t* data, size_t length);
double var_int32(const int32_t* data, size_t length);
double mean_int64(const int64_t* data, size_t length);
double var_int64(const int64_t* data, size_t length);
double mean_float(const float* data, size_t length);
double var_float(const float* data, size_t length);
double mean_double(const double* data, size_t length);
double var_double(const double* data, size_t length);

#endif // STATS_H