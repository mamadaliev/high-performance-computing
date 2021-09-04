/**
 * Лабораторная работа 6. Вариант 8.
 * Дана последовательность натуральных чисел {a0...an–1}.
 * Создать OpenMP-приложение для поиска суммы ∑ai, где ai – четные числа.
 *
 * Студень группы 3540904/10102:
 * Мамадалиев Шерзод Рустам угли
 *
 */
#include "stdio.h"
#include "omp.h"

#define N 20

int main() {
    int sum = 0;

#pragma omp parallel for
    for (int i = 1; i < N; ++i) {
        if (i % 2 == 0) {
            sum += i;
            printf("Current even number is %d\n", i);
        }
    }

    printf("\nThe sum of natural numbers from 1 to %d is equal to %d.\n", N, sum);
    return 0;
}
