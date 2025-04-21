#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int ierror, rank, size, n, i = 0;
    ierror = MPI_Init(&argc, &argv);
    int tag = 0;
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &size);

    double total_time, start_time, end_time;
    MPI_Status status;
    int message = 10;
    int rec_buf;

    if (rank == 0) {
        printf("Enter the number of iterations (n): ");
        scanf("%d", &n);
    }

    // Broadcast the number of iterations to all processes
    ierror = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Start the timer before the loop
    start_time = MPI_Wtime();

    // Ping-pong loop
    for (i = 0; i < n; i++) {
        if (rank == 0) {
            // P0 increments the message and sends it to P1
            ++message;
            ierror = MPI_Send(&message, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

            // P0 receives the message back from P1
            ierror = MPI_Recv(&message, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
        }
        else if (rank == 1) {
            // P1 receives the message from P0
            ierror = MPI_Recv(&message, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            // P1 increments the message and sends it back to P0
            ++message;
            ierror = MPI_Send(&message, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }
    }

    // Stop the timer after the loop
    end_time = MPI_Wtime();
    total_time = end_time - start_time;

    // P0 prints the final message and the average time per exchange
    if (rank == 0) {
        printf("Final message value after %d exchanges: %d\n", n, message);
        printf("Average time per exchange: %lf seconds\n", total_time / n);
    }

    // Finalize MPI
    ierror = MPI_Finalize();
    return ierror;
}
