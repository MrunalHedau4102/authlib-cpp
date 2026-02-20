#include <authlib/database/Database.h>
#include <authlib/utils/exceptions.h>
#include <sqlite3.h>
#include <stdexcept>

namespace authlib {

Database::Database(const std::string& connectionUrl)
    : connectionUrl(connectionUrl), dbHandle(nullptr) {}

Database::~Database() {
    if (dbHandle) {
        sqlite3_close(static_cast<sqlite3*>(dbHandle));
    }
}

void Database::initialize() {
    try {
        sqlite3* db;
        int result = sqlite3_open(connectionUrl.c_str(), &db);

        if (result != SQLITE_OK) {
            throw DatabaseError("Failed to open database: " + std::string(sqlite3_errmsg(db)));
        }

        dbHandle = db;
        createTables();
    } catch (const std::exception& e) {
        throw DatabaseError("Database initialization failed: " + std::string(e.what()));
    }
}

bool Database::isConnected() const {
    return dbHandle != nullptr;
}

void Database::createTables() {
    if (!dbHandle) {
        throw DatabaseError("Database not connected");
    }

    sqlite3* db = static_cast<sqlite3*>(dbHandle);
    const char* createUsersSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "email TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "first_name TEXT,"
        "last_name TEXT,"
        "is_active BOOLEAN DEFAULT 1,"
        "is_verified BOOLEAN DEFAULT 0,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "last_login DATETIME"
        ");";

    const char* createTokenBlacklistSQL =
        "CREATE TABLE IF NOT EXISTS token_blacklist ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "token TEXT NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "expires_at DATETIME NOT NULL,"
        "blacklisted_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createUsersSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw DatabaseError("Failed to create users table: " + error);
    }

    if (sqlite3_exec(db, createTokenBlacklistSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw DatabaseError("Failed to create token_blacklist table: " + error);
    }
}

User Database::insertUser(const User& user) {
    // Implementation for SQLite
    throw DatabaseError("Not fully implemented in this example");
}

User Database::findUserById(uint32_t id) {
    throw DatabaseError("Not fully implemented in this example");
}

User Database::findUserByEmail(const std::string& email) {
    throw DatabaseError("Not fully implemented in this example");
}

void Database::updateUser(const User& user) {
    throw DatabaseError("Not fully implemented in this example");
}

void Database::blacklistToken(const TokenBlacklist& entry) {
    throw DatabaseError("Not fully implemented in this example");
}

bool Database::isTokenBlacklisted(const std::string& token) {
    throw DatabaseError("Not fully implemented in this example");
}

void Database::cleanExpiredTokens() {
    throw DatabaseError("Not fully implemented in this example");
}

} // namespace authlib
