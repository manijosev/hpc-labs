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

    // Define the size of the message (e.g., 1MB)
    int message_size = 1024 * 1024;  // 1 MB message
    char *message = (char *)malloc(message_size * sizeof(char));
    char *rec_buf = (char *)malloc(message_size * sizeof(char));  // Receive buffer

    // Initialize the message with some data
    for (int j = 0; j < message_size; j++) {
        message[j] = 'a';  // Fill the buffer with 'a'
    }

    if (rank == 0) {
        printf("Enter the number of iterations (n) for bandwidth test: ");
        scanf("%d", &n);
    }

    // Broadcast the number of iterations to all processes
    ierror = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Start the timer for measuring bandwidth
    start_time = MPI_Wtime();

    // Ping-pong loop for measuring bandwidth with a large message transfer
    for (i = 0; i < n; i++) {
        if (rank == 0) {
            // P0 sends a large message to P1
            ierror = MPI_Send(message, message_size, MPI_CHAR, 1, tag, MPI_COMM_WORLD);

            // P0 receives the message back from P1
            ierror = MPI_Recv(rec_buf, message_size, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
        }
        else if (rank == 1) {
            // P1 receives the message from P0
            ierror = MPI_Recv(rec_buf, message_size, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

            // P1 sends the message back to P0
            ierror = MPI_Send(message, message_size, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        }
    }

    // Stop the timer after the loop
    end_time = MPI_Wtime();
    total_time = end_time - start_time;

    // Calculate bandwidth: Size of message * number of iterations / total time
    double bandwidth = (message_size * n) / total_time;  // in bytes per second

    // Rank 0 prints the final bandwidth and the average time per exchange
    if (rank == 0) {
        printf("Total transfer time for %d exchanges: %lf seconds\n", n, total_time);
        printf("Average bandwidth: %lf MB/s\n", bandwidth / (1024 * 1024));  // Convert bytes to MB
    }

    // Finalize MPI and free allocated memory
    ierror = MPI_Finalize();
    free(message);
    free(rec_buf);

    return ierror;
}
