#include <iostream>
#include "immintrin.h"
#include "xmmintrin.h"

#define ITER 10

int main(int argc , char *argv[]) {
    
    double x = 2.051881;
    float y = 6.1556156;

    __declspec(align(32)) double a[] = {1.15, 7.32, 4.63, 6.24};
    __declspec(align(32)) double b[] = {2.03, 8.32, 1.06, 9.32};

    __m256d vv1d, vv2d;

    vv1d = _mm256_load_pd(a);
    vv2d = _mm256_load_pd(b);

    #pragma omp parallel for reduction(+:x,y)
    for(int i = 0; i < ITER ; i++) {
        x += (i+1) + (1/(i+1));
        y += (i+1) - (1/(i+1));
    }

    printf("x : %lf\ty : %lf\n",x,y);
    
    

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
    printf("\n");



    return 123456;
}