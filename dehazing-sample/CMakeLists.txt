cmake_minimum_required(VERSION 3.1)
project(dehazing-sample)

set(CMAKE_CXX_STANDARD 17)

# Dependencies
find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

# Include
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})

# Executable file
add_executable(main_dehazing main_dehazing.cpp)
target_link_libraries(main_dehazing ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
