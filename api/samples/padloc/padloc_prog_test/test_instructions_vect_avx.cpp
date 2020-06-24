#include "test_instructions.h"

#define EPSILLON 0.01

int main(int argc, char const *argv[])
{   
    /* Default seed get a static test case for comparison when running with dynamoRIO */
    unsigned int seed = 42;

    if(argc == 2) seed = atoi(argv[1]);

    init(seed);

    v_src0_f_avx = _mm256_load_ps(src0_f_avx);
    v_src1_f_avx = _mm256_load_ps(src1_f_avx);
    v_src2_f_avx = _mm256_load_ps(src2_f_avx);
    v_src0_d_avx = _mm256_load_pd(src0_d_avx);
    v_src1_d_avx = _mm256_load_pd(src1_d_avx);
    v_src2_d_avx = _mm256_load_pd(src2_d_avx);


    std::cout << "\n\t***** VECTORISED INSTRUCTIONS AVX *****\n\n";

    v_res_f_avx = _mm256_add_ps(v_src0_f_avx , v_src1_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_add_pd(v_src0_d_avx , v_src1_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VADDPS"); 
    if(     res_f_avx[0] >= -0.478685-EPSILLON && res_f_avx[0] <= -0.478685+EPSILLON &&
            res_f_avx[1] >= 0.108127-EPSILLON && res_f_avx[1] <= 0.108127+EPSILLON &&
            res_f_avx[2] >= -0.196688-EPSILLON && res_f_avx[2] <= -0.196688+EPSILLON &&
            res_f_avx[3] >= -1.44725-EPSILLON && res_f_avx[3] <= -1.44725+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    print<double>(res_d_avx , 2 , "VADDPD");
    if(     res_d_avx[0] >= 0.416576-EPSILLON && res_d_avx[0] <= 0.416576+EPSILLON &&
            res_d_avx[1] >= 0.731816-EPSILLON && res_d_avx[1] <= 0.731816+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    

    std::cout << std::endl;


    v_res_f_avx = _mm256_sub_ps(v_src0_f_avx , v_src1_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_sub_pd(v_src0_d_avx , v_src1_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VSUBPS");
    if(     res_f_avx[0] >= -0.501625-EPSILLON && res_f_avx[0] <= -0.501625+EPSILLON &&
            res_f_avx[1] >= 1.73199-EPSILLON && res_f_avx[1] <= 1.73199+EPSILLON &&
            res_f_avx[2] >= 0.437793-EPSILLON && res_f_avx[2] <= 0.437793+EPSILLON &&
            res_f_avx[3] >= 3.68788-EPSILLON && res_f_avx[3] <= 3.68788+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    print<double>(res_d_avx , 2 , "VSUBPD");
    if(     res_d_avx[0] >= -0.381002-EPSILLON && res_d_avx[0] <= -0.381002+EPSILLON &&
            res_d_avx[1] >= 0.463603-EPSILLON && res_d_avx[1] <= 0.463603+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    std::cout << std::endl;


    v_res_f_avx = _mm256_mul_ps(v_src0_f_avx , v_src1_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_mul_pd(v_src0_d_avx , v_src1_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VMULPS");
    if(     res_f_avx[0] >= -0.00562214-EPSILLON && res_f_avx[0] <= -0.00562214+EPSILLON &&
            res_f_avx[1] >= -0.747024-EPSILLON && res_f_avx[1] <= -0.747024+EPSILLON &&
            res_f_avx[2] >= -0.0382441-EPSILLON && res_f_avx[2] <= -0.0382441+EPSILLON &&
            res_f_avx[3] >= -2.87648-EPSILLON && res_f_avx[3] <= -2.87648+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    print<double>(res_d_avx , 2 , "VMULPD");
    if(     res_d_avx[0] >= 0.00709311-EPSILLON && res_d_avx[0] <= 0.00709311+EPSILLON &&
            res_d_avx[1] >= 0.0801569-EPSILLON && res_d_avx[1] <= 0.0801569+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;

    v_res_f_avx = _mm256_div_ps(v_src0_f_avx , v_src1_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_div_pd(v_src0_d_avx , v_src1_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VDIVPS");
     if(     res_f_avx[0] >= -42.7332-EPSILLON && res_f_avx[0] <= -42.7332+EPSILLON &&
            res_f_avx[1] >= -1.13317-EPSILLON && res_f_avx[1] <= -1.13317+EPSILLON &&
            res_f_avx[2] >= -0.380003-EPSILLON && res_f_avx[2] <= -0.380003+EPSILLON &&
            res_f_avx[3] >= -0.436332-EPSILLON && res_f_avx[3] <= -0.436332+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    
    
    print<double>(res_d_avx , 2 , "VDIVPD");
    if(     res_d_avx[0] >= 0.0446016-EPSILLON && res_d_avx[0] <= 0.0446016+EPSILLON &&
            res_d_avx[1] >= 4.45696-EPSILLON && res_d_avx[1] <= 4.45696+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    std::cout << std::endl;
    
    return 0;
}
