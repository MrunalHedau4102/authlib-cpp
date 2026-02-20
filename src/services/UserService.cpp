#include <authlib/services/UserService.h>
#include <authlib/utils/PasswordHandler.h>
#include <authlib/utils/Validators.h>
#include <authlib/utils/exceptions.h>

namespace authlib {

UserService::UserService(Database& database) : database(database) {}

User UserService::createUser(const CreateUserInput& input) {
    // Validate email and password
    EmailValidator::validate(input.email);
    PasswordValidator::validate(input.password);

    // Check if user exists
    try {
        getUserByEmail(input.email);
        throw UserAlreadyExists("User with email " + input.email + " already exists");
    } catch (const UserNotFound&) {
        // Expected - user doesn't exist yet
    }

    // Hash password
    PasswordHandler passwordHandler;
    User user;
    user.email = input.email;
    user.passwordHash = passwordHandler.hashPassword(input.password);
    user.firstName = input.firstName;
    user.lastName = input.lastName;
    user.isActive = true;
    user.isVerified = false;

    // Insert user into database
    return database.insertUser(user);
}

User UserService::getUserById(uint32_t userId) {
    return database.findUserById(userId);
}

User UserService::getUserByEmail(const std::string& email) {
    return database.findUserByEmail(email);
}

User UserService::updateUser(uint32_t userId, const User& updates) {
    User user = getUserById(userId);
    // Update fields
    user.firstName = updates.firstName;
    user.lastName = updates.lastName;
    user.isActive = updates.isActive;
    user.isVerified = updates.isVerified;
    user.lastLogin = updates.lastLogin;
    user.updatedAt = std::time(nullptr);

    database.updateUser(user);
    return user;
}

User UserService::activateUser(uint32_t userId) {
    User user = getUserById(userId);
    user.isActive = true;
    return updateUser(userId, user);
}

User UserService::deactivateUser(uint32_t userId) {
    User user = getUserById(userId);
    user.isActive = false;
    return updateUser(userId, user);
}

User UserService::verifyUser(uint32_t userId) {
    User user = getUserById(userId);
    user.isVerified = true;
    return updateUser(userId, user);
}

User UserService::updateLastLogin(uint32_t userId) {
    User user = getUserById(userId);
    user.lastLogin = std::time(nullptr);
    return updateUser(userId, user);
}

} // namespace authlib
