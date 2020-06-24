
#include <stdlib.h>
#include <stdio.h>
#include "cblas.h"

#define MATRIX_SIZE 200
#define EPSILLON 0.01

void matrix_multiplcation(double *A, int A_width, int A_height,
         double *B, int B_width, int B_height,
         double *AB, bool tA, bool tB, double beta)
{
    A_height = tA ? A_width  : A_height;
    A_width  = tA ? A_height : A_width;

    B_height = tB ? B_width  : B_height;
    B_width  = tB ? B_height : B_width;

    int m = A_height;
    int n = B_width;
    int k = A_width;


    int lda = tA ? m : k;
    int ldb = tB ? k : n;

    #define TRANSPOSE(X) ((X) ? CblasTrans : CblasNoTrans)

    cblas_dgemm(CblasRowMajor, TRANSPOSE(tA), TRANSPOSE(tB), m, n, k, 1.0, A, lda,B, ldb,beta, AB, n);
                
    #undef TRANSPOSE
}

int main(int argc , char *argv[]) {

    printf("MULT %u*%u\n",MATRIX_SIZE,MATRIX_SIZE);

    double *A, *B , *AB;
    
    if(posix_memalign((void**)&A , 32 , (MATRIX_SIZE*MATRIX_SIZE)*sizeof(double))){
        fprintf(stderr, "posix_memalign error\n");
        return 1;
    }
    if(posix_memalign((void**)&B , 32 , (MATRIX_SIZE*MATRIX_SIZE)*sizeof(double))){
        fprintf(stderr, "posix_memalign error\n");
        free(A);
        return 1;
    }
    if(posix_memalign((void**)&AB, 32 , (MATRIX_SIZE*MATRIX_SIZE)*sizeof(double))){
        fprintf(stderr, "posix_memalign error\n");
        free(A);
        free(B);
        return 1;
    }

    for(int i = 0 ; i < (MATRIX_SIZE*MATRIX_SIZE) ; i++) {
        A[i] = 1 / ((double) (i+(i%MATRIX_SIZE)+1));
        B[i] = 2 / ((double) (i+(i%MATRIX_SIZE)+1));
        AB[i] = 0;
    }

    matrix_multiplcation(A , MATRIX_SIZE , MATRIX_SIZE , B , MATRIX_SIZE , MATRIX_SIZE , AB , false , false , 0);

    printf("%e",AB[0]); 
    if(AB[0] >= 2.006091-EPSILLON && AB[0] <= 2.006091+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

    printf("%e",AB[1]);
    if(AB[1] >= 0.672717-EPSILLON && AB[1] <= 0.672717+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

    printf("%e",AB[2]);
    if(AB[2] >= 0.406010-EPSILLON && AB[2] <= 0.406010+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

    printf("%e",AB[3]);
    if(AB[3] >= 0.291685-EPSILLON && AB[3] <= 0.291685+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

    printf("%e",AB[45]);
    if(AB[45] >= 0.026723-EPSILLON && AB[45] <= 0.026723+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

    printf("%e",AB[150]);
    if(AB[150] >= 0.009949-EPSILLON && AB[150] <= 0.009949+EPSILLON) printf("\t\033[1;32m[ OK ]\033[0m\n"); else printf("\t\033[1;31m[ KO ]\033[0m\n");

   



    free(A);
    free(B);
    free(AB);
    printf("\n");
}