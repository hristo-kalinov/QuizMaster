#pragma once
#include "Question.h"
#include <fstream>

class SingleChoiceQuestion : public Question {
private:
    char* questionText;
    char* options[4];
    int correctOption;
	int score;

public:
	SingleChoiceQuestion();
    SingleChoiceQuestion(const char* text, const char* opt1, const char* opt2,
        const char* opt3, const char* opt4, int correct, int score);
    ~SingleChoiceQuestion();

    void ask() const override;
    bool checkAnswer(const char* userAnswer) const override;
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
    int getScore() const override;
    void setScore(int score) override;
    Question* clone() const override;
};