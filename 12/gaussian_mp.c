// https://ita.sibsutis.ru/sites/csc.sibsutis.ru/files/courses/pvt/%20%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D1%8B_0.pdf

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M 1600
/* Описываем массивы для расширенной матрицы - MA и вектор V - решение
системы. */
double MA[M][M + 1], V[M];
int main(int argc, char **argv)
{
int i, j, p;
double a;
struct timeval tv_start, tv_end;
gettimeofday(&tv_start, NULL);
/* Заполняется матрица системы */
 for (i = 0; i < M; i++)
 for(j = 0; j < M; j++)
 if (i == j)
 MA[i][j] = 2.0;
 else
44
 MA[i][j] = 1.0;
/* Задается решение системы. */
 for (j = 0; j < M; j++)
 V[j] = -(double)(j + 1) / 2.;
/* Вычисляется вектор правой части, который записывается в последний столбец
 расширенной матрицы */
 for(i = 0; i < M; i++) {
 MA[i][M] = 0.0;
for(j = 0; j < M; j++)
MA[i][M] += MA[i][j] * V[j];
}

/* Прямой ход */
for(p = 0; p < M; p++) {
a = MA[p][p];
for(i = p; i <= M; i++)
MA[p][i] = MA[p][i] / a;

/* Цикл k - цикл по строкам. (Все ветви "крутят" этот цикл). */
#pragma omp parallel for private (i, j, a)
 for(j = p + 1; j < M; j++) {
 a = MA[j][p];
 for(i = p; i <= M; i++)
MA[j][i] = MA[j][i] - a * MA[p][i];
}
}
/* Обратный ход */
 for(p = M - 1; p >= 0; p--) {
#pragma omp parallel for private (i, j)
 for(j = p - 1; j >=0; j--) {
for(i = M; i > j; i--)
MA[j][i] = MA[j][i]- MA[j][p] * MA[p][i];
}
}
gettimeofday(&tv_end, NULL);
/* Вычисленные значения решения расположены в последнем столбце
 расширенной матрицы MA */
45
 /* Выдаются для контроля первые четыре значения корня */
 printf("Вычисленные значения: %13.4e %13.4e %13.4e %13.4e \n", MA[0][M],
MA[1][M], MA[2][M], MA[3][M]);
 printf("Точные значения: %13.4e %13.4e %13.4e %13.4e\n", V[0], V[1], V[2],
V[3]);
printf("Время счета = %.6f sec.\n", (tv_end.tv_sec * 1E6 + tv_end.tv_usec -
 tv_start.tv_sec * 1E6 + tv_start.tv_usec) / 1E6);
 return 0;
}
