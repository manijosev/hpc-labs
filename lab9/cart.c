#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, ierror, size, tag = 0;
    ierror = MPI_Init(&argc, &argv);

    int n = 8, m = 6, p = 2, q = 3;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[2] = {p, q};
    int periods[2] = {0, 0};  // non-cyclic
    int coords[2];
    MPI_Comm cart_comm;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm); // no rerank
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    int local_rows = n / p;
    int local_cols = m / q;

    int **local_block = malloc(local_rows * sizeof(int *));
    for (int i = 0; i < local_rows; i++) {
        local_block[i] = malloc(local_cols * sizeof(int));
        for (int j = 0; j < local_cols; j++) {
            local_block[i][j] = rank;
        }
    }

    printf("Process (%d,%d) [Rank %d] has block:\n", coords[0], coords[1], rank);
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < local_cols; j++) {
            printf("%3d ", local_block[i][j]);
        }
        printf("\n");
    }

    int up, down, left, right;

    MPI_Cart_shift(cart_comm, 0, 1, &up, &down);   // vertical
    MPI_Cart_shift(cart_comm, 1, 1, &left, &right); // horizontal

    printf("Rank %d (coords %d,%d): up=%d, down=%d, left=%d, right=%d\n",
           rank, coords[0], coords[1], up, down, left, right);

    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}
