
#include <iostream>
#include "immintrin.h"
#include "xmmintrin.h"

#define ITER 10
#define EPSILLON 0.01

int main(int argc , char *argv[]) {
    
    double x = 2.051881;
    float y = 6.1556156;

    double a[] __attribute__ ((aligned(32))) = {1.15, 7.32, 4.63, 6.24};
    double b[] __attribute__ ((aligned(32))) = {2.03, 8.32, 1.06, 9.32};

    __m256d vv1d, vv2d;

    vv1d = _mm256_load_pd(a);
    vv2d = _mm256_load_pd(b);

    #pragma omp parallel for reduction(+:x,y)
    for(int i = 0; i < ITER ; i++) {
        x += (i+1) + (1/(i+1));
        y += (i+1) - (1/(i+1));
    }

    printf("x : %lf\ty : %lf",x,y);
    if(x >= 58.051881-EPSILLON && x <= 58.051881+EPSILLON && y >= 60.155617-EPSILLON && y <= 60.155617+EPSILLON) std::cout << "\t\t\t\t\033[1;32m[ OK ]\033[0m\n"; else std::cout  << "\t\t\t\t\033[1;31m[ KO ]\033[0m\n";
    
    #pragma omp parallel for
    for(int i = 0 ; i < ITER ; i++) {
        #pragma omp critical
        {
            vv1d = _mm256_add_pd(vv1d, vv2d);
        }
    }

    _mm256_store_pd(a , vv1d);

    printf("a[] : ");
    for(int i = 0 ; i < 4 ; i++) printf("%lf ",a[i]);
    if( a[0] >= 21.45-EPSILLON && a[0] <= 21.45+EPSILLON && 
        a[1] >= 90.52-EPSILLON && a[1] <= 90.52+EPSILLON && 
        a[2] >= 15.23-EPSILLON && a[2] <= 15.23+EPSILLON && 
        a[3] >= 99.44-EPSILLON && a[3] <= 99.44+EPSILLON) 

    std::cout << "\t\t\033[1;32m[ OK ]\033[0m\n"; else std::cout  << "\t\t\033[1;31m[ KO ]\033[0m\n";
    
    return 123456;
}