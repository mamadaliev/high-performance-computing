#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <openmpi/mpi.h>

using namespace std;

void init_matrix(int n1, int n2, ifstream &in, vector<vector<int>> &A);

void out_matrix(int n1, int n2, const vector<vector<int>> &A);

// структуры для обмена между процессами
typedef struct Item {
    int value1; // значение 1
    int value2; // значение 2
    int x;      // позиция x
    int y;      // позиция y
} matrix_item;

typedef struct Result {
    int value; // значение 1
    int x;     // позиция x
    int y;     // позиция y
} calc_result;

int main(int argc, char **argv) {
    int n1 = 3, n2 = 4, n3 = 3;
    int c_x_size = 3, c_y_size = 3;
    vector<vector<int>> C(c_x_size, vector<int>(c_y_size));

    int size, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Опеределение новых MPI типов
    const int item_type_fields_size = 4;
    int item_type_block_size[4] = {50, 50, 50, 50};
    MPI_Datatype item_subtypes[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_item_type;
    MPI_Aint item_type_offsets[4];
    item_type_offsets[0] = offsetof(Item, value1);
    item_type_offsets[1] = offsetof(Item, value2);
    item_type_offsets[2] = offsetof(Item, x);
    item_type_offsets[3] = offsetof(Item, y);

    const int result_type_fields_size = 3;
    int result_type_block_size[4] = {50, 50, 50};
    MPI_Datatype result_subtypes[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_result_type;
    MPI_Aint result_type_offsets[3];
    result_type_offsets[0] = offsetof(Result, value);
    result_type_offsets[1] = offsetof(Result, x);
    result_type_offsets[2] = offsetof(Result, y);

    MPI_Type_create_struct(item_type_fields_size, item_type_block_size, item_type_offsets, item_subtypes, &mpi_item_type);
    MPI_Type_create_struct(result_type_fields_size, result_type_block_size, result_type_offsets, result_subtypes, &mpi_result_type);
    MPI_Type_commit(&mpi_item_type);
    MPI_Type_commit(&mpi_result_type);

    printf("\nProcess %d of %d is running...\n", rank, size);

    // Счетчик вызовов
    vector<int> process_call_count(size);

    // Переменные для обмена между процессами
    Item item{};
    Result result{};

    vector<vector<int>> A(n1, vector<int>(n2));
    vector<vector<int>> B(n2, vector<int>(n3));

    if (rank == 0) {
        std::ifstream in("in.txt");

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
    }

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

                if (rank == 0) {
                    MPI_Send(&item, 1, mpi_item_type, process_index, 5, MPI_COMM_WORLD);
                    printf("P%d: Sent item from P%d to P%d.\n", 0, 0, process_index);
                }

                if (rank == process_index) {
                    MPI_Recv(&item, 1, mpi_item_type, 0, 5, MPI_COMM_WORLD, &status);
                    printf("P%d: Received result from P%d to P%d. Items = [%d, %d]\n",
                           process_index, 0, process_index, item.value1, item.value2);
                    int c = item.value1 * item.value2;
                    result.value = c;
                    result.x = item.x;
                    result.y = item.y;
                    MPI_Send(&result, 1, mpi_result_type, 0, 5, MPI_COMM_WORLD);
                    printf("P%d: Sent item from P%d to P%d.\n", process_index, process_index, 0);
                }

                if (rank == 0) {
                    MPI_Recv(&result, 1, mpi_result_type, process_index, 5, MPI_COMM_WORLD, &status);
                    printf("P%d: Received result from P%d to P%d. Result = %d\n", 0, process_index, 0, result.value);
                    C[result.x][result.y] = result.value;
                    process_call_count[process_index]++;
                }
                process_index++;
            }
        }
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
    MPI_Finalize();
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
