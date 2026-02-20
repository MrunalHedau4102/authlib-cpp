cmake_minimum_required(VERSION 3.10)

# Google Test
find_package(GTest REQUIRED)

# Create test executable
add_executable(authlib_tests
    integration_tests.cpp
)

# Link libraries
target_link_libraries(authlib_tests
    PRIVATE authlib
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
)

# Enable testing
enable_testing()
add_test(
    NAME authlib_integration_tests
    COMMAND authlib_tests
)

# Add verbose output
add_test(
    NAME authlib_integration_tests_verbose
    COMMAND authlib_tests --gtest_filter=* --gtest_print_time=1
)
