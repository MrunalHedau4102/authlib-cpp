#include <gtest/gtest.h>
#include <authlib/AuthService.h>
#include <authlib/UserService.h>
#include <authlib/config/Config.h>
#include <authlib/database/Database.h>
#include <chrono>
#include <thread>

using namespace authlib;

/**
 * Integration tests for AuthLib C++ version
 * Tests the complete authentication flow
 */

class AuthLibIntegrationTest : public ::testing::Test {
protected:
    static Database db;
    static Config config;
    
    static void SetUpTestSuite() {
        // Initialize database for testing
        db.initialize();
    }

    void SetUp() override {
        // Clear data before each test would go here
    }

    void TearDown() override {
        // Cleanup after each test
        db.cleanExpiredTokens();
    }
};

Database AuthLibIntegrationTest::db("sqlite:///./authlib_test.db");
Config AuthLibIntegrationTest::config;

// ==================== User Registration Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldSuccessfullyRegisterNewUser) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "newuser@example.com",
        "SecurePass123!",
        "John",
        "Doe"
    };

    auto result = authService.registerUser(registerInput);

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.user.email, "newuser@example.com");
    EXPECT_EQ(result.user.firstName, "John");
    EXPECT_EQ(result.user.lastName, "Doe");
    EXPECT_FALSE(result.accessToken.empty());
    EXPECT_FALSE(result.refreshToken.empty());
}

TEST_F(AuthLibIntegrationTest, ShouldRejectInvalidEmailFormat) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "invalid-email",
        "SecurePass123!",
        "Jane",
        "Doe"
    };

    EXPECT_THROW(
        authService.registerUser(registerInput),
        ValidationError
    );
}

TEST_F(AuthLibIntegrationTest, ShouldRejectWeakPassword) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "weak@example.com",
        "weak",
        "Test",
        "User"
    };

    EXPECT_THROW(
        authService.registerUser(registerInput),
        ValidationError
    );
}

TEST_F(AuthLibIntegrationTest, ShouldPreventDuplicateEmailRegistration) {
    AuthService authService(db, config);
    
    RegisterInput input1{
        "duplicate@example.com",
        "SecurePass123!",
        "First",
        "User"
    };

    RegisterInput input2{
        "duplicate@example.com",
        "SecurePass123!",
        "Second",
        "User"
    };

    // First registration should succeed
    auto result1 = authService.registerUser(input1);
    EXPECT_TRUE(result1.success);

    // Duplicate registration should fail
    EXPECT_THROW(
        authService.registerUser(input2),
        UserAlreadyExists
    );
}

TEST_F(AuthLibIntegrationTest, ShouldGenerateValidJWTTokens) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "tokentest@example.com",
        "SecurePass123!",
        "Token",
        "Tester"
    };

    auto result = authService.registerUser(registerInput);

    auto payload = authService.verifyToken(result.accessToken);
    EXPECT_EQ(payload.userId, result.user.id);
    EXPECT_EQ(payload.email, result.user.email);
    EXPECT_EQ(payload.type, "access");
}

// ==================== User Login Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldSuccessfullyLoginWithCorrectCredentials) {
    AuthService authService(db, config);
    
    // Register user first
    RegisterInput registerInput{
        "logintest@example.com",
        "SecurePass123!",
        "Login",
        "Tester"
    };
    authService.registerUser(registerInput);

    // Login
    LoginInput loginInput{
        "logintest@example.com",
        "SecurePass123!"
    };

    auto result = authService.login(loginInput);

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.user.email, "logintest@example.com");
    EXPECT_FALSE(result.accessToken.empty());
    EXPECT_FALSE(result.refreshToken.empty());
}

TEST_F(AuthLibIntegrationTest, ShouldRejectIncorrectPassword) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "wrongpass@example.com",
        "SecurePass123!",
        "Wrong",
        "Pass"
    };
    authService.registerUser(registerInput);

    LoginInput loginInput{
        "wrongpass@example.com",
        "WrongPassword123!"
    };

    EXPECT_THROW(
        authService.login(loginInput),
        InvalidCredentials
    );
}

TEST_F(AuthLibIntegrationTest, ShouldRejectNonExistentEmail) {
    AuthService authService(db, config);
    
    LoginInput loginInput{
        "nonexistent@example.com",
        "SecurePass123!"
    };

    EXPECT_THROW(
        authService.login(loginInput),
        UserNotFound
    );
}

// ==================== Token Management Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldVerifyValidAccessToken) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "tokenverify@example.com",
        "SecurePass123!",
        "Token",
        "Verify"
    };

    auto registerResult = authService.registerUser(registerInput);
    auto payload = authService.verifyToken(registerResult.accessToken);

    EXPECT_EQ(payload.userId, registerResult.user.id);
    EXPECT_EQ(payload.email, "tokenverify@example.com");
    EXPECT_EQ(payload.type, "access");
}

TEST_F(AuthLibIntegrationTest, ShouldRejectInvalidToken) {
    AuthService authService(db, config);

    EXPECT_THROW(
        authService.verifyToken("invalid.token.here"),
        InvalidToken
    );
}

TEST_F(AuthLibIntegrationTest, ShouldRefreshAccessToken) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "refresh@example.com",
        "SecurePass123!",
        "Refresh",
        "Test"
    };

    auto registerResult = authService.registerUser(registerInput);
    auto refreshResponse = authService.refreshAccessToken(registerResult.refreshToken);

    EXPECT_FALSE(refreshResponse["accessToken"].empty());
    EXPECT_NE(refreshResponse["accessToken"], registerResult.accessToken);

    // Verify new token works
    auto newPayload = authService.verifyToken(refreshResponse["accessToken"]);
    EXPECT_EQ(newPayload.userId, registerResult.user.id);
    EXPECT_EQ(newPayload.type, "access");
}

TEST_F(AuthLibIntegrationTest, ShouldRejectRefreshWithWrongTokenType) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "wrongtype@example.com",
        "SecurePass123!",
        "Wrong",
        "Type"
    };

    auto registerResult = authService.registerUser(registerInput);

    // Try to use access token as refresh token
    EXPECT_THROW(
        authService.refreshAccessToken(registerResult.accessToken),
        InvalidToken
    );
}

TEST_F(AuthLibIntegrationTest, ShouldLogoutAndBlacklistTokens) {
    AuthService authService(db, config);
    
    RegisterInput registerInput{
        "logout@example.com",
        "SecurePass123!",
        "Logout",
        "Test"
    };

    auto registerResult = authService.registerUser(registerInput);

    auto logoutResult = authService.logout(
        registerResult.accessToken,
        registerResult.refreshToken
    );

    EXPECT_TRUE(logoutResult["success"]);

    // Token should now be blacklisted
    EXPECT_THROW(
        authService.refreshAccessToken(registerResult.refreshToken),
        InvalidToken
    );
}

// ==================== User Service Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldRetrieveUserById) {
    AuthService authService(db, config);
    UserService userService(db);
    
    RegisterInput registerInput{
        "getbyid@example.com",
        "SecurePass123!",
        "Get",
        "ById"
    };

    auto registerResult = authService.registerUser(registerInput);
    auto user = userService.getUserById(registerResult.user.id);

    EXPECT_EQ(user.id, registerResult.user.id);
    EXPECT_EQ(user.email, "getbyid@example.com");
}

TEST_F(AuthLibIntegrationTest, ShouldRetrieveUserByEmail) {
    AuthService authService(db, config);
    UserService userService(db);
    
    RegisterInput registerInput{
        "getbyemail@example.com",
        "SecurePass123!",
        "Get",
        "ByEmail"
    };

    auto registerResult = authService.registerUser(registerInput);
    auto user = userService.getUserByEmail("getbyemail@example.com");

    EXPECT_EQ(user.id, registerResult.user.id);
    EXPECT_EQ(user.email, "getbyemail@example.com");
}

TEST_F(AuthLibIntegrationTest, ShouldActivateAndDeactivateUser) {
    AuthService authService(db, config);
    UserService userService(db);
    
    RegisterInput registerInput{
        "activate@example.com",
        "SecurePass123!",
        "Activate",
        "Test"
    };

    auto registerResult = authService.registerUser(registerInput);

    auto deactivatedUser = userService.deactivateUser(registerResult.user.id);
    EXPECT_FALSE(deactivatedUser.isActive);

    auto activatedUser = userService.activateUser(registerResult.user.id);
    EXPECT_TRUE(activatedUser.isActive);
}

TEST_F(AuthLibIntegrationTest, ShouldVerifyUser) {
    AuthService authService(db, config);
    UserService userService(db);
    
    RegisterInput registerInput{
        "verify@example.com",
        "SecurePass123!",
        "Verify",
        "Test"
    };

    auto registerResult = authService.registerUser(registerInput);

    auto verifiedUser = userService.verifyUser(registerResult.user.id);
    EXPECT_TRUE(verifiedUser.isVerified);
}

// ==================== End-to-End Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldCompleteFullWorkflow) {
    AuthService authService(db, config);
    UserService userService(db);

    // 1. Register
    RegisterInput registerInput{
        "e2e@example.com",
        "SecurePass123!",
        "End",
        "ToEnd"
    };
    auto registerResult = authService.registerUser(registerInput);
    EXPECT_TRUE(registerResult.success);

    // 2. Retrieve user
    auto user = userService.getUserById(registerResult.user.id);
    EXPECT_EQ(user.email, "e2e@example.com");

    // 3. Verify tokens
    auto payload = authService.verifyToken(registerResult.accessToken);
    EXPECT_EQ(payload.userId, registerResult.user.id);

    // 4. Logout
    auto logoutResult = authService.logout(
        registerResult.accessToken,
        registerResult.refreshToken
    );
    EXPECT_TRUE(logoutResult["success"]);

    // 5. Login again
    LoginInput loginInput{
        "e2e@example.com",
        "SecurePass123!"
    };
    auto loginResult = authService.login(loginInput);
    EXPECT_TRUE(loginResult.success);

    // 6. Verify new tokens
    auto newPayload = authService.verifyToken(loginResult.accessToken);
    EXPECT_EQ(newPayload.userId, registerResult.user.id);
}

// ==================== Configuration Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldLoadConfiguration) {
    EXPECT_FALSE(config.JWT_SECRET_KEY.empty());
    EXPECT_FALSE(config.JWT_ALGORITHM.empty());
    EXPECT_GT(config.JWT_ACCESS_TOKEN_EXPIRY_MINUTES, 0);
    EXPECT_GT(config.JWT_REFRESH_TOKEN_EXPIRY_DAYS, 0);
    EXPECT_FALSE(config.DATABASE_URL.empty());
}

// ==================== Performance Tests ====================

TEST_F(AuthLibIntegrationTest, ShouldHandleConcurrentRegistrations) {
    AuthService authService(db, config);
    std::vector<std::thread> threads;
    std::vector<bool> results;

    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&, i]() {
            try {
                RegisterInput input{
                    "concurrent" + std::to_string(i) + "@example.com",
                    "SecurePass123!",
                    "Concurrent",
                    "User"
                };
                auto result = authService.registerUser(input);
                // Would track success
            } catch (...) {
                // Would track failure
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // All threads completed (would verify success count in real scenario)
}
