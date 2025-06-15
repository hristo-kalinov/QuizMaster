#include "Question.h"
#include "SingleChoiceQuestion.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

SingleChoiceQuestion::SingleChoiceQuestion()
    : questionText(nullptr), correctOption(0), score(0) {
}
// Constructor implementation
SingleChoiceQuestion::SingleChoiceQuestion(const char* text, const char* opt1,
    const char* opt2, const char* opt3, const char* opt4, int correct, int score)
    : correctOption(correct), score(score) {

    questionText = new char[strlen(text) + 1];
    strcpy_s(questionText, strlen(text) + 1, text);

    options[0] = new char[strlen(opt1) + 1];
    strcpy_s(options[0], strlen(opt1) + 1, opt1);

    options[1] = new char[strlen(opt2) + 1];
    strcpy_s(options[1], strlen(opt2) + 1, opt2);

    options[2] = new char[strlen(opt3) + 1];
    strcpy_s(options[2], strlen(opt3) + 1, opt3);

    options[3] = new char[strlen(opt4) + 1];
    strcpy_s(options[3], strlen(opt4) + 1, opt4);
}

// Destructor implementation
SingleChoiceQuestion::~SingleChoiceQuestion() {
    delete[] questionText;
    for (int i = 0; i < 4; i++) {
        delete[] options[i];
    }
}

// ask method implementation
void SingleChoiceQuestion::ask() const {
    std::cout << questionText << "\n";
    for (int i = 0; i < 4; i++) {
        std::cout << (i + 1) << ") " << options[i] << "\n";
    }
    std::cout << "Your choice (1-4): ";
}

// checkAnswer method implementation
bool SingleChoiceQuestion::checkAnswer(const char* userAnswer) const {
    int answer = atoi(userAnswer) - 1;
    return answer == correctOption;
}

// saveToFile method implementation
void SingleChoiceQuestion::saveToFile(std::ofstream& out) const {
    out << "SC\n";
    out << questionText << "\n";
    for (int i = 0; i < 4; i++) {
        out << options[i] << "\n";
    }
    out << correctOption << "\n";
    out << score << "\n";
}

// loadFromFile method implementation
void SingleChoiceQuestion::loadFromFile(std::ifstream& in) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    if (questionText) delete[] questionText;
    for (int i = 0; i < 4; i++) {
        if (options[i]) delete[] options[i];
    }

    in.getline(buffer, BUFFER_SIZE);
    questionText = new char[strlen(buffer) + 1];
    strcpy_s(questionText, strlen(buffer) + 1, buffer);

    for (int i = 0; i < 4; i++) {
        in.getline(buffer, BUFFER_SIZE);
        options[i] = new char[strlen(buffer) + 1];
        strcpy_s(options[i], strlen(buffer) + 1, buffer);
    }

    in.getline(buffer, BUFFER_SIZE);
    correctOption = atoi(buffer);

    in.getline(buffer, BUFFER_SIZE);
    score = atoi(buffer);
}

// getScore method implementation
int SingleChoiceQuestion::getScore() const {
    return score;
}

void SingleChoiceQuestion::setScore(int score)
{
    this->score = score;
}
// clone method implementation
Question* SingleChoiceQuestion::clone() const {
    return new SingleChoiceQuestion(*this);
}