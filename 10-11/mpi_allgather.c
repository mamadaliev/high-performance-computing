#include "stdio.h"
#include "mpi.h"

int main(int argc, char *argv[]) {
    int size, rank, k;
    double start, end, min = 100000000000.00, max = 0;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int buffer[size];
    int j = 2 * rank;

    start = MPI_Wtime();

    MPI_Allgather(&j, 1, MPI_INT, buffer, 1, MPI_INT, MPI_COMM_WORLD);

    end = MPI_Wtime();

    printf("Process %d: ", rank);
    for (k = 0; k < size; k++)
        printf(" %d", buffer[k]);
    printf("\n");

    MPI_Finalize();

    if (min > start) {
        min = start;
    }

    if (max < end) {
        max = end;
    }

    if (rank == 0) {
        printf("\nTotal time: %.8f\n", max - min);
    }

    return 0;
}
