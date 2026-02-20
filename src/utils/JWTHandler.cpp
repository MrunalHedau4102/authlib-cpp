#include <authlib/utils/JWTHandler.h>
#include <authlib/utils/exceptions.h>
#include <jwt-cpp/jwt.h>
#include <ctime>

namespace authlib {

JWTHandler::JWTHandler(const Config& config) : config(config) {}

std::string JWTHandler::createAccessToken(
    uint32_t userId,
    const std::string& email,
    const json& additionalClaims
) {
    uint32_t expirySeconds = config.JWT_ACCESS_TOKEN_EXPIRY_MINUTES * 60;
    return createToken(userId, email, "access", expirySeconds, additionalClaims);
}

std::string JWTHandler::createRefreshToken(
    uint32_t userId,
    const std::string& email,
    const json& additionalClaims
) {
    uint32_t expirySeconds = config.JWT_REFRESH_TOKEN_EXPIRY_DAYS * 86400;
    return createToken(userId, email, "refresh", expirySeconds, additionalClaims);
}

std::string JWTHandler::createToken(
    uint32_t userId,
    const std::string& email,
    const std::string& type,
    uint32_t expirySeconds,
    const json& additionalClaims
) {
    if (userId == 0) {
        throw ValidationError("userId must be a positive number");
    }
    if (email.empty()) {
        throw ValidationError("email must not be empty");
    }

    auto now = std::time(nullptr);
    auto expired = now + expirySeconds;

    try {
        auto token = jwt::create()
            .set_issuer("authlib")
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds(expirySeconds))
            .set_payload_claim("userId", jwt::claim(static_cast<int>(userId)))
            .set_payload_claim("email", jwt::claim(email))
            .set_payload_claim("type", jwt::claim(type));

        // Add additional claims
        for (auto& [key, value] : additionalClaims.items()) {
            token.set_payload_claim(key, jwt::claim(value.dump()));
        }

        return token.sign(jwt::algorithm::hs256{ config.JWT_SECRET_KEY });
    } catch (const std::exception& e) {
        throw InvalidToken(std::string("Token creation failed: ") + e.what());
    }
}

TokenPayload JWTHandler::verifyToken(const std::string& token) {
    try {
        auto decoded = jwt::decode(token);
        decoded.verify(jwt::algorithm::hs256{ config.JWT_SECRET_KEY });

        return parsePayload(decoded.get_payload());
    } catch (const std::exception& e) {
        throw InvalidToken(std::string("Token verification failed: ") + e.what());
    }
}

TokenPayload JWTHandler::decodeToken(const std::string& token) {
    try {
        auto decoded = jwt::decode(token);
        return parsePayload(decoded.get_payload());
    } catch (...) {
        throw InvalidToken("Failed to decode token");
    }
}

TokenPayload JWTHandler::parsePayload(const json& payload) {
    TokenPayload result;
    result.userId = payload.at("userId").get<uint32_t>();
    result.email = payload.at("email").get<std::string>();
    result.type = payload.at("type").get<std::string>();

    if (payload.contains("iat")) {
        result.iat = payload.at("iat").get<uint32_t>();
    }
    if (payload.contains("exp")) {
        result.exp = payload.at("exp").get<uint32_t>();
    }

    return result;
}

} // namespace authlib
