#include <fstream>
#pragma once
class Question {
public:
    virtual ~Question() {}
    virtual void ask() const = 0;
    virtual bool checkAnswer(const char* userAnswer) const = 0;
    virtual void saveToFile(std::ofstream& out) const = 0;
    virtual void loadFromFile(std::ifstream& in) = 0;
    virtual int getScore() const = 0;
    virtual void setScore(int score) = 0;
    virtual Question* clone() const = 0;
};
