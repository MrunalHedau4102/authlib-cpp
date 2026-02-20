/**
 * Input validators for email and password
 */

#ifndef AUTHLIB_VALIDATORS_H
#define AUTHLIB_VALIDATORS_H

#include <string>
#include <authlib/utils/exceptions.h>

namespace authlib {

class EmailValidator {
public:
    static void validate(const std::string& email);
};

class PasswordValidator {
public:
    static void validate(const std::string& password);
};

} // namespace authlib

#endif // AUTHLIB_VALIDATORS_H
