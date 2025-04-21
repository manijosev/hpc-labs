#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int ierror, rank, size, m = 1000, n, local_sum = 0, total_sum;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Assuming vector A is distributed across 'n' processes, with each process
    // holding a chunk of the vector. Let's say each chunk is of size m/n.

    // Initialize local vector chunk for each process.
    int chunk_size = m / size;
    double *local_A = (double *)malloc(chunk_size * sizeof(double));

    // Initialize the local chunk of the vector (just for demonstration purposes).
    for (int i = 0; i < chunk_size; i++) {
        local_A[i] = rank * chunk_size + i + 1;  // Just an example of initializing
    }

    // Compute local sum
    for (int i = 0; i < chunk_size; i++) {
        local_sum += local_A[i];
    }

    // Use MPI_Reduce to compute the total sum across all processes
    ierror = MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Only process 0 will print the final result
    if (rank == 0) {
        printf("Total sum of the vector: %d\n", total_sum);
    }

    // Free allocated memory
    free(local_A);

    // Finalize MPI
    ierror = MPI_Finalize();
    return ierror;
}
