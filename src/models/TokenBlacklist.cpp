#include <authlib/models/TokenBlacklist.h>

namespace authlib {

TokenBlacklist::TokenBlacklist()
    : id(0),
      token(""),
      userId(0),
      expiresAt(0),
      blacklistedAt(std::time(nullptr)) {}

json TokenBlacklist::toJson() const {
    return json{
        {"id", id},
        {"userId", userId},
        {"expiresAt", expiresAt},
        {"blacklistedAt", blacklistedAt}
    };
}

} // namespace authlib
