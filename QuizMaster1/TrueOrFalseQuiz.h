#pragma once
#include "Quiz.h"
#include <fstream>
class TrueOrFalseQuiz : public Quiz {
private:
    int numOfQuestions;
    char** questions = nullptr;
    bool* answers = nullptr;
    int* questionScore = nullptr;

public:
    TrueOrFalseQuiz(int id, char* name, int numOfQuestions, char** questions, bool* answers, int* questionScore);
    ~TrueOrFalseQuiz();

    void saveToFile(std::ofstream& out) const;
    bool loadFromFile(std::ifstream& in, int id);
	int startQuiz() const;
};
