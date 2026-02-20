/**
 * JWT token handling utility
 */

#ifndef AUTHLIB_JWT_HANDLER_H
#define AUTHLIB_JWT_HANDLER_H

#include <string>
#include <nlohmann/json.hpp>
#include <authlib/config/Config.h>

using json = nlohmann::json;

namespace authlib {

struct TokenPayload {
    uint32_t userId;
    std::string email;
    std::string type; // "access" or "refresh"
    uint32_t iat = 0; // issued at
    uint32_t exp = 0; // expiration
};

class JWTHandler {
public:
    explicit JWTHandler(const Config& config);

    /**
     * Create an access token
     */
    std::string createAccessToken(
        uint32_t userId,
        const std::string& email,
        const json& additionalClaims = json::object()
    );

    /**
     * Create a refresh token
     */
    std::string createRefreshToken(
        uint32_t userId,
        const std::string& email,
        const json& additionalClaims = json::object()
    );

    /**
     * Verify and decode a token
     */
    TokenPayload verifyToken(const std::string& token);

    /**
     * Decode token without verification (for inspection)
     */
    TokenPayload decodeToken(const std::string& token);

private:
    const Config& config;

    std::string createToken(
        uint32_t userId,
        const std::string& email,
        const std::string& type,
        uint32_t expirySeconds,
        const json& additionalClaims
    );

    TokenPayload parsePayload(const json& payload);
};

} // namespace authlib

#endif // AUTHLIB_JWT_HANDLER_H
