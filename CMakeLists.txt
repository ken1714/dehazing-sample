cmake_minimum_required(VERSION 3.1)
project(dehazing-sample)

set(CMAKE_CXX_STANDARD 17)
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS}; -std=c++17")

# Dependencies
find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

# Include
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(include)

add_subdirectory(src)
add_subdirectory(samples)

# Test code
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
