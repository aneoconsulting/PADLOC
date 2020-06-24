#include "test_instructions.h"

#define EPSILLON 0.01


int main(int argc, char const *argv[])
{
    /* Default seed get a static test case for comparison when running with dynamoRIO */
    unsigned int seed = 42;

    if(argc == 2) seed = atoi(argv[1]);

    init(seed);

    v_src0_f_sse = _mm_load_ps(src0_f_sse);
    v_src1_f_sse = _mm_load_ps(src1_f_sse);
    v_src2_f_sse = _mm_load_ps(src2_f_sse);
    v_src0_d_sse = _mm_load_pd(src0_d_sse);
    v_src1_d_sse = _mm_load_pd(src1_d_sse);
    v_src2_d_sse = _mm_load_pd(src2_d_sse);

    #ifdef __AVX__
        const char * name = "***** SCALAR INSTRUCTIONS AVX *****";
        const char * addss = "VADDSS";
        const char * addsd = "VADDSD";
        const char * subss = "VSUBSS";
        const char * subsd = "VSUBSD";
        const char * mulss = "VMULSS";
        const char * mulsd = "VMULSD";
        const char * divss = "VDIVSS";
        const char * divsd = "VDIVSD";
    #else
        const char * name = "***** SCALAR INSTRUCTIONS SSE *****";
        const char * addss = "ADDSS";
        const char * addsd = "ADDSD";
        const char * subss = "SUBSS";
        const char * subsd = "SUBSD";
        const char * mulss = "MULSS";
        const char * mulsd = "MULSD";
        const char * divss = "DIVSS";
        const char * divsd = "DIVSD";
    #endif

    /* *************************** SCALAR *************************** */
    std::cout << "\n\t" << name << "\n\n";

    /* ADD */
    v_res_f_sse = _mm_add_ss(v_src0_f_sse , v_src1_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_add_sd(v_src0_d_sse , v_src1_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);
    
    print<float>(res_f_sse , 4 , addss);
    if(    res_f_sse[0] >= -0.00483936-EPSILLON && res_f_sse[0] <= -0.00483936+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , addsd);
    if(     res_d_sse[0] >= -0.671491-EPSILLON && res_d_sse[0] <= -0.671491+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    
    /* SUB */
    v_res_f_sse = _mm_sub_ss(v_src0_f_sse , v_src1_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_sub_sd(v_src0_d_sse , v_src1_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , subss);
    if(    res_f_sse[0] >= -1.79766-EPSILLON && res_f_sse[0] <= -1.79766+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , subsd);
    if(     res_d_sse[0] >= -0.293141-EPSILLON && res_d_sse[0] <= -0.293141+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    

    /* MUL */
    v_res_f_sse = _mm_mul_ss(v_src0_f_sse , v_src1_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_mul_sd(v_src0_d_sse , v_src1_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , mulss);
    if(    res_f_sse[0] >= -0.807887-EPSILLON && res_f_sse[0] <= -0.807887+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , mulsd);
    if(     res_d_sse[0] >= 0.0912422-EPSILLON && res_d_sse[0] <= 0.0912422+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    
    /* DIV */
    v_res_f_sse = _mm_div_ss(v_src0_f_sse , v_src1_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_div_sd(v_src0_d_sse , v_src1_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , divss);
    if(    res_f_sse[0] >= -1.0054-EPSILLON && res_f_sse[0] <= -1.0054+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , divsd);
    if(     res_d_sse[0] >= 2.54957-EPSILLON && res_d_sse[0] <= 2.54957+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout  << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    
    return 0;
}   