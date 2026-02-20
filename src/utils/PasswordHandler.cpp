#include <authlib/utils/PasswordHandler.h>
#include <openssl/crypto.h>
#include <cstring>
#include <stdexcept>

// Simplified bcrypt implementation (production should use bcrypt library)
// For now using OpenSSL PBKDF2 as fallback

namespace authlib {

std::string PasswordHandler::hashPassword(const std::string& password) {
    // This is a simplified implementation
    // In production, integrate with crypt() or bcrypt library
    unsigned char hash[32];
    unsigned int hash_len = 32;

    // Generate salt (simplified)
    unsigned char salt[16];
    for (int i = 0; i < 16; ++i) {
        salt[i] = static_cast<unsigned char>(rand() % 256);
    }

    // Use PBKDF2 for demonstration
    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            salt,
            sizeof(salt),
            10000,
            EVP_sha256(),
            hash_len,
            hash
        )) {
        throw std::runtime_error("Password hashing failed");
    }

    // Return hash as hex string (simplified)
    std::string result;
    result.reserve(hash_len * 2);
    for (unsigned int i = 0; i < hash_len; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        result += buf;
    }
    return result;
}

bool PasswordHandler::verifyPassword(const std::string& password, const std::string& hash) {
    try {
        std::string newHash = hashPassword(password);
        return newHash == hash;
    } catch (...) {
        return false;
    }
}

bool PasswordHandler::needsRehashing(const std::string& hash) {
    return hash.length() < 32;
}

} // namespace authlib
