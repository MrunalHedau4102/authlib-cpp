cmake_minimum_required(VERSION 3.15)

# Create test executable
add_executable(authlib_tests
    integration_tests.cpp
)

# Link against your library + fetched GoogleTest
target_link_libraries(authlib_tests
    PRIVATE
        authlib
        gtest
        gtest_main
)

# Enable testing
enable_testing()

add_test(
    NAME authlib_integration_tests
    COMMAND authlib_tests
)

# Optional verbose test
add_test(
    NAME authlib_integration_tests_verbose
    COMMAND authlib_tests --gtest_print_time=1
)