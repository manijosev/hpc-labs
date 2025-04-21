#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments are provided
    if (argc != 3) {
        printf("Usage: %s <number_of_rows> <number_of_columns>\n", argv[0]);
        return 1; 
    }

    // Get matrix dimensions from command-line arguments
    int m = atoi(argv[1]); // Rows
    int n = atoi(argv[2]); // Columns

    // Validate the matrix dimensions
    if (m <= 0 || n <= 0) {
        printf("Matrix dimensions must be positive integers.\n");
        return 1; 
    }

    // Dynamically allocate memory for the matrix
    double **matrix = (double **)malloc(m * sizeof(double *));
    if (matrix == NULL) {
        printf("Memory allocation failed for matrix rows.\n");
        return 1; 
    }
    for (int i = 0; i < m; i++) {
        matrix[i] = (double *)malloc(n * sizeof(double));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed for matrix columns.\n");
            return 1;
        }
    }

    // Initialize random number generator
    srand(time(NULL));
    // Fill the matrix with random values 
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // Generate a random value between -10 and 10
            matrix[i][j] = (rand() % 21) - 10; 
        }
    }

    // Start timer for the Frobenius norm calculation
    double start_time = omp_get_wtime();
    // Calculate the Frobenius norm
    double norm = 0.0;
    // Parallelize the norm calculation
    #pragma omp parallel for reduction(+:norm) 
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            norm += matrix[i][j] * matrix[i][j]; 
        }
    }
    norm = sqrt(norm); 
    // End timer for the Frobenius norm calculation
    double end_time = omp_get_wtime();

    printf("The Frobenius norm of the matrix is: %.2lf\n", norm);
    printf("Time taken for Frobenius norm calculation: %.6f seconds\n", end_time - start_time);

    // Free the dynamically allocated memory
    for (int i = 0; i < m; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

