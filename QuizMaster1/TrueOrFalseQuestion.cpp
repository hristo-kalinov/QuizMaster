#include "TrueOrFalseQuestion.h"
#include <iostream>  
#include <cstring>

TrueOrFalseQuestion::TrueOrFalseQuestion(const char* text, bool answer, int score)
    : correctAnswer(answer), score(score) {
    if (text != nullptr)
    {
        questionText = new char[strlen(text) + 1];
        strcpy_s(questionText, strlen(text) + 1, text);
    }
}

// Destructor implementation
TrueOrFalseQuestion::~TrueOrFalseQuestion() {
    delete[] questionText;
}

// ask method implementation
void TrueOrFalseQuestion::ask() const {
    std::cout << questionText << " (True/False): ";
}

// checkAnswer method implementation
bool TrueOrFalseQuestion::checkAnswer(const char* userAnswer) const {
    return (strcmp(userAnswer, "True") == 0) == correctAnswer;
}

// saveToFile method implementation
void TrueOrFalseQuestion::saveToFile(std::ofstream& out) const {
    out << "T/F\n";
    out << questionText << "\n";
    out << (correctAnswer ? "true" : "false") << "\n";
    out << score << "\n";
}

// loadFromFile method implementation
void TrueOrFalseQuestion::loadFromFile(std::ifstream& in) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    if (questionText) delete[] questionText;

    in.getline(buffer, BUFFER_SIZE);
    questionText = new char[strlen(buffer) + 1];
    strcpy_s(questionText, strlen(buffer) + 1, buffer);

    in.getline(buffer, BUFFER_SIZE);
    correctAnswer = (strcmp(buffer, "true") == 0);

    in.getline(buffer, BUFFER_SIZE);
    score = atoi(buffer);
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