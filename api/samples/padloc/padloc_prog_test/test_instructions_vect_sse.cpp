

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

    std::cout << "\n\n\t***** VECTORISED INSTRUCTIONS SSE *****\n\n";
    
    /* ADD */
    v_res_f_sse = _mm_add_ps(v_src0_f_sse , v_src1_f_sse);   _mm_store_ps(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_add_pd(v_src0_d_sse , v_src1_d_sse);   _mm_store_pd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , "ADDPS");
    if(    res_f_sse[0] >= -0.00483936-EPSILLON && res_f_sse[0] <= -0.00483936+EPSILLON &&
            res_f_sse[1] >= -1.46682-EPSILLON && res_f_sse[1] <= -1.46682+EPSILLON &&
            res_f_sse[2] >= -1.57362-EPSILLON && res_f_sse[2] <= -1.57362+EPSILLON &&
            res_f_sse[3] >= -0.971406-EPSILLON && res_f_sse[3] <= -0.971406+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , "ADDPD");
    if(     res_d_sse[0] >= -0.671491-EPSILLON && res_d_sse[0] <= -0.671491+EPSILLON &&
            res_d_sse[1] >= -1.64813-EPSILLON && res_d_sse[1] <= -1.64813+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    
    std::cout << std::endl;

    
    /* SUB */
    v_res_f_sse = _mm_sub_ps(v_src0_f_sse , v_src1_f_sse); _mm_store_ps(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_sub_pd(v_src0_d_sse , v_src1_d_sse); _mm_store_pd(res_d_sse , v_res_d_sse);
    
    print<float>(res_f_sse , 4 , "SUBPS");
    if(    res_f_sse[0] >= -1.79766-EPSILLON && res_f_sse[0] <= -1.79766+EPSILLON &&
            res_f_sse[1] >= -1.88727-EPSILLON && res_f_sse[1] <= -1.88727+EPSILLON &&
            res_f_sse[2] >= 0.435128-EPSILLON && res_f_sse[2] <= 0.435128+EPSILLON &&
            res_f_sse[3] >= -1.20108-EPSILLON && res_f_sse[3] <= -1.20108+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , "SUBPD");
    if(     res_d_sse[0] >= -0.293141-EPSILLON && res_d_sse[0] <= -0.293141+EPSILLON &&
            res_d_sse[1] >= 0.840497-EPSILLON && res_d_sse[1] <= 0.840497+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    std::cout << std::endl;
    

    /* MUL */
    v_res_f_sse = _mm_mul_ps(v_src0_f_sse , v_src1_f_sse); _mm_store_ps(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_mul_pd(v_src0_d_sse , v_src1_d_sse); _mm_store_pd(res_d_sse , v_res_d_sse);
    
    print<float>(res_f_sse , 4 , "MULPS");
    if(    res_f_sse[0] >= -0.807887-EPSILLON && res_f_sse[0] <= -0.807887+EPSILLON &&
            res_f_sse[1] >= -0.352564-EPSILLON && res_f_sse[1] <= -0.352564+EPSILLON &&
            res_f_sse[2] >= 0.57174-EPSILLON && res_f_sse[2] <= 0.57174+EPSILLON &&
            res_f_sse[3] >= -0.12474-EPSILLON && res_f_sse[3] <= -0.12474+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    print<double>(res_d_sse , 2 , "MULPD");
    if(     res_d_sse[0] >= 0.0912422-EPSILLON && res_d_sse[0] <= 0.0912422+EPSILLON &&
            res_d_sse[1] >= 0.502472-EPSILLON && res_d_sse[1] <= 0.502472+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    std::cout << std::endl;
    
    /* DIV */
    v_res_f_sse = _mm_div_ps(v_src0_f_sse , v_src1_f_sse); _mm_store_ps(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_div_pd(v_src0_d_sse , v_src1_d_sse); _mm_store_pd(res_d_sse , v_res_d_sse);
						
    print<float>(res_f_sse , 4 , "DIVPS");
    if(    res_f_sse[0] >= -1.0054-EPSILLON && res_f_sse[0] <= -1.0054+EPSILLON &&
            res_f_sse[1] >= -7.97721-EPSILLON && res_f_sse[1] <= -7.97721+EPSILLON &&
            res_f_sse[2] >= 0.566768-EPSILLON && res_f_sse[2] <= 0.566768+EPSILLON &&
            res_f_sse[3] >= -9.45904-EPSILLON && res_f_sse[3] <= -9.45904+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , "DIVPD");
    if(     res_d_sse[0] >= 2.54957-EPSILLON && res_d_sse[0] <= 2.54957+EPSILLON &&
            res_d_sse[1] >= 0.324529-EPSILLON && res_d_sse[1] <= 0.324529+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;


    return 0;
}
