#include <cstdio>
#include <vector>
#include <algorithm>
#include "mpi.h"

int main(int argc, char **argv) {
    int size, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int d = 0;
    std::vector<int> offers = {343, 642, 121};
    std::vector<int> result;

    printf("\nProcess %d of %d is running...\n", rank, size);
    if (rank != 0) {
        d = offers.at(rank - 1);
        printf("P%d: Input your offer: %d\n", rank, d);
        printf("P%d: Sending to P%d = %d\n", rank, 0, d);
        MPI_Send(&d, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; ++i) {
            MPI_Recv(&d, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
            printf("P%d: Receiving from P%d = %d\n", rank, i, d);
            result.push_back(d);
        }

        printf("\n----------\n");
        for (int i = 0; i < result.size(); ++i) {
            printf("P%d: Offer from P%d = %d\n", rank, i, result.at(i));
        }
        printf("----------\n");

        printf("Max offer = %d\n", *max_element(result.begin(), result.end()));
    }
    MPI_Finalize();
    return 0;
}
