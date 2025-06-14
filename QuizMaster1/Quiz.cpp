#include "Quiz.h"
#include <cstring>

Quiz::Quiz() : id(0), name(nullptr), description(nullptr) {}

Quiz::~Quiz() {
    if (name) {
        delete[] name;
        name = nullptr;
    }
    if (description) {
        delete[] description;
        description = nullptr;
    }
}

void Quiz::setId(int id) {
    this->id = id;
}

int Quiz::getId() const {
    return id;
}

void Quiz::setName(const char* name) {
    if (this->name) {
        delete[] this->name;
        this->name = nullptr;
    }
    if (name) {
        this->name = new char[strlen(name) + 1];
        strcpy_s(this->name, strlen(name) + 1, name);
    }
}

void Quiz::getName(char* buffer, int bufferSize) const {
    if (bufferSize > 0 && name)
        strcpy_s(buffer, bufferSize, name);
    else if (bufferSize > 0)
        buffer[0] = '\0';
}

void Quiz::setDescription(const char* description) {
    if (this->description) {
        delete[] this->description;
        this->description = nullptr;
    }
    if (description) {
        this->description = new char[strlen(description) + 1];
        strcpy_s(this->description, strlen(description) + 1, description);
    }
}

void Quiz::getDescription(char* buffer, int bufferSize) const {
    if (bufferSize > 0 && description)
        strcpy_s(buffer, bufferSize, description);
    else if (bufferSize > 0)
        buffer[0] = '\0';
}
