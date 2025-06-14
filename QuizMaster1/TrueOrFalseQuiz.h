#pragma once
#include "Quiz.h"
#include <fstream>

class TrueOrFalseQuiz : public Quiz {
private:
    int numOfQuestions;
    char** questions = nullptr;
    bool* answers = nullptr;

public:
    TrueOrFalseQuiz(int id, char* name, char* description, int numOfQuestions, char** questions, bool* answers);
    ~TrueOrFalseQuiz();

    void saveToFile(std::ofstream& out) const;
    bool loadFromFile(std::ifstream& in);
};
