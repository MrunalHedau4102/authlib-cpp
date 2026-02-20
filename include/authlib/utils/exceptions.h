/**
 * Custom exceptions for AuthLib
 */

#ifndef AUTHLIB_EXCEPTIONS_H
#define AUTHLIB_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace authlib {

class AuthException : public std::runtime_error {
public:
    explicit AuthException(const std::string& message) : std::runtime_error(message) {}
};

class UserNotFound : public AuthException {
public:
    explicit UserNotFound(const std::string& message = "User not found")
        : AuthException(message) {}
};

class InvalidCredentials : public AuthException {
public:
    explicit InvalidCredentials(const std::string& message = "Invalid email or password")
        : AuthException(message) {}
};

class InvalidToken : public AuthException {
public:
    explicit InvalidToken(const std::string& message = "Invalid or expired token")
        : AuthException(message) {}
};

class UserAlreadyExists : public AuthException {
public:
    explicit UserAlreadyExists(const std::string& message = "User already exists")
        : AuthException(message) {}
};

class ValidationError : public AuthException {
public:
    explicit ValidationError(const std::string& message = "Validation failed")
        : AuthException(message) {}
};

class DatabaseError : public AuthException {
public:
    explicit DatabaseError(const std::string& message = "Database operation failed")
        : AuthException(message) {}
};

} // namespace authlib

#endif // AUTHLIB_EXCEPTIONS_H
