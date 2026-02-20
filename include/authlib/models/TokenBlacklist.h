/**
 * Token blacklist model for logout and revocation
 */

#ifndef AUTHLIB_TOKEN_BLACKLIST_H
#define AUTHLIB_TOKEN_BLACKLIST_H

#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace authlib {

class TokenBlacklist {
public:
    uint32_t id;
    std::string token;
    uint32_t userId;
    std::time_t expiresAt;
    std::time_t blacklistedAt;

    TokenBlacklist();

    json toJson() const;
};

} // namespace authlib

#endif // AUTHLIB_TOKEN_BLACKLIST_H
