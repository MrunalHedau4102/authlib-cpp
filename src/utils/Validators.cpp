#include <authlib/utils/Validators.h>
#include <regex>

namespace authlib {

void EmailValidator::validate(const std::string& email) {
    if (email.empty()) {
        throw ValidationError("Email must not be empty");
    }

    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!std::regex_match(email, emailRegex)) {
        throw ValidationError("Invalid email format");
    }

    if (email.length() > 254) {
        throw ValidationError("Email must not exceed 254 characters");
    }
}

void PasswordValidator::validate(const std::string& password) {
    if (password.empty()) {
        throw ValidationError("Password must not be empty");
    }

    if (password.length() < 8) {
        throw ValidationError("Password must be at least 8 characters long");
    }

    if (password.length() > 128) {
        throw ValidationError("Password must not exceed 128 characters");
    }

    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasNumber = false;
    bool hasSpecialChar = false;

    for (char c : password) {
        if (std::isupper(c)) hasUpperCase = true;
        else if (std::islower(c)) hasLowerCase = true;
        else if (std::isdigit(c)) hasNumber = true;
        else if (!std::isalnum(c)) hasSpecialChar = true;
    }

    if (!hasUpperCase || !hasLowerCase || !hasNumber || !hasSpecialChar) {
        throw ValidationError(
            "Password must contain uppercase, lowercase, number, and special character"
        );
    }
}

} // namespace authlib
