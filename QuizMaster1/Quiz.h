#pragma once
class Quiz {
protected:
    int id;
    char* name = nullptr;
    char* description = nullptr;

public:
    Quiz();
    virtual ~Quiz();

    void setId(int id);
    int getId() const;
    void setName(const char* name);
    void getName(char* buffer, int bufferSize) const;
    void setDescription(const char* description);
    void getDescription(char* buffer, int bufferSize) const;
};
