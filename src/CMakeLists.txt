file(GLOB SRC_FILES *.cpp)
file(GLOB SRC_KERNELS *.cu)

cuda_add_library(dehazing ${SRC_FILES} ${SRC_KERNELS} SHARED)
