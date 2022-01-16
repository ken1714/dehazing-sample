# Executable file
cuda_add_executable(main_dehazing main_dehazing.cpp)
target_link_libraries(main_dehazing ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} dehazing)
