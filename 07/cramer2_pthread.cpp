#include <iostream>
#include <vector>
#include "pthread.h"

using namespace std;

#define N 4

struct Params {
    vector<vector<int>> matrix;
    int index;
    double result;
};

double determinant_3x3(vector<vector<int>> m) {
    return m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[2][1] * m[1][2] - m[0][1] * m[1][0] * m[2][2] +
           m[0][1] * m[2][0] * m[1][2] + m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[2][0] * m[1][1];
}

double determinant_4x4(vector<vector<int>> matrix) {
    vector<vector<int>> D1 = {
            {matrix[1][1], matrix[1][2], matrix[1][3]},
            {matrix[2][1], matrix[2][2], matrix[2][3]},
            {matrix[3][1], matrix[3][2], matrix[3][3]}
    };
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
    return matrix[0][0] * determinant_3x3(D1) - matrix[0][1] * determinant_3x3(D2) +
           matrix[0][2] * determinant_3x3(D3) - matrix[0][3] * determinant_3x3(D4);
}

void *determinant(void *params) {
    auto *p = (struct Params *) params;
    int temp;
    for (auto &row: p->matrix) {
        for (int j = 0; j < row.size(); ++j) {
            temp = row[p->index];
            row[p->index] = row[row.size() - 1];
            row[row.size() - 1] = temp;
        }
    }
    p->result = determinant_4x4(p->matrix);
    pthread_exit(nullptr);
}

int main() {
    double main_d, x[N];

    // Объявление матрицы 5x5.
    vector<vector<int>> matrix = {
            {1, 3, 5, 7, 12},
            {3, 5, 7, 1, 0},
            {5, 7, 1, 3, 4},
            {7, 1, 3, 5, 16}
    };

    // Шаг 1. Находим главный опеределитель системы.
    main_d = determinant_4x4(matrix);

    if (main_d == 0) {
        cout << "Главный определитель равень нулю. Система не имеет решений.\n";
        return 0;
    }

    printf("Главный определитель равен %f\n", main_d);

    pthread_t threads[N];
    Params params[N];

    for (int i = 0; i < N; ++i) {
        params[i].index = i;
        params[i].matrix = matrix;
    }

    // Шаг 2. Находим вспомогательных определителей системы в разных потоках.
    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], nullptr, determinant, &params[i]);
    }

    // Ожидаем вычисление всех вспомогательных определителей.
    for (unsigned long thread: threads) {
        pthread_join(thread, nullptr);
    }

    // Шаг 3. Находим корни в главном потоке.
    // Здесь также можно выпольнить вычисления корней в разных потоках как с доп. определителями.
    for (int i = 0; i < N; ++i) {
        x[i] = params[i].result / main_d;
    }

    printf("x1 = %f\nx2 = %f\nx3 = %f\nx4 = %f\n", x[0], x[1], x[2], x[3]);
    return 0;
}
