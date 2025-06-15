#pragma once

class User {
protected:
    char* username = nullptr;
    char* password = nullptr;

public:
    User();
    User(const User& other);
    User& operator=(const User& other);
    virtual ~User();
	char* getUsername() const;

    int getId() const;
};
