#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <openmpi/mpi.h>

using namespace std;

void init_matrix(int n1, int n2, ifstream &in, vector<vector<int>> &A);

void out_matrix(int n1, int n2, const vector<vector<int>> &A);

// структуры для обмена между процессами
struct Item {
    int value1; // значение 1
    int value2; // значение 2
    int x;      // позиция x
    int y;      // позиция y
};

struct Result {
    int value; // значение 1
    int x;     // позиция x
    int y;     // позиция y
};

int main(int argc, char **argv) {
    int n1 = 3, n2 = 4, n3 = 3;
    int c_x_size = 3, c_y_size = 3;
    vector<vector<int>> C(c_x_size, vector<int>(c_y_size));

    int size, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("\nProcess %d of %d is running...\n", rank, size);

    // Переменные для обмена между процессами
    Item item {};
    Result result {};

    if (rank == 0) {
        std::ifstream in("in.txt");
        vector<vector<int>> A(n1, vector<int>(n2));
        vector<vector<int>> B(n2, vector<int>(n3));

        init_matrix(n1, n2, in, A);
        init_matrix(n2, n3, in, B);

        in.close();

        printf("n1 = %d\nn2 = %d\nn3 = %d\n", n1, n2, n3);
        printf("\n");

        printf("Matrix A:\n");
        out_matrix(n1, n2, A);
        printf("\n");
        printf("Matrix B:\n");
        out_matrix(n2, n3, B);

        printf("\n");
        printf("A * B = C\n");

        int process_index = 1;

        for (int i = 0; i < n1; ++i) {
            for (int j = 0; j < n3; ++j) {
                for (int k = 0; k < n2; ++k) {
                    // C[i][j] += A[i][k] * B[k][j];
                    item.value1 = A[i][k];
                    item.value2 = B[k][j];
                    item.x = i;
                    item.y = j;

                    if (process_index == size) {
                        process_index = 1;
                    }
                    MPI_Send(&item, sizeof(item), MPI_CHAR, process_index, 5, MPI_COMM_WORLD);
                    MPI_Recv(&result, sizeof(result), MPI_CHAR, process_index, 5, MPI_COMM_WORLD, &status);
                    C[result.x][result.y] = result.value;
                    process_index++;
                }
            }
        }
    } else {
        MPI_Recv(&item, sizeof(item), MPI_CHAR, 0, 5, MPI_COMM_WORLD, &status);
        int c = item.value1 * item.value2;
        result.value = c;
        result.x = item.x;
        result.y = item.y;
        MPI_Send(&result, sizeof(item), MPI_CHAR, 0, 5, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("\n");
        printf("Matrix C:\n");
        out_matrix(c_x_size, c_x_size, C);

        std::ofstream out("out.txt");

        for (int i = 0; i < c_x_size; ++i) {
            for (int j = 0; j < c_y_size; ++j) {
                out << C[i][j];
                if (j + 1 < c_y_size) out << " ";
            }
            out << "\n";
        }
    }
    return 0;
}

void out_matrix(int n1, int n2, const vector<vector<int>> &A) {
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

void init_matrix(int n1, int n2, ifstream &in, vector<vector<int>> &A) {
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            in >> A[i][j];
        }
    }
}
