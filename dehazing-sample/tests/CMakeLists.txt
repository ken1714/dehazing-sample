include_directories(include)
add_subdirectory(googletest)

# Build macro
function(build_test test_name test_src)
    # Executable file
    cuda_add_executable(${test_name} ${test_src} src/utils.cpp)
    target_link_libraries(${test_name} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} dehazing gmock gtest gmock_main)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endfunction(build_test)

build_test(test_atmospheric_light test_atmospheric_light.cpp)
build_test(test_dark_channel_prior test_dark_channel_prior.cpp)
build_test(test_dehazing test_dehazing.cpp)
build_test(test_remove_haze test_remove_haze.cpp)
build_test(test_transmission_map test_transmission_map.cpp)
