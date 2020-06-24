
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "immintrin.h"
#include "mmintrin.h"

 
void test(double a , double b , double *res) {
    //printf("In callllllll\n");
    //double aaaa  = a+b;


    //double r = a+b;
    
    *res = a+3*b;

    //printf("AFTER ADDITION IN FUNC\n");
    
}


int main(int argc , char *argv[]) {
    
    double a=10 , b=20, res=500;

    //*res=a+b;
    //printf("THIS IS THE ENDD : %p\n",res);

    //printf("Res addr : %p\n",res);
    test(a,b,&res);

    printf("MID : %f\n",res);

    //#pragma omp parallel for reduction(+:res)
    for(long i = 0 ; i < 10 ; i++) {
        res += (a*b);
    }

    printf("END : %f\n",res);


    __declspec(align(32)) float AAAAAAA[]  = {1,2,3,4}, BBBBBBB[] = {9,10,11,12} , CCCCCCC[] = {0,0,0,0} ;

    __m128 v1 = _mm_load_ps(AAAAAAA);
    __m128 v2 = _mm_load_ps(BBBBBBB);
    __m128 v3 = v1;

    #pragma omp parallel for
    for(int i = 0 ; i < 10 ; i++) {
        #pragma omp critical
         v1 = _mm_add_ps(v2 , v1);
    }
    

    _mm_store_ps(CCCCCCC , v1);

    printf("CCCCCCCC : ");
    for(int i = 0 ; i < 4 ; i++) {
        printf("%.2f ",CCCCCCC[i]);
    }
    printf("\n");   
    
    __declspec(align(32)) double DDDD[]  = {10,100,1000,10000} , EEEE[]  = {20,200,2000,20000} , FFFF[]  = {0,0,0,0};

    __m256d vv1 = _mm256_load_pd(DDDD);
    __m256d vv2 = _mm256_load_pd(EEEE);
    __m256d vv3 = _mm256_add_pd(vv1 , vv2);

    _mm256_store_pd(FFFF , vv3);

    printf("FFFF : ");
    for(int i = 0 ; i < 4 ; i++) {
        printf("%.2f ",FFFF[i]);
    }
    printf("\n"); 


}