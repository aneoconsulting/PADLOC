
/*
* Created on 2019-08-06
*/

#ifndef TEST_INSTRUCTION_H
#define TEST_INSTRUCTION_H

#include <random>
#include <iostream>
#include "xmmintrin.h"
#include "immintrin.h"

#define aligned __attribute__((aligned(32)))

    /* SOURCES */

float   src0_f_sse[4] aligned, 
        src1_f_sse[4] aligned, 
        src2_f_sse[4] aligned,
        src0_f_avx[8] aligned,
        src1_f_avx[8] aligned,
        src2_f_avx[8] aligned;

double  src0_d_sse[2] aligned, 
        src1_d_sse[2] aligned, 
        src2_d_sse[2] aligned,
        src0_d_avx[4] aligned,
        src1_d_avx[4] aligned,
        src2_d_avx[4] aligned;

__m128   v_src0_f_sse, v_src1_f_sse, v_src2_f_sse;
__m128d  v_src0_d_sse, v_src1_d_sse, v_src2_d_sse;
__m256   v_src0_f_avx, v_src1_f_avx, v_src2_f_avx;
__m256d  v_src0_d_avx, v_src1_d_avx, v_src2_d_avx;

    /* RES */

float   res_f_sse[4] aligned,
        res_f_avx[8] aligned;

double  res_d_sse[2] aligned,
        res_d_avx[4] aligned;
    
__m128  v_res_f_sse;
__m128d v_res_d_sse;
__m256  v_res_f_avx;
__m256d v_res_d_avx;

const char * tabs = "\t\t\t\t";
const char * tabd = "\t\t\t\t\t\t\t\t\t\t";


template <typename FTYPE>
void print(FTYPE *values , int nb_values, const char *name) {
    std::cout << name << " : ";
    for(int i = 0 ; i < nb_values ; i++) {
        std::cout << std::fixed << values[i] << "\t\t";
    }
    std::cout << (sizeof(FTYPE) == 8 ? tabd : tabs);
}



void init(unsigned int seed) {
    std::normal_distribution<double> dist_double;
    std::normal_distribution<float> dist_float;
    std::default_random_engine generator(seed);

    for(int i = 0 ; i < 8 ; i++) {
        if(i < 4) {
            src0_f_sse[i] = dist_float(generator);
            src1_f_sse[i] = dist_float(generator);
            src2_f_sse[i] = dist_float(generator);
        }
        src0_f_avx[i] = dist_float(generator);
        src1_f_avx[i] = dist_float(generator);
        src2_f_avx[i] = dist_float(generator);
    }

    for(int i = 0 ; i < 4 ; i++) {
        if(i < 2) {
            src0_d_sse[i] = dist_double(generator);
            src1_d_sse[i] = dist_double(generator);
            src2_d_sse[i] = dist_double(generator);
        }
        src0_d_avx[i] = dist_double(generator);
        src1_d_avx[i] = dist_double(generator);
        src2_d_avx[i] = dist_double(generator);
    }
}




void print_all_src() {
    print<float>(src0_f_sse , 4 , "src0_f_sse"); std::cout << std::endl;
    print<float>(src1_f_sse , 4 , "src1_f_sse"); std::cout << std::endl;
    print<float>(src2_f_sse , 4 , "src2_f_sse"); std::cout << std::endl;
    print<float>(src0_f_avx , 8 , "src0_f_avx"); std::cout << std::endl;
    print<float>(src1_f_avx , 8 , "src1_f_avx"); std::cout << std::endl;
    print<float>(src2_f_avx , 8 , "src2_f_avx"); std::cout << std::endl;

    print<double>(src0_d_sse , 2 , "src0_d_sse"); std::cout << std::endl;
    print<double>(src1_d_sse , 2 , "src1_d_sse"); std::cout << std::endl;
    print<double>(src2_d_sse , 2 , "src2_d_sse"); std::cout << std::endl;
    print<double>(src0_d_avx , 4 , "src0_d_avx"); std::cout << std::endl;
    print<double>(src1_d_avx , 4 , "src1_d_avx"); std::cout << std::endl;
    print<double>(src2_d_avx , 4 , "src2_d_avx"); std::cout << std::endl;   
}

#endif