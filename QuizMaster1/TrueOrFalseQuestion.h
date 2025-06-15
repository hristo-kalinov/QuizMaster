#include "Question.h"
#include <fstream>

class TrueOrFalseQuestion : public Question {
private:
    char* questionText;
    bool correctAnswer;
	int score;

public:
    TrueOrFalseQuestion(const char* text, bool answer, int score);
    ~TrueOrFalseQuestion();

    // Override methods from the base Question class
    void ask() const override;
    bool checkAnswer(const char* userAnswer) const override;
    void saveToFile(std::ofstream& out) const override;
    void loadFromFile(std::ifstream& in) override;
    int getScore() const override;
    void setScore(int score) override;
    Question* clone() const override;
};

