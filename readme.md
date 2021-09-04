# schpc
Laboratory works for the course "Supercomputers and High Performance Computing".

## Requirements
- Ubuntu 20.04 LTS
- GCC/Clang
- OpenMP
- OpenMPI

## Enviroment setup
First, install the requirements:
```
apt update
apt install gcc
apt install clang
apt install build-essential
apt install libomp-dev
apt install openmpi-bin
```

## Open MP
You can test programs via `gcc/g++` or `clang` writing via Open MP.
For example, simple program via Open MP on C language:
```c
#include "stdio.h"
#include "omp.h"

int main() {
#pragma omp parallel
    {
        int count = omp_get_thread_num();
        int its_me = omp_get_num_threads();
        printf("Hello, OpenMP! I am %d of %d\n", count, its_me);
    }
    return 0;
}
```

Compile this program like this:
```
gcc -fopenmp hello_openmp.c
``

Next, need to setup count of threads in enviroments. For example, we need to use 4 theads, 
then execute following command on terminal:
```
export OMP_NUM_THREADS=4
```

And if execute this program with 4 threads, resul of the executing will look like this:
```
./a.out
Hello, OpenMP! I am 1 of 8
Hello, OpenMP! I am 0 of 8
Hello, OpenMP! I am 3 of 8
Hello, OpenMP! I am 6 of 8
Hello, OpenMP! I am 2 of 8
Hello, OpenMP! I am 5 of 8
Hello, OpenMP! I am 4 of 8
Hello, OpenMP! I am 7 of 8
```

Or with 4 threads count like:
```
export OMP_NUM_THREADS=4
```

Output:
```
Hello, OpenMP! I am 0 of 2
Hello, OpenMP! I am 1 of 2
```

## Open MPI

In progress...
