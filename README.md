# AuthLib - C++ Authentication Library

A scalable, framework-agnostic C++ authentication library with JWT token management, user registration, login, and password reset functionality.

## Features

- User registration and login with email/password
- JWT-based access and refresh tokens
- Password reset flow
- Token blacklisting for logout and revocation
- User account management (activation/deactivation)
- Password strength validation and bcrypt hashing
- Database-agnostic: SQLite, PostgreSQL, MySQL support via ORM
- C++17 standard with modern design patterns
- Exception-based error handling
- Production-ready

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake >= 3.10
- OpenSSL >= 1.1.0
- nlohmann/json >= 3.2.0
- jwt-cpp >= 0.4.0
- SQLite3 or PostgreSQL library

## Installation

### Linux/macOS

```bash
# Clone repository
git clone https://github.com/authlib/authlib-cpp.git
cd authlib-cpp

# Install dependencies (Ubuntu/Debian)
sudo apt-get install libssl-dev nlohmann-json3-dev sqlite3 libsqlite3-dev

# Install dependencies (macOS)
brew install openssl nlohmann-json sqlite3

# Build
mkdir build && cd build
cmake ..
make
sudo make install
```

### Windows

```bash
# Using vcpkg
vcpkg install nlohmann-json openssl sqlite3 jwt-cpp

# Build with Visual Studio
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

## Quick Start

### 1. Set up environment

Create a `.env` file:

```env
JWT_SECRET_KEY=your-super-secret-key-change-this
DATABASE_URL=sqlite:///./authlib.db
DATABASE_TYPE=sqlite
SMTP_SERVER=smtp.gmail.com
SMTP_USERNAME=your-email@gmail.com
SMTP_PASSWORD=your-app-password
```

### 2. Initialize database

```cpp
#include <authlib/database/Database.h>

authlib::Database db("sqlite:///./authlib.db");
db.initialize();
```

### 3. Use in your application

```cpp
#include <authlib/AuthService.h>
#include <iostream>

int main() {
    authlib::Config config;
    authlib::Database db(config.DATABASE_URL);
    authlib::AuthService authService(db, config);

    // Register user
    auto registerResult = authService.registerUser({
        "user@example.com",
        "SecurePassword123!",
        "John",
        "Doe"
    });

    std::cout << "Access Token: " << registerResult.accessToken << std::endl;

    // Login user
    auto loginResult = authService.login({
        "user@example.com",
        "SecurePassword123!"
    });

    // Verify token
    auto decoded = authService.verifyToken(loginResult.accessToken);
    std::cout << "User ID: " << decoded.userId << std::endl;

    return 0;
}
```

## Publishing to Conan

1. Create Conan account: https://conan.io
2. Create conanfile.py in repository
3. Export recipe: `conan export . authlib/1.0@`
4. Upload: `conan upload authlib* -r conancenter`

Full guide: [CONAN_PUBLISHING.md](./CONAN_PUBLISHING.md)

## License

MIT
