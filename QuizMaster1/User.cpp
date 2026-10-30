#include "User.h"
#include <cstring>

User::User()
    : username(nullptr), password(nullptr)
{
}

User::User(const char* username, const char* password)
    : username(nullptr), password(nullptr)
{
    if (username) {
        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    }
    if (password) {
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
    }
}

User::~User() {
    delete[] username;
    delete[] password;
}

User::User(const User& other)
    : username(nullptr), password(nullptr) {
    if (other.username) {
        username = new char[strlen(other.username) + 1];
        strcpy(username, other.username);
    }
    if (other.password) {
        password = new char[strlen(other.password) + 1];
        strcpy(password, other.password);
    }
}

// Assignment operator
User& User::operator=(const User& other) {
    if (this == &other) return *this;

    delete[] username;
    delete[] password;
    username = nullptr;
    password = nullptr;

    if (other.username) {
        username = new char[strlen(other.username) + 1];
        strcpy(username, other.username);
    }

    if (other.password) {
        password = new char[strlen(other.password) + 1];
        strcpy(password, other.password);
    }

    return *this;
}

char* User::getUsername() const {
    return username;
}

int User::getId() const {
    return 0;
}

