#include <iostream>
#include <string>
#include <memory>
#include "../include/authlib/authlib.h"

using namespace std;
using namespace authlib;

/**
 * Simple menu system for demonstrating AuthLib usage
 */
class AuthDemo {
private:
    unique_ptr<Config> config;
    unique_ptr<Database> database;
    unique_ptr<UserService> userService;
    unique_ptr<AuthService> authService;

public:
    AuthDemo() {
        cout << "\n=== AuthLib C++ Demo Application ===" << endl;
        cout << "Initializing..." << endl;

        try {
            // Initialize configuration
            config = make_unique<Config>();
            cout << "✓ Configuration loaded" << endl;

            // Initialize database
            database = make_unique<Database>(config->getDbUrl());
            cout << "✓ Database connected" << endl;

            // Initialize services
            userService = make_unique<UserService>(database);
            authService = make_unique<AuthService>(database, config);
            cout << "✓ Services initialized" << endl;

        } catch (const exception& e) {
            cerr << "Error initializing AuthLib: " << e.what() << endl;
            throw;
        }
    }

    void showMenu() {
        cout << "\n=== AuthLib Demo Menu ===" << endl;
        cout << "1. Register new user" << endl;
        cout << "2. Login user" << endl;
        cout << "3. Refresh access token" << endl;
        cout << "4. Get user info" << endl;
        cout << "5. Activate user" << endl;
        cout << "6. Deactivate user" << endl;
        cout << "7. Verify user" << endl;
        cout << "8. Logout (blacklist token)" << endl;
        cout << "9. Exit" << endl;
        cout << "Choose option (1-9): ";
    }

    void registerUser() {
        cout << "\n--- Register New User ---" << endl;
        
        string email, password, firstName, lastName;
        
        cout << "Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;
        cout << "First Name: ";
        cin >> firstName;
        cout << "Last Name: ";
        cin >> lastName;

        try {
            auto input = AuthService::RegisterInput{
                email, password, firstName, lastName
            };
            
            auto result = authService->registerUser(input);

            if (result.success) {
                cout << "\n✓ User registered successfully!" << endl;
                cout << "  Email: " << email << endl;
                cout << "  Access Token: " << result.accessToken.substr(0, 20) << "..." << endl;
                cout << "  Refresh Token: " << result.refreshToken.substr(0, 20) << "..." << endl;
            } else {
                cout << "\n✗ Registration failed: " << result.message << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void loginUser() {
        cout << "\n--- User Login ---" << endl;
        
        string email, password;
        
        cout << "Email: ";
        cin >> email;
        cout << "Password: ";
        cin >> password;

        try {
            auto result = authService->login(email, password);

            if (result.success) {
                cout << "\n✓ Login successful!" << endl;
                cout << "  User ID: " << result.user.id << endl;
                cout << "  Email: " << result.user.email << endl;
                cout << "  Access Token: " << result.accessToken.substr(0, 20) << "..." << endl;
                cout << "  Token expires in: " << config->getJwtExpiry() << " seconds" << endl;
            } else {
                cout << "\n✗ Login failed: " << result.message << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void refreshToken() {
        cout << "\n--- Refresh Access Token ---" << endl;
        
        string refreshToken;
        
        cout << "Enter your refresh token: ";
        cin >> refreshToken;

        try {
            auto result = authService->refreshAccessToken(refreshToken);

            if (result.success) {
                cout << "\n✓ Token refreshed successfully!" << endl;
                cout << "  New Access Token: " << result.accessToken.substr(0, 20) << "..." << endl;
                cout << "  Expires in: " << config->getJwtExpiry() << " seconds" << endl;
            } else {
                cout << "\n✗ Token refresh failed: " << result.message << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void getUserInfo() {
        cout << "\n--- Get User Info ---" << endl;
        
        string userId;
        
        cout << "Enter user ID: ";
        cin >> userId;

        try {
            auto user = userService->getUserById(userId);

            if (user) {
                cout << "\n✓ User found!" << endl;
                cout << "  ID: " << user->id << endl;
                cout << "  Email: " << user->email << endl;
                cout << "  Name: " << user->firstName << " " << user->lastName << endl;
                cout << "  Active: " << (user->isActive ? "Yes" : "No") << endl;
                cout << "  Verified: " << (user->isVerified ? "Yes" : "No") << endl;
                cout << "  Created at: " << user->createdAt << endl;
                cout << "  Last login: " << user->lastLogin << endl;
            } else {
                cout << "\n✗ User not found" << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void activateUser() {
        cout << "\n--- Activate User ---" << endl;
        
        string userId;
        
        cout << "Enter user ID: ";
        cin >> userId;

        try {
            bool success = userService->activateUser(userId);

            if (success) {
                cout << "\n✓ User activated successfully!" << endl;
            } else {
                cout << "\n✗ User not found or already active" << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void deactivateUser() {
        cout << "\n--- Deactivate User ---" << endl;
        
        string userId;
        
        cout << "Enter user ID: ";
        cin >> userId;

        try {
            bool success = userService->deactivateUser(userId);

            if (success) {
                cout << "\n✓ User deactivated successfully!" << endl;
            } else {
                cout << "\n✗ User not found or already inactive" << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void verifyUser() {
        cout << "\n--- Verify User ---" << endl;
        
        string userId;
        
        cout << "Enter user ID: ";
        cin >> userId;

        try {
            bool success = userService->verifyUser(userId);

            if (success) {
                cout << "\n✓ User verified successfully!" << endl;
            } else {
                cout << "\n✗ User not found or already verified" << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void logoutUser() {
        cout << "\n--- Logout (Blacklist Token) ---" << endl;
        
        string refreshToken;
        
        cout << "Enter refresh token to blacklist: ";
        cin >> refreshToken;

        try {
            auto result = authService->logout(refreshToken);

            if (result.success) {
                cout << "\n✓ Logout successful! Token has been blacklisted." << endl;
            } else {
                cout << "\n✗ Logout failed: " << result.message << endl;
            }

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void run() {
        int choice;

        while (true) {
            showMenu();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please try again." << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loginUser();
                    break;
                case 3:
                    refreshToken();
                    break;
                case 4:
                    getUserInfo();
                    break;
                case 5:
                    activateUser();
                    break;
                case 6:
                    deactivateUser();
                    break;
                case 7:
                    verifyUser();
                    break;
                case 8:
                    logoutUser();
                    break;
                case 9:
                    cout << "\nGoodbye!" << endl;
                    return;
                default:
                    cout << "Invalid option. Please try again." << endl;
            }
        }
    }
};

int main() {
    try {
        AuthDemo demo;
        demo.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
