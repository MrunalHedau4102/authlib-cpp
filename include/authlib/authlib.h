/**
 * Main header file for AuthLib
 */

#ifndef AUTHLIB_H
#define AUTHLIB_H

// Config
#include <authlib/config/Config.h>

// Models
#include <authlib/models/User.h>
#include <authlib/models/TokenBlacklist.h>

// Services
#include <authlib/services/AuthService.h>
#include <authlib/services/UserService.h>

// Utilities
#include <authlib/utils/exceptions.h>
#include <authlib/utils/JWTHandler.h>
#include <authlib/utils/PasswordHandler.h>
#include <authlib/utils/Validators.h>

// Database
#include <authlib/database/Database.h>

namespace authlib {
constexpr const char* VERSION = "1.0.0";
}

#endif // AUTHLIB_H
