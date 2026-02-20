/**
 * Password hashing and verification utility
 */

#ifndef AUTHLIB_PASSWORD_HANDLER_H
#define AUTHLIB_PASSWORD_HANDLER_H

#include <string>

namespace authlib {

class PasswordHandler {
public:
    /**
     * Hash a password using bcrypt
     */
    std::string hashPassword(const std::string& password);

    /**
     * Verify a password against a hash
     */
    bool verifyPassword(const std::string& password, const std::string& hash);

    /**
     * Check if a password needs rehashing
     */
    bool needsRehashing(const std::string& hash);

private:
    static constexpr int SALT_ROUNDS = 12;
};

} // namespace authlib

#endif // AUTHLIB_PASSWORD_HANDLER_H
