
#include <stdlib.h>
#include <stdio.h>
#include <cblas.h>

#define MATRIX_SIZE 200

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
    
    A = (double*)_aligned_malloc((MATRIX_SIZE*MATRIX_SIZE)*sizeof(double), 32);
    B = (double*)_aligned_malloc((MATRIX_SIZE*MATRIX_SIZE)*sizeof(double), 32);
    AB = (double*)_aligned_malloc((MATRIX_SIZE*MATRIX_SIZE)*sizeof(double), 32);

    for(int i = 0 ; i < (MATRIX_SIZE*MATRIX_SIZE) ; i++) {
        A[i] = 1 / ((double) (i+(i%MATRIX_SIZE)+1));
        B[i] = 2 / ((double) (i+(i%MATRIX_SIZE)+1));
        AB[i] = 0;
    }
    printf("%.100f\n",AB[0]);
     printf("%.100f\n",AB[1]);
      printf("%.100f\n",AB[2]);
       printf("%.100f\n",AB[3]);

        printf("%.100f\n",AB[150]);
         printf("%.100f\n",AB[45]);

    matrix_multiplcation(A , MATRIX_SIZE , MATRIX_SIZE , B , MATRIX_SIZE , MATRIX_SIZE , AB , false , false , 0);

/*
    printf("\t***** A *****\n");
    for(int i = 0 ; i < MATRIX_SIZE*MATRIX_SIZE ; i++) {
        printf("%.4f ",A[i]);
        if(i && !((i+1) %MATRIX_SIZE)) printf("\n");
    }
    
    printf("\n\n");
    printf("\t***** B *****\n");
    for(int i = 0 ; i < MATRIX_SIZE*MATRIX_SIZE ; i++) {
        printf("%.4f ",B[i]);
        if(i && !((i+1)%MATRIX_SIZE)) printf("\n");
    }

    printf("\n\n");
*/
/*
    printf("\t***** A*B *****\n");
    for(int i = 0 ; i < MATRIX_SIZE*MATRIX_SIZE ; i++) {
        printf("%.4f ",AB[i]);
        if(i && !((i+1)%MATRIX_SIZE)) printf("\n");
    }
*/

    printf("%.100lf\n",AB[0]);
     printf("%.100lf\n",AB[1]);
      printf("%.100lf\n",AB[2]);
       printf("%.100lf\n",AB[3]);

        printf("%.100lf\n",AB[150]);
         printf("%.100lf\n",AB[45]);



    _aligned_free(A);
    _aligned_free(B);
    _aligned_free(AB);

    printf("\n");
	system("pause");
}