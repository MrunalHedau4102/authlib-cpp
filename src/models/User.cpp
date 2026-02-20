#include <authlib/models/User.h>
#include <sstream>

namespace authlib {

User::User()
    : id(0),
      email(""),
      passwordHash(""),
      firstName(""),
      lastName(""),
      isActive(true),
      isVerified(false),
      createdAt(std::time(nullptr)),
      updatedAt(std::time(nullptr)),
      lastLogin(0) {}

json User::toJson() const {
    return json{
        {"id", id},
        {"email", email},
        {"firstName", firstName},
        {"lastName", lastName},
        {"isActive", isActive},
        {"isVerified", isVerified},
        {"createdAt", createdAt},
        {"updatedAt", updatedAt},
        {"lastLogin", lastLogin}
    };
}

std::string User::toString() const {
    std::stringstream ss;
    ss << "<User(id=" << id << ", email=" << email << ", isActive=" << isActive << ")>";
    return ss.str();
}

} // namespace authlib
