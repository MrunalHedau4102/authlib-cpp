/**
 * Configuration management for AuthLib
 */

#ifndef AUTHLIB_CONFIG_H
#define AUTHLIB_CONFIG_H

#include <string>
#include <cstdint>

namespace authlib {

class Config {
public:
    std::string JWT_SECRET_KEY;
    std::string JWT_ALGORITHM;
    uint32_t JWT_ACCESS_TOKEN_EXPIRY_MINUTES;
    uint32_t JWT_REFRESH_TOKEN_EXPIRY_DAYS;

    std::string DATABASE_URL;
    std::string DATABASE_TYPE;

    std::string SMTP_SERVER;
    std::string SMTP_USERNAME;
    std::string SMTP_PASSWORD;
    std::string SMTP_FROM;

    bool DEBUG;

    Config();
    
    void validate() const;
    bool isProductionMode() const;

private:
    std::string getEnv(const std::string& key, const std::string& defaultValue = "");
};

} // namespace authlib

#endif // AUTHLIB_CONFIG_H
