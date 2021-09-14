# schpc
Laboratory works for the course "Supercomputers and High Performance Computing".

## Requirements
- Ubuntu 20.04 LTS
- GCC/Clang
- OpenMP
- OpenMPI

## Enviroment setup
First, install the requirements:

```bash
apt update
apt install gcc
apt install clang
apt install build-essential
apt install libomp-dev
apt install openmpi-bin
```

CMake configurations for pthread an openmp:
```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(MyProject main.cpp)
```

CMake configurations for openmpi:
```cmake
cmake_minimum_required(VERSION 3.18.4)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

find_package (MPI)

set (EXTRA_INCLUDES ${MPI_CXX_INCLUDE_DIRS})
set (EXTRA_CXX_FLAGS ${EXTRA_CXX_FLAGS} ${MPI_CXX_COMPILE_FLAGS})
set (EXTRA_LIBS "${EXTRA_LIBS} ${MPI_CXX_LIBRARIES}")
set (EXTRA_LIBS "${EXTRA_LIBS} ${MPI_CXX_LINK_FLAGS}")

add_executable(MyProject main.cpp)

target_include_directories (MyProject PUBLIC ${MPI_CXX_INCLUDE_DIRS})
target_link_libraries (MyProject ${MPI_CXX_LIBRARIES})
```

Set thread size for openmp:
```shell
export OMP_NUM_THREADS=4
```
