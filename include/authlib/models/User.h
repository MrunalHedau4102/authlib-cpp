/**
 * User database model
 */

#ifndef AUTHLIB_USER_H
#define AUTHLIB_USER_H

#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace authlib {

class User {
public:
    uint32_t id;
    std::string email;
    std::string passwordHash;
    std::string firstName;
    std::string lastName;
    bool isActive;
    bool isVerified;
    std::time_t createdAt;
    std::time_t updatedAt;
    std::time_t lastLogin;

    User();

    json toJson() const;
    std::string toString() const;
};

} // namespace authlib

#endif // AUTHLIB_USER_H
