#include <stdio.h>
#include <float.h>
#include <omp.h>
#include<stdlib.h>

int main(){
    int A[4][4],B[4][4],C[4][4];
    unsigned int seed = 45;
    int i,j,k;

    #pragma omp parallel shared(A,B) private(i,j)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for(i=0;i<4;i++){
                    for(j=0;j<4;j++){
                        A[i][j] = rand_r(&seed);
                        B[i][j] = rand_r(&seed);
                    }
                }
            }
        }
        #pragma omp for private(i,j,k)
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                C[i][j] = 0;
                for (k = 0; k < 4; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

    }

    // Write matrix C to file
        FILE *fptr = fopen("matrix_C.txt", "w");
        if (fptr == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fprintf(fptr, "Matrix C = A * B:\n");
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                fprintf(fptr, "%d\t", C[i][j]);
            }
            fprintf(fptr, "\n");
        }

        fclose(fptr);
        printf("Matrix C written to matrix_C.txt\n");


}