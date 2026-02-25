cmake_minimum_required(VERSION 3.15)
project(authlib VERSION 1.0.0 LANGUAGES CXX)

# ------------------------
# C++ Settings
# ------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ------------------------
# Dependencies
# ------------------------
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(PostgreSQL)

# jwt-cpp (header-only library - fetch from GitHub)
include(FetchContent)
FetchContent_Declare(
  jwt_cpp
  URL https://github.com/Thalhammer/jwt-cpp/archive/refs/tags/v0.9.1.zip
)
FetchContent_MakeAvailable(jwt_cpp)

# ------------------------
# Source Files
# ------------------------
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

# ------------------------
# Library
# ------------------------
add_library(authlib SHARED ${SOURCES})

target_include_directories(authlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(authlib
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC jwt_cpp    
    PRIVATE OpenSSL::Crypto
    PRIVATE SQLite::SQLite3
    PRIVATE Boost::system
)

if(PostgreSQL_FOUND)
    target_link_libraries(authlib PRIVATE PostgreSQL::PostgreSQL)
    target_compile_definitions(authlib PRIVATE WITH_POSTGRESQL=1)
endif()

# ------------------------
# Install
# ------------------------
install(TARGETS authlib
        EXPORT authlibTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

# ------------------------
# Testing (GoogleTest via FetchContent)
# ------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Prevent overriding parent compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Prevent GoogleTest from installing to system directories
set(INSTALL_GTEST OFF)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)