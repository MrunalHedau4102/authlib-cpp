/**
 * Authentication service
 */

#ifndef AUTHLIB_AUTH_SERVICE_H
#define AUTHLIB_AUTH_SERVICE_H

#include <string>
#include <nlohmann/json.hpp>
#include <authlib/models/User.h>
#include <authlib/database/Database.h>
#include <authlib/services/UserService.h>
#include <authlib/utils/JWTHandler.h>
#include <authlib/config/Config.h>

using json = nlohmann::json;

namespace authlib {

struct RegisterInput {
    std::string email;
    std::string password;
    std::string firstName;
    std::string lastName;
};

struct LoginInput {
    std::string email;
    std::string password;
};

struct AuthResponse {
    bool success;
    User user;
    std::string accessToken;
    std::string refreshToken;

    json toJson() const;
};

class AuthService {
public:
    AuthService(Database& database, const Config& config = Config());

    /**
     * Register a new user
     */
    AuthResponse registerUser(const RegisterInput& input);

    /**
     * Login user
     */
    AuthResponse login(const LoginInput& input);

    /**
     * Verify access token
     */
    TokenPayload verifyToken(const std::string& token);

    /**
     * Refresh access token
     */
    json refreshAccessToken(const std::string& refreshToken);

    /**
     * Logout user (blacklist tokens)
     */
    json logout(const std::string& accessToken, const std::string& refreshToken);

private:
    Database& database;
    UserService userService;
    JWTHandler jwtHandler;
    PasswordHandler passwordHandler;
    const Config& config;

    json generateTokens(const User& user);
    json userToResponse(const User& user);
};

} // namespace authlib

#endif // AUTHLIB_AUTH_SERVICE_H
