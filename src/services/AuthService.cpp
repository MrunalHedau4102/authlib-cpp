#include <authlib/services/AuthService.h>
#include <authlib/utils/Validators.h>
#include <authlib/utils/exceptions.h>

namespace authlib {

json AuthResponse::toJson() const {
    return json{
        {"success", success},
        {"user", user.toJson()},
        {"accessToken", accessToken},
        {"refreshToken", refreshToken}
    };
}

AuthService::AuthService(Database& database, const Config& config)
    : database(database),
      userService(database),
      jwtHandler(config),
      config(config) {}

AuthResponse AuthService::registerUser(const RegisterInput& input) {
    // Validate inputs
    EmailValidator::validate(input.email);
    PasswordValidator::validate(input.password);

    // Create user
    CreateUserInput userInput{input.email, input.password, input.firstName, input.lastName};
    User user = userService.createUser(userInput);

    // Generate tokens
    json tokens = generateTokens(user);

    return {
        true,
        user,
        tokens["accessToken"].get<std::string>(),
        tokens["refreshToken"].get<std::string>()
    };
}

AuthResponse AuthService::login(const LoginInput& input) {
    // Validate inputs
    EmailValidator::validate(input.email);

    // Find user
    User user = userService.getUserByEmail(input.email);

    // Check if user is active
    if (!user.isActive) {
        throw InvalidCredentials("User account is deactivated");
    }

    // Verify password
    if (!passwordHandler.verifyPassword(input.password, user.passwordHash)) {
        throw InvalidCredentials("Invalid email or password");
    }

    // Update last login
    userService.updateLastLogin(user.id);

    // Generate tokens
    json tokens = generateTokens(user);

    return {
        true,
        user,
        tokens["accessToken"].get<std::string>(),
        tokens["refreshToken"].get<std::string>()
    };
}

TokenPayload AuthService::verifyToken(const std::string& token) {
    return jwtHandler.verifyToken(token);
}

json AuthService::refreshAccessToken(const std::string& refreshToken) {
    TokenPayload decoded = jwtHandler.verifyToken(refreshToken);

    if (decoded.type != "refresh") {
        throw InvalidToken("Invalid token type");
    }

    // Check if token is blacklisted
    if (database.isTokenBlacklisted(refreshToken)) {
        throw InvalidToken("Token has been revoked");
    }

    // Generate new access token
    std::string accessToken = jwtHandler.createAccessToken(decoded.userId, decoded.email);

    return json{{"accessToken", accessToken}};
}

json AuthService::logout(const std::string& accessToken, const std::string& refreshToken) {
    TokenPayload accessPayload = jwtHandler.decodeToken(accessToken);
    TokenPayload refreshPayload = jwtHandler.decodeToken(refreshToken);

    // Blacklist both tokens
    TokenBlacklist accessEntry;
    accessEntry.token = accessToken;
    accessEntry.userId = accessPayload.userId;
    accessEntry.expiresAt = accessPayload.exp;

    TokenBlacklist refreshEntry;
    refreshEntry.token = refreshToken;
    refreshEntry.userId = refreshPayload.userId;
    refreshEntry.expiresAt = refreshPayload.exp;

    database.blacklistToken(accessEntry);
    database.blacklistToken(refreshEntry);

    return json{{"success", true}};
}

json AuthService::generateTokens(const User& user) {
    std::string accessToken = jwtHandler.createAccessToken(user.id, user.email);
    std::string refreshToken = jwtHandler.createRefreshToken(user.id, user.email);

    return json{
        {"accessToken", accessToken},
        {"refreshToken", refreshToken}
    };
}

json AuthService::userToResponse(const User& user) {
    return user.toJson();
}

} // namespace authlib
