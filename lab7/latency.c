#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv) {
    int ierror, rank, size, n, i = 0;
    ierror = MPI_Init(&argc, &argv);
    int tag = 0;
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &size);

    double total_time, start_time, end_time;
    MPI_Status status;

    // Use a single byte for the message (MPI_CHAR)
    char message = 'a';  // Placeholder value (single byte)
    char rec_buf;

    if (rank == 0) {
        printf("Enter the number of iterations (n) for latency test: ");
        scanf("%d", &n);
    }

    // Broadcast the number of iterations to all processes
    ierror = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Start the timer for measuring latency
    start_time = MPI_Wtime();

    // Ping-pong loop for measuring latency with minimal data transfer (1 byte)
    for (i = 0; i < n; i++) {
        if (rank == 0) {
            // P0 sends a minimal message to P1 and waits for a reply
            ierror = MPI_Send(&message, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
            ierror = MPI_Recv(&rec_buf, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
        }
        else if (rank == 1) {
            // P1 receives the message from P0 and sends a reply back
            ierror = MPI_Recv(&rec_buf, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
            ierror = MPI_Send(&message, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        }
    }

    // Stop the timer after the loop
    end_time = MPI_Wtime();
    total_time = end_time - start_time;

    // Rank 0 prints the final latency and the average time per exchange
    if (rank == 0) {
        printf("Total latency time for %d exchanges: %lf seconds\n", n, total_time);
        printf("Average latency per exchange: %lf seconds\n", total_time / n);
    }

    // Finalize MPI
    ierror = MPI_Finalize();
    return ierror;
}
