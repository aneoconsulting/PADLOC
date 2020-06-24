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
    v_src0_f_avx = _mm256_load_ps(src0_f_avx);
    v_src1_f_avx = _mm256_load_ps(src1_f_avx);
    v_src2_f_avx = _mm256_load_ps(src2_f_avx);
    v_src0_d_avx = _mm256_load_pd(src0_d_avx);
    v_src1_d_avx = _mm256_load_pd(src1_d_avx);
    v_src2_d_avx = _mm256_load_pd(src2_d_avx);

    std::cout << std::endl;

     /* SCALAR FMADD */
    v_res_f_sse = _mm_fmadd_ss(v_src0_f_sse , v_src1_f_sse , v_src2_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_fmadd_sd(v_src0_d_sse , v_src1_d_sse , v_src2_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , "FMADDSS");    
    if(     res_f_sse[0] >= -2.0106-EPSILLON && res_f_sse[0] <= -2.0106+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_sse , 2 , "FMADDSD");
    if(     res_d_sse[0] >= 0.281624-EPSILLON && res_d_sse[0] <= 0.281624+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    

    std::cout << std::endl;

    /* SCALAR FMSUB */
    v_res_f_sse = _mm_fmsub_ss(v_src0_f_sse , v_src1_f_sse , v_src2_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_fmsub_sd(v_src0_d_sse , v_src1_d_sse , v_src2_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , "FMSUBSS");
    if(    res_f_sse[0] >= 0.394827-EPSILLON && res_f_sse[0] <= 0.394827+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    
    print<double>(res_d_sse , 2 , "FMSUBSD");
     if(     res_d_sse[0] >= -0.0991397-EPSILLON && res_d_sse[0] <= -0.0991397+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;

    /* SCALAR NEG FMADD */
    v_res_f_sse = _mm_fnmadd_ss(v_src0_f_sse , v_src1_f_sse , v_src2_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_fnmadd_sd(v_src0_d_sse , v_src1_d_sse , v_src2_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , "FNMADDSS");
    if(     res_f_sse[0] >= -0.394827-EPSILLON && res_f_sse[0] <= -0.394827+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;


    print<double>(res_d_sse , 2 , "FNMADDSD");
     if(    res_d_sse[0] >= 0.0991397-EPSILLON && res_d_sse[0] <= 0.0991397+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;
    

    /* SCALAR NEG FMSUB */
    v_res_f_sse = _mm_fnmsub_ss(v_src0_f_sse , v_src1_f_sse , v_src2_f_sse); _mm_store_ss(res_f_sse , v_res_f_sse);
    v_res_d_sse = _mm_fnmsub_sd(v_src0_d_sse , v_src1_d_sse , v_src2_d_sse); _mm_store_sd(res_d_sse , v_res_d_sse);

    print<float>(res_f_sse , 4 , "FNMSUBSS");
    if(    res_f_sse[0] >= 2.0106-EPSILLON && res_f_sse[0] <= 2.0106+EPSILLON &&
            res_f_sse[1] >= 0.0-EPSILLON && res_f_sse[1] <= 0.0+EPSILLON &&
            res_f_sse[2] >= 0.0-EPSILLON && res_f_sse[2] <= 0.0+EPSILLON &&
            res_f_sse[3] >= 0.0-EPSILLON && res_f_sse[3] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;


    print<double>(res_d_sse , 2 , "FNMSUBSD");
    if(     res_d_sse[0] >=  -0.281624-EPSILLON && res_d_sse[0] <=  -0.281624+EPSILLON &&
            res_d_sse[1] >= 0.0-EPSILLON && res_d_sse[1] <= 0.0+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;

    v_res_f_avx = _mm256_fmadd_ps(v_src0_f_avx , v_src1_f_avx, v_src2_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_fmadd_pd(v_src0_d_avx , v_src1_d_avx, v_src2_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    print<float>(res_f_avx , 4 , "VFMADDPS");
    
    if(     res_f_avx[0] >= 0.437491-EPSILLON && res_f_avx[0] <= 0.437491+EPSILLON &&
            res_f_avx[1] >= -1.56545-EPSILLON && res_f_avx[1] <= -1.56545+EPSILLON &&
            res_f_avx[2] >= 0.140077-EPSILLON && res_f_avx[2] <= 0.140077+EPSILLON &&
            res_f_avx[3] >= -4.31585-EPSILLON && res_f_avx[3] <= -4.31585+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_avx , 2 , "VFMADDPD");
     if(     res_d_avx[0] >= 1.84641-EPSILLON && res_d_avx[0] <= 1.84641+EPSILLON &&
            res_d_avx[1] >= -2.30632-EPSILLON && res_d_avx[1] <= -2.30632+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;

    v_res_f_avx = _mm256_fmsub_ps(v_src0_f_avx , v_src1_f_avx, v_src2_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_fmsub_pd(v_src0_d_avx , v_src1_d_avx, v_src2_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VFMSUBPS");
    if(     res_f_avx[0] >= -0.448736-EPSILLON && res_f_avx[0] <= -0.448736+EPSILLON &&
            res_f_avx[1] >= 0.0713985-EPSILLON && res_f_avx[1] <= 0.0713985+EPSILLON &&
            res_f_avx[2] >= -0.216565-EPSILLON && res_f_avx[2] <= -0.216565+EPSILLON &&
            res_f_avx[3] >= -1.43711-EPSILLON && res_f_avx[3] <= -1.43711+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_avx , 2 , "VFMSUBPD");
     if(    res_d_avx[0] >= -1.83223-EPSILLON && res_d_avx[0] <= -1.83223+EPSILLON &&
            res_d_avx[1] >= 2.46663-EPSILLON && res_d_avx[1] <= 2.46663	+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    std::cout << std::endl;

    v_res_f_avx = _mm256_fnmadd_ps(v_src0_f_avx , v_src1_f_avx, v_src2_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_fnmadd_pd(v_src0_d_avx , v_src1_d_avx, v_src2_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);

    print<float>(res_f_avx , 4 , "VFNMADDPS");
    if(     res_f_avx[0] >= 0.448736-EPSILLON && res_f_avx[0] <= 0.448736+EPSILLON &&
            res_f_avx[1] >= -0.0713985-EPSILLON && res_f_avx[1] <= -0.0713985+EPSILLON &&
            res_f_avx[2] >= 0.216565-EPSILLON && res_f_avx[2] <= 0.216565+EPSILLON &&
            res_f_avx[3] >= 1.43711	-EPSILLON && res_f_avx[3] <= 1.43711+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_avx , 2 , "VFNMADDPD");
    if(     res_d_avx[0] >= 1.83223-EPSILLON && res_d_avx[0] <= 1.83223+EPSILLON &&
            res_d_avx[1] >= -2.46663-EPSILLON && res_d_avx[1] <= -2.46663+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    std::cout << std::endl;

    v_res_f_avx = _mm256_fnmsub_ps(v_src0_f_avx , v_src1_f_avx, v_src2_f_avx); _mm256_store_ps(res_f_avx , v_res_f_avx);
    v_res_d_avx = _mm256_fnmsub_pd(v_src0_d_avx , v_src1_d_avx, v_src2_d_avx); _mm256_store_pd(res_d_avx , v_res_d_avx);
    
    print<float>(res_f_avx , 4 , "VFNMSUBPS");
    if(     res_f_avx[0] >= -0.437491-EPSILLON && res_f_avx[0] <= -0.437491+EPSILLON &&
            res_f_avx[1] >= 1.56545-EPSILLON && res_f_avx[1] <= 1.56545+EPSILLON &&
            res_f_avx[2] >= -0.140077-EPSILLON && res_f_avx[2] <= -0.140077+EPSILLON &&
            res_f_avx[3] >= 4.31585-EPSILLON && res_f_avx[3] <= 4.31585+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;

    print<double>(res_d_avx , 2 , "VFNMSUBPD");
    if(     res_d_avx[0] >= -1.84641-EPSILLON && res_d_avx[0] <= -1.84641+EPSILLON &&
            res_d_avx[1] >= 2.30632-EPSILLON && res_d_avx[1] <= 2.30632+EPSILLON) 
        std::cout << "\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\033[1;31m[ KO ]\033[0m" << std::endl;
    std::cout << std::endl;




    return 0;
}
