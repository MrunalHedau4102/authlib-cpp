cmake_minimum_required(VERSION 3.10)
project(authlib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(sqlite3 REQUIRED)

# Locate optional PostgreSQL
find_package(PostgreSQL)
find_package(Boost REQUIRED SYSTEM)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${OPENSSL_INCLUDE_DIR})

# Source files
set(SOURCES
    src/config/Config.cpp
    src/utils/PasswordHandler.cpp
    src/utils/JWTHandler.cpp
    src/utils/Validators.cpp
    src/utils/exceptions.cpp
    src/models/User.cpp
    src/models/TokenBlacklist.cpp
    src/database/Database.cpp
    src/services/UserService.cpp
    src/services/AuthService.cpp
)

# Create library
add_library(authlib SHARED ${SOURCES})

# Link libraries
target_link_libraries(authlib
    PUBLIC nlohmann_json::nlohmann_json
    PRIVATE jwt-cpp::jwt-cpp
    PRIVATE OpenSSL::Crypto
    PRIVATE sqlite3
    PRIVATE Boost::system
)

if(PostgreSQL_FOUND)
    target_link_libraries(authlib PRIVATE pq)
    target_compile_definitions(authlib PRIVATE WITH_POSTGRESQL=1)
endif()

# Install targets
install(TARGETS authlib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

# Build tests
enable_testing()
add_subdirectory(tests)
