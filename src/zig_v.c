#include <stdio.h>
#include <gsl/gsl_cdf.h>
#include <math.h>

// static const double M_PI = 3.14159265358979323846;

int main()
{
    const double R =  3.442619855899;
    double V;
    
    V = R * exp(-.5*R*R) + gsl_cdf_gaussian_Q(R, 1.0) * sqrt(2*M_PI);

    printf("V = %.20f\n", V);
}
