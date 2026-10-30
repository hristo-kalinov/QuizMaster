#include "TrueOrFalseQuestion.h"
#include <iostream>  
#include <cstring>
#include <cstdlib>

TrueOrFalseQuestion::TrueOrFalseQuestion(const char* text, bool answer, int score)
    : questionText(nullptr), correctAnswer(answer), score(score) {
    if (text != nullptr)
    {
        questionText = new char[strlen(text) + 1];
        strcpy(questionText, text);
    }
}

TrueOrFalseQuestion::TrueOrFalseQuestion(const TrueOrFalseQuestion& other)
    : questionText(nullptr), correctAnswer(other.correctAnswer), score(other.score) {
    if (other.questionText != nullptr) {
        questionText = new char[strlen(other.questionText) + 1];
        strcpy(questionText, other.questionText);
    }
}

TrueOrFalseQuestion& TrueOrFalseQuestion::operator=(const TrueOrFalseQuestion& other) {
    if (this == &other) return *this;

    delete[] questionText;
    questionText = nullptr;

    correctAnswer = other.correctAnswer;
    score = other.score;

    if (other.questionText != nullptr) {
        questionText = new char[strlen(other.questionText) + 1];
        strcpy(questionText, other.questionText);
    }
    return *this;
}

// Destructor implementation
TrueOrFalseQuestion::~TrueOrFalseQuestion() {
    delete[] questionText;
}

// ask method implementation
void TrueOrFalseQuestion::ask() const {
    if (questionText) {
        std::cout << questionText << " (True/False): ";
    }
}

// checkAnswer method implementation
bool TrueOrFalseQuestion::checkAnswer(const char* userAnswer) const {
    if (!userAnswer) return false;
    bool userBool = (strcmp(userAnswer, "True") == 0 || strcmp(userAnswer, "true") == 0 || strcmp(userAnswer, "1") == 0 || strcmp(userAnswer, "T") == 0 || strcmp(userAnswer, "t") == 0);
    return userBool == correctAnswer;
}

// saveToFile method implementation
void TrueOrFalseQuestion::saveToFile(std::ofstream& out) const {
    out << "T/F\n";
    out << (questionText ? questionText : "") << "\n";
    out << (correctAnswer ? "true" : "false") << "\n";
    out << score << "\n";
}

// loadFromFile method implementation
void TrueOrFalseQuestion::loadFromFile(std::ifstream& in) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    delete[] questionText;
    questionText = nullptr;

    if (in.getline(buffer, BUFFER_SIZE)) {
        questionText = new char[strlen(buffer) + 1];
        strcpy(questionText, buffer);
    }

    if (in.getline(buffer, BUFFER_SIZE)) {
        correctAnswer = (strcmp(buffer, "true") == 0);
    }

    if (in.getline(buffer, BUFFER_SIZE)) {
        score = atoi(buffer);
    }
}

// getScore method implementation
int TrueOrFalseQuestion::getScore() const {
    return score;
}

void TrueOrFalseQuestion::setScore(int score)
{
    this->score = score;
}

// clone method implementation
Question* TrueOrFalseQuestion::clone() const {
    return new TrueOrFalseQuestion(*this);
}