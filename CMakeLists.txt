cmake_minimum_required(VERSION 3.22)
project(progko)

set(CMAKE_CXX_STANDARD 14)

# Find MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find OpenMP
find_package(OpenMP REQUIRED)

# Add executable
add_executable(progko
        src/main.cpp
        src/helper.cpp
        src/helper.h
        src/core_plain.cpp
        src/core_plain.h
        src/core_omp.cpp
        src/core_omp.h
        src/core_mpi.cpp
        src/core_mpi.h
        src/core_omp_mpi.cpp
        src/core_omp_mpi.h)

# Link libraries
target_link_libraries(progko
        ${MPI_LIBRARIES}
        ${OpenCV_LIBS}
        OpenMP::OpenMP_CXX)
