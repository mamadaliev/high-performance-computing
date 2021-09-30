#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define N1 3            /* number of rows in matrix A */
#define N2 4            /* number of columns in matrix A */
#define N3 3            /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main(int argc, char *argv[]) {
    int numtasks,              /* number of tasks in partition */
    taskid,                /* A task identifier */
    numworkers,            /* number of worker tasks */
    source,                /* task id of message source */
    dest,                  /* task id of message destination */
    mtype,                 /* message type */
    rows,                  /* rows of matrix A sent to each worker */
    averow, extra, offset, /* used to determine rows sent to each worker */
    i, j, k, rc;           /* misc */
    double A[N1][N2],           /* matrix A to be multiplied */
    B[N2][N3],           /* matrix B to be multiplied */
    C[N1][N3];           /* result matrix C */
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks < 2) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    numworkers = numtasks - 1;


/**************************** master task ************************************/
    if (taskid == MASTER) {
        std::ifstream in("in.txt");

        printf("mpi_mm has started with %d tasks.\n", numtasks);
        printf("Initializing arrays...\n");
        for (i = 0; i < N1; i++)
            for (j = 0; j < N2; j++)
                in >> A[i][j];
        for (i = 0; i < N2; i++)
            for (j = 0; j < N3; j++)
                in >> B[i][j];

        in.close();

        printf("******************************************************\n");
        printf("Matrix A:\n");
        for (i = 0; i < N1; i++) {
            printf("\n");
            for (j = 0; j < N3; j++)
                printf("%6.2f   ", A[i][j]);
        }

        printf("\n\nMatrix B:\n");
        for (i = 0; i < N1; i++) {
            printf("\n");
            for (j = 0; j < N3; j++)
                printf("%6.2f   ", B[i][j]);
        }
        printf("\n******************************************************\n");

        /* Measure start time */
        double start = MPI_Wtime();

        /* Send matrix data to the worker tasks */
        averow = N1 / numworkers;
        extra = N1 % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            printf("Sending %d rows to task %d offset=%d\n", rows, dest, offset);
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], rows * N2, MPI_DOUBLE, dest, mtype,
                     MPI_COMM_WORLD);
            MPI_Send(&B, N2 * N3, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        /* Receive results from worker tasks */
        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows * N3, MPI_DOUBLE, source, mtype,
                     MPI_COMM_WORLD, &status);
            printf("Received results from task %d\n", source);
        }

        /* Print results */
        printf("******************************************************\n");
        printf("Result Matrix:\n");
        std::ofstream out("out.txt");
        for (i = 0; i < N1; i++) {
            for (j = 0; j < N3; j++) {
                printf("%6.2f   ", C[i][j]);
                out << C[i][j];
                if (j + 1 < N3) out << " ";
            }
            printf("\n");
            out << "\n";
        }
        out.close();
        printf("\n******************************************************\n");


        /* Measure finish time */
        double finish = MPI_Wtime();
        printf("Done in %f seconds.\n", finish - start);
    }


/**************************** worker task ************************************/
    if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, rows * N2, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, N2 * N3, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k = 0; k < N3; k++)
            for (i = 0; i < rows; i++) {
                C[i][k] = 0.0;
                for (j = 0; j < N2; j++)
                    C[i][k] = C[i][k] + A[i][j] * B[j][k];
            }
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&C, rows * N3, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
