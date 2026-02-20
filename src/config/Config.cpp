#include <authlib/config/Config.h>
#include <cstdlib>
#include <stdexcept>

namespace authlib {

Config::Config() {
    JWT_SECRET_KEY = getEnv("JWT_SECRET_KEY", "change-me-in-production");
    JWT_ALGORITHM = getEnv("JWT_ALGORITHM", "HS256");
    JWT_ACCESS_TOKEN_EXPIRY_MINUTES = std::stoul(getEnv("JWT_ACCESS_TOKEN_EXPIRY_MINUTES", "15"));
    JWT_REFRESH_TOKEN_EXPIRY_DAYS = std::stoul(getEnv("JWT_REFRESH_TOKEN_EXPIRY_DAYS", "7"));

    DATABASE_URL = getEnv("DATABASE_URL", "sqlite:///./authlib.db");
    DATABASE_TYPE = getEnv("DATABASE_TYPE", "sqlite");

    SMTP_SERVER = getEnv("SMTP_SERVER", "smtp.gmail.com");
    SMTP_USERNAME = getEnv("SMTP_USERNAME", "");
    SMTP_PASSWORD = getEnv("SMTP_PASSWORD", "");
    SMTP_FROM = getEnv("SMTP_FROM", "noreply@authlib.dev");

    DEBUG = getEnv("DEBUG", "false") == "true";
}

void Config::validate() const {
    if (JWT_SECRET_KEY == "change-me-in-production") {
        throw std::runtime_error("JWT_SECRET_KEY must be set in production");
    }
    if (DATABASE_URL.empty()) {
        throw std::runtime_error("DATABASE_URL must be set");
    }
}

bool Config::isProductionMode() const {
    const char* env = std::getenv("MODE");
    return env && std::string(env) == "production";
}

std::string Config::getEnv(const std::string& key, const std::string& defaultValue) {
    const char* value = std::getenv(key.c_str());
    return value ? std::string(value) : defaultValue;
}

} // namespace authlib
