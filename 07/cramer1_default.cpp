#include <iostream>
#include <vector>
#include <cmath>
#include "omp.h"

using namespace std;

#define N 4;

// найти определителя
int determinant(vector<vector<int>> m) {
    return m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[2][1] * m[1][2] - m[0][1] * m[1][0] * m[2][2] +
           m[0][1] * m[2][0] * m[1][2] + m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[2][0] * m[1][1];
}

// найти допольнительного определителя
int determinant(vector<vector<int>> matrix, int n) {
    int temp;
    for (auto & row : matrix) {
        for (int j = 0; j < row.size(); ++j) {
            temp = row[n];
            row[n] = row[row.size() - 1];
            row[row.size() - 1] = temp;
        }
    }
    return determinant(matrix);
}

int main() {
    // объявление матрицы 4x4
    vector<vector<int>> matrix = {
            {1, 3, 5, 7, 12},
            {3, 5, 7, 1, 0},
            {5, 7, 1, 3, 4},
            {7, 1, 3, 5, 16}
    };

    vector<vector<int>> D1 = {
            {matrix[1][1], matrix[1][2], matrix[1][3]},
            {matrix[2][1], matrix[2][2], matrix[2][3]},
            {matrix[3][1], matrix[3][2], matrix[3][3]}
    };

    //int a1 = determinant(matrix, 0, 0);

    vector<vector<int>> D2 = {
            {matrix[1][0], matrix[1][2], matrix[1][3]},
            {matrix[2][0], matrix[2][2], matrix[2][3]},
            {matrix[3][0], matrix[3][2], matrix[3][3]}
    };

    vector<vector<int>> D3 = {
            {matrix[1][0], matrix[1][1], matrix[1][3]},
            {matrix[2][0], matrix[2][1], matrix[2][3]},
            {matrix[3][0], matrix[3][1], matrix[3][3]}
    };

    vector<vector<int>> D4 = {
            {matrix[1][0], matrix[1][1], matrix[1][2]},
            {matrix[2][0], matrix[2][1], matrix[2][2]},
            {matrix[3][0], matrix[3][1], matrix[3][2]}
    };

    // Шаг 1. Находим главного определителя
    auto d = matrix[0][0] * determinant(D1) - matrix[0][1] * determinant(D2) + matrix[0][2] * determinant(D3) -
             matrix[0][3] * determinant(D4);

    if (d == 0) {
        cout << "Определитель равень нулю.\n";
        return 0;
    }

    // Шаг 2. Находим вспомогательных определителей
    int d1 = determinant(matrix, 0);
    int d2 = determinant(matrix, 1);
    int d3 = determinant(matrix, 2);
    int d4 = determinant(matrix, 3);

    // Шаг 3. Находим корни (можно распараллелить)
    double x1 = (d1 != 0) ? (double) d1 / d : 0;
    double x2 = (d2 != 0) ? (double) d2 / d : 0;
    double x3 = (d3 != 0) ? (double) d3 / d : 0;
    double x4 = (d4 != 0) ? (double) d4 / d : 0;

    printf("x1 = %f\nx2 = %f\nx3 = %f\nx4 = %f\n", x1, x2, x3, x4);

    printf("-----");

    return 0;
}
