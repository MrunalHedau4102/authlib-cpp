/**
 * Database connection and management
 */

#ifndef AUTHLIB_DATABASE_H
#define AUTHLIB_DATABASE_H

#include <string>
#include <memory>
#include <authlib/models/User.h>
#include <authlib/models/TokenBlacklist.h>
#include <authlib/config/Config.h>

namespace authlib {

class Database {
public:
    explicit Database(const std::string& connectionUrl);
    
    ~Database();

    /**
     * Initialize database and create tables
     */
    void initialize();

    /**
     * Check if database is connected
     */
    bool isConnected() const;

    /**
     * Insert a user
     */
    User insertUser(const User& user);

    /**
     * Find user by ID
     */
    User findUserById(uint32_t id);

    /**
     * Find user by email
     */
    User findUserByEmail(const std::string& email);

    /**
     * Update user
     */
    void updateUser(const User& user);

    /**
     * Blacklist a token
     */
    void blacklistToken(const TokenBlacklist& entry);

    /**
     * Check if token is blacklisted
     */
    bool isTokenBlacklisted(const std::string& token);

    /**
     * Clean expired blacklisted tokens
     */
    void cleanExpiredTokens();

private:
    std::string connectionUrl;
    void* dbHandle; // SQLite3 or DB-specific handle

    void createTables();
};

} // namespace authlib

#endif // AUTHLIB_DATABASE_H
