#include <iostream>

#define EPSILLON 0.01


int main(int argc , char *argv[]) {

    double a[] __attribute__ ((aligned(32))) = {1.15, 7.32, 4.63, 6.24};
    double b[] __attribute__ ((aligned(32))) = {2.03, 8.32, 1.06, 9.32};
    double c[] __attribute__ ((aligned(32))) = {8.31, 11.1, 5.26, 8.94};

    float aa[] __attribute__ ((aligned(32))) = {1.15, 7.32, 4.63, 6.24, 2.03, 8.32, 1.06, 9.32};
    float bb[] __attribute__ ((aligned(32))) = {2.03, 8.32, 1.06, 9.32, 1.15, 7.32, 4.63, 6.24,};
    float cc[] __attribute__ ((aligned(32))) = {8.31, 11.1, 5.26, 8.94, 8.32, 1.06, 9.32, 1.15};

    printf("\t\t*********** SCALAR DOUBLE ***********\n");

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd231sd %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n"
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 231 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    if(     c[0] >= 18.019300-EPSILLON && c[0] <= 18.019300+EPSILLON &&
            c[1] >= 7.320000-EPSILLON && c[1] <= 7.320000+EPSILLON &&
            c[2] >= 0.0-EPSILLON && c[2] <= 0.0+EPSILLON &&
            c[3] >= 0.0-EPSILLON && c[3] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd213sd %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 213 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    if(     c[0] >= 20.353800-EPSILLON && c[0] <= 20.353800+EPSILLON &&
            c[1] >= 7.320000-EPSILLON && c[1] <= 7.320000+EPSILLON &&
            c[2] >= 0.0-EPSILLON && c[2] <= 0.0+EPSILLON &&
            c[3] >= 0.0-EPSILLON && c[3] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd132sd %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 132 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    if(     c[0] >= 25.436870-EPSILLON && c[0] <= 25.436870+EPSILLON &&
            c[1] >= 7.320000-EPSILLON && c[1] <= 7.320000+EPSILLON &&
            c[2] >= 0.0-EPSILLON && c[2] <= 0.0+EPSILLON &&
            c[3] >= 0.0-EPSILLON && c[3] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;


    printf("\t\t*********** SCALAR FLOAT ***********\n");

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd231ss %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
    if(     cc[0] >= 18.019300-EPSILLON && cc[0] <= 18.019300+EPSILLON &&
            cc[1] >= 7.320000-EPSILLON && cc[1] <= 7.320000+EPSILLON &&
            cc[2] >= 4.630000-EPSILLON && cc[2] <= 4.630000+EPSILLON &&
            cc[3] >= 6.240000-EPSILLON && cc[3] <= 6.240000+EPSILLON && 
            cc[4] >= 0.0-EPSILLON && cc[4] <= 0.0+EPSILLON &&
            cc[5] >= 0.0-EPSILLON && cc[5] <= 0.0+EPSILLON &&
            cc[6] >= 0.0-EPSILLON && cc[6] <= 0.0+EPSILLON &&
            cc[7] >= 0.0-EPSILLON && cc[7] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;


    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd213ss %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
     if(    cc[0] >= 20.353800-EPSILLON && cc[0] <= 20.353800+EPSILLON &&
            cc[1] >= 7.320000-EPSILLON && cc[1] <= 7.320000+EPSILLON &&
            cc[2] >= 4.630000-EPSILLON && cc[2] <= 4.630000+EPSILLON &&
            cc[3] >= 6.240000-EPSILLON && cc[3] <= 6.240000+EPSILLON && 
            cc[4] >= 0.0-EPSILLON && cc[4] <= 0.0+EPSILLON &&
            cc[5] >= 0.0-EPSILLON && cc[5] <= 0.0+EPSILLON &&
            cc[6] >= 0.0-EPSILLON && cc[6] <= 0.0+EPSILLON &&
            cc[7] >= 0.0-EPSILLON && cc[7] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"                
                    "\tvfmadd132ss %%xmm0, %%xmm1, %%xmm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
    if(     cc[0] >= 25.436869-EPSILLON && cc[0] <= 25.436869+EPSILLON &&
            cc[1] >= 7.320000-EPSILLON && cc[1] <= 7.320000+EPSILLON &&
            cc[2] >= 4.630000-EPSILLON && cc[2] <= 4.630000+EPSILLON &&
            cc[3] >= 6.240000-EPSILLON && cc[3] <= 6.240000+EPSILLON && 
            cc[4] >= 0.0-EPSILLON && cc[4] <= 0.0+EPSILLON &&
            cc[5] >= 0.0-EPSILLON && cc[5] <= 0.0+EPSILLON &&
            cc[6] >= 0.0-EPSILLON && cc[6] <= 0.0+EPSILLON &&
            cc[7] >= 0.0-EPSILLON && cc[7] <= 0.0+EPSILLON) 
        std::cout << "\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;


    printf("\t\t*********** PACKED DOUBLE ***********\n");

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd231pd %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 231 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    if(     c[0] >= 52.786846-EPSILLON && c[0] <= 52.786846+EPSILLON &&
            c[1] >= 68.222400-EPSILLON && c[1] <= 68.222400+EPSILLON &&
            c[2] >= 4.630000-EPSILLON && c[2] <= 4.630000+EPSILLON &&
            c[3] >= 6.240000-EPSILLON && c[3] <= 6.240000+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd213pd %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 213 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    if(     c[0] >= 55.121346-EPSILLON && c[0] <= 55.121346+EPSILLON &&
            c[1] >= 129.124800-EPSILLON && c[1] <= 129.124800+EPSILLON &&
            c[2] >= 9.537800-EPSILLON && c[2] <= 9.537800+EPSILLON &&
            c[3] >= 64.396800-EPSILLON && c[3] <= 64.396800+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd132pd %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(c)
                    : "m"(a), "m"(b), "m"(c));
    
    printf("FMA 132 : %lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
     if(    c[0] >= 65.419548-EPSILLON && c[0] <= 65.419548+EPSILLON &&
            c[1] >= 953.513536-EPSILLON && c[1] <= 953.513536+EPSILLON &&
            c[2] >= 45.220014-EPSILLON && c[2] <= 45.220014+EPSILLON &&
            c[3] >= 411.156032-EPSILLON && c[3] <= 411.156032+EPSILLON) 
        std::cout << "\t\t\t\t\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    printf("\t\t*********** PACKED FLOAT ***********\n");

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd231ps %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
    if(     cc[0] >= 52.786842-EPSILLON && cc[0] <= 52.786842+EPSILLON &&
            cc[1] >= 68.222397-EPSILLON && cc[1] <= 68.222397+EPSILLON &&
            cc[2] >= 9.537800-EPSILLON && cc[2] <= 9.537800+EPSILLON &&
            cc[3] >= 64.396797-EPSILLON && cc[3] <= 64.396797+EPSILLON && 
            cc[4] >= 2.030000-EPSILLON && cc[4] <= 2.030000+EPSILLON &&
            cc[5] >= 8.320000-EPSILLON && cc[5] <= 8.320000+EPSILLON &&
            cc[6] >= 1.060000-EPSILLON && cc[6] <= 1.060000+EPSILLON &&
            cc[7] >= 9.320000-EPSILLON && cc[7] <= 9.320000+EPSILLON) 
        std::cout << "\t\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"
                    "\tvfmadd213ps %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
    if(     cc[0] >= 55.121342-EPSILLON && cc[0] <= 55.121342+EPSILLON &&
            cc[1] >= 129.124802-EPSILLON && cc[1] <= 129.124802+EPSILLON &&
            cc[2] >= 14.445600-EPSILLON && cc[2] <= 14.445600+EPSILLON &&
            cc[3] >= 122.553596-EPSILLON && cc[3] <= 122.553596+EPSILLON && 
            cc[4] >= 4.364500-EPSILLON && cc[4] <= 4.364500+EPSILLON &&
            cc[5] >= 69.222397-EPSILLON && cc[5] <= 69.222397+EPSILLON &&
            cc[6] >= 5.967800-EPSILLON && cc[6] <= 5.967800 +EPSILLON &&
            cc[7] >= 67.476799-EPSILLON && cc[7] <= 67.476799+EPSILLON) 
        std::cout << "\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;

    asm volatile(   "\tvmovaps %%ymm0, %1\n"
                    "\tvmovaps %%ymm1, %2\n"
                    "\tvmovaps %%ymm2, %3\n"                
                    "\tvfmadd132ps %%ymm0, %%ymm1, %%ymm2\n"
                    "\tvmovaps %0, %%ymm0\n" 
                    : "=m"(cc)
                    : "m"(aa), "m"(bb), "m"(cc));
    
    printf("FMA 231 : %lf %lf %lf %lf %lf %lf %lf %lf", cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7]);
    if(     cc[0] >= 65.419540-EPSILLON && cc[0] <= 65.419540+EPSILLON &&
            cc[1] >= 953.513550-EPSILLON && cc[1] <= 953.513550+EPSILLON &&
            cc[2] >= 67.943130-EPSILLON && cc[2] <= 67.943130+EPSILLON &&
            cc[3] >= 774.054443-EPSILLON && cc[3] <= 774.054443+EPSILLON && 
            cc[4] >= 10.009935-EPSILLON && cc[4] <= 10.009935+EPSILLON &&
            cc[5] >= 583.250305-EPSILLON && cc[5] <= 583.250305+EPSILLON &&
            cc[6] >= 10.955868-EPSILLON && cc[6] <= 10.955868 +EPSILLON &&
            cc[7] >= 635.123718-EPSILLON && cc[7] <= 635.123718+EPSILLON) 
        std::cout << "\t\t\t\033[1;32m[ OK ]\033[0m" << std::endl; else std::cout  << "\t\t\t\033[1;31m[ KO ]\033[0m" << std::endl;


    
    
    
    return 0;
}