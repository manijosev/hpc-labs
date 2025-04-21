#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifndef ALLOCD
#define ALLOCD

int **alloc2dInt(size_t n1, size_t n2) {
    if (n1 * n2 == 0) {
        n1 = n2 = 1;
    }

    int **dummy = (int **)calloc(n1, sizeof(int *));
    if (dummy == NULL) {
        printf("Could not allocate memory for 2D int array\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    *dummy = (int *)calloc(n1 * n2, sizeof(int));
    if (*dummy == NULL) {
        printf("Could not allocate flat memory for 2D int array\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (size_t i = 1; i < n1; i++) {
        dummy[i] = (*dummy + n2 * i);
    }

    return dummy;
}
#endif

int main(int argc, char **argv) {
    int rank, ierror, size, tag = 0;
    ierror = MPI_Init(&argc, &argv);

    int n = 8, m = 6, p = 2, q = 3;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != p * q) {
        if (rank == 0)
            printf("Error: size must be %d\n", p * q);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int dims[2] = {p, q};
    int periods[2] = {0, 0}; // non-cyclic grid
    int coords[2];
    MPI_Comm cart_comm;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    int local_rows = n / p;
    int local_cols = m / q;

    // ✅ Use the provided allocation function
    int **local_block = alloc2dInt(local_rows, local_cols);

    // Fill with rank number
    for (int i = 0; i < local_rows; i++) {
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
