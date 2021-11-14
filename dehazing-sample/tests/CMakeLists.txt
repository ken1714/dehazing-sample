include_directories(include)
add_subdirectory(googletest)

# Build macro
function(build_test test_name test_src)
    # Executable file
    add_executable(test_name ${test_src} src/utils.cpp)
    target_link_libraries(test_name ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} dehazing gmock gtest gmock_main)
    add_test(NAME GoogleTestTest COMMAND test_name WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endfunction(build_test)

build_test(test_dark_channel_prior test_dark_channel_prior.cpp)
