#include "User.h"
#include <cstring>

User::User()
{
	username = nullptr;
	password = nullptr;
}

User::~User() {
	delete[] username;
	delete[] password;
}

// Copy constructor
User::User(const User& other) {
    if (other.username) {
        username = new char[strlen(other.username) + 1];
        strcpy_s(username, strlen(other.username) + 1, other.username);
    }
    else {
        username = nullptr;
    }

    if (other.password) {
        password = new char[strlen(other.password) + 1];
        strcpy_s(password, strlen(other.password) + 1, other.password);
    }
    else {
        password = nullptr;
    }
}

// Assignment operator
User& User::operator=(const User& other) {
    if (this == &other) return *this;

    delete[] username;
    delete[] password;

    if (other.username) {
        username = new char[strlen(other.username) + 1];
        strcpy_s(username, strlen(other.username) + 1, other.username);
    }
    else {
        username = nullptr;
    }

    if (other.password) {
        password = new char[strlen(other.password) + 1];
        strcpy_s(password, strlen(other.password) + 1, other.password);
    }
    else {
        password = nullptr;
    }

    return *this;
}
