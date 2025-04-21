#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int m, n, rank, size;
    float **matrix = NULL;
    float norm = 0.0, local_norm = 0.0;
    float start_time, end_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get the rank of the process and the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check if the correct number of command line arguments are provided
    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <number_of_rows> <number_of_columns>\n", argv[0]);
        }
        MPI_Finalize();
        return 1; 
    }

    // Get matrix dimensions from command-line arguments
    m = atoi(argv[1]); // Rows
    n = atoi(argv[2]); // Columns

    // Validate matrix dimensions
    if (m <= 0 || n <= 0) {
        if (rank == 0) {
            printf("Matrix dimensions must be positive integers.\n");
        }
        MPI_Finalize();
        return 1; 
    }

    // Dynamically allocate memory for the matrix (on all processes)
    matrix = (float **)malloc(m * sizeof(float *));
    if (matrix == NULL) {
        printf("Memory allocation failed for matrix rows.\n");
        MPI_Finalize();
        return 1;
    }
    for (int i = 0; i < m; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed for matrix columns.\n");
            MPI_Finalize();
            return 1;
        }
    }
        
    // Initialize random number generator
    srand(time(NULL));
    // Fill the matrix with random values (between -10 and 10)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (rand() % 21) - 10;
        }
    }

    // Start timer for the Frobenius norm calculation
    start_time = MPI_Wtime();

    // Divide the work: Each process will handle a subset of rows
    // The matrix row size does not need to be a multiple of the number of processes.
    int rows_per_process = m / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? m : start_row + rows_per_process;

    // Compute local Frobenius norm (sum of squares of elements in assigned rows)
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            local_norm += matrix[i][j] * matrix[i][j];
        }
    }
    // Use MPI_Reduce to sum up the local norms from all processes
    MPI_Reduce(&local_norm, &norm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // End the timer for the Frobenius norm calculation
    end_time = MPI_Wtime();

    // Output the result (only on the root process)
    if (rank == 0) {
        norm = sqrt(norm); 
        printf("The Frobenius norm of the matrix is: %.2f\n", norm);
        printf("Time taken for Frobenius norm calculation: %.6f seconds\n", end_time - start_time);
    }
        
    // Free the matrix memory
    for (int i = 0; i < m; i++) {
        free(matrix[i]);
    }
    free(matrix);
    
    // Finalize MPI
    MPI_Finalize();

    return 0;
}

