# 06
Лабораторная работа 6. Вариант 8.<br>
Студент группы 3540904/10102:<br>
Мамадалиев Шерзод Рустам угли

## Задание
Дана последовательность натуральных чисел {a0...an–1}.
Создать OpenMP-приложение для поиска суммы ∑ai, где ai – четные числа.

## Последовательная программа
C:
```c
#include "stdio.h"

#define N 20

int main() {
    int sum = 0;

    for (int i = 1; i < N; ++i) {
        if (i % 2 == 0) {
            sum += i;
            printf("Current even number is %d\n", i);
        }
    }

    printf("\nThe sum of natural numbers from 1 to %d is equal to %d.\n", N, sum);
    return 0;
}
```

Запуск:
```shell
gcc -fopenmp sum_of_natural_numbers.c
```

Вывод:
```shell
Current even number is 2
Current even number is 4
Current even number is 6
Current even number is 8
Current even number is 10
Current even number is 12
Current even number is 14
Current even number is 16
Current even number is 18

The sum of natural numbers from 1 to 20 is equal to 90.
```

## Параллельная программа
C:
```c
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
```

Конфигурации запуска:
```bash
export OMP_NUM_THREADS=4
```

Вывод:
```shell
Current even number is 2
Current even number is 4
Current even number is 6
Current even number is 8
Current even number is 10
Current even number is 16
Current even number is 18
Current even number is 12
Current even number is 14

The sum of natural numbers from 1 to 20 is equal to 90.
```
