#pragma once

class User {
protected:
    char* username = nullptr;
    char* password = nullptr;

public:
    User();
    virtual ~User();

    int getId() const;
};
