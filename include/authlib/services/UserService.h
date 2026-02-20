/**
 * User management service
 */

#ifndef AUTHLIB_USER_SERVICE_H
#define AUTHLIB_USER_SERVICE_H

#include <string>
#include <authlib/models/User.h>
#include <authlib/database/Database.h>

namespace authlib {

struct CreateUserInput {
    std::string email;
    std::string password;
    std::string firstName;
    std::string lastName;
};

class UserService {
public:
    explicit UserService(Database& database);

    /**
     * Create a new user
     */
    User createUser(const CreateUserInput& input);

    /**
     * Get user by ID
     */
    User getUserById(uint32_t userId);

    /**
     * Get user by email
     */
    User getUserByEmail(const std::string& email);

    /**
     * Update user
     */
    User updateUser(uint32_t userId, const User& updates);

    /**
     * Activate user
     */
    User activateUser(uint32_t userId);

    /**
     * Deactivate user
     */
    User deactivateUser(uint32_t userId);

    /**
     * Verify user
     */
    User verifyUser(uint32_t userId);

    /**
     * Update last login
     */
    User updateLastLogin(uint32_t userId);

private:
    Database& database;
};

} // namespace authlib

#endif // AUTHLIB_USER_SERVICE_H
