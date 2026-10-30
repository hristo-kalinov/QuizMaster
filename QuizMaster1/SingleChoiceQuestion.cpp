#include "Question.h"
#include "SingleChoiceQuestion.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

SingleChoiceQuestion::SingleChoiceQuestion()
    : questionText(nullptr), correctOption(0), score(0) {
    for (int i = 0; i < 4; i++) {
        options[i] = nullptr;
    }
}

SingleChoiceQuestion::SingleChoiceQuestion(const char* text, const char* opt1,
    const char* opt2, const char* opt3, const char* opt4, int correct, int score)
    : correctOption(correct), score(score), questionText(nullptr) {

    for (int i = 0; i < 4; i++) {
        options[i] = nullptr;
    }

    if (text) {
        questionText = new char[strlen(text) + 1];
        strcpy(questionText, text);
    }

    const char* opts[4] = {opt1, opt2, opt3, opt4};
    for (int i = 0; i < 4; i++) {
        if (opts[i]) {
            options[i] = new char[strlen(opts[i]) + 1];
            strcpy(options[i], opts[i]);
        }
    }
}

SingleChoiceQuestion::SingleChoiceQuestion(const SingleChoiceQuestion& other)
    : correctOption(other.correctOption), score(other.score), questionText(nullptr) {
    for (int i = 0; i < 4; i++) {
        options[i] = nullptr;
    }
    if (other.questionText) {
        questionText = new char[strlen(other.questionText) + 1];
        strcpy(questionText, other.questionText);
    }
    for (int i = 0; i < 4; i++) {
        if (other.options[i]) {
            options[i] = new char[strlen(other.options[i]) + 1];
            strcpy(options[i], other.options[i]);
        }
    }
}

SingleChoiceQuestion& SingleChoiceQuestion::operator=(const SingleChoiceQuestion& other) {
    if (this == &other) return *this;

    delete[] questionText;
    questionText = nullptr;
    for (int i = 0; i < 4; i++) {
        delete[] options[i];
        options[i] = nullptr;
    }

    correctOption = other.correctOption;
    score = other.score;

    if (other.questionText) {
        questionText = new char[strlen(other.questionText) + 1];
        strcpy(questionText, other.questionText);
    }
    for (int i = 0; i < 4; i++) {
        if (other.options[i]) {
            options[i] = new char[strlen(other.options[i]) + 1];
            strcpy(options[i], other.options[i]);
        }
    }
    return *this;
}

SingleChoiceQuestion::~SingleChoiceQuestion() {
    delete[] questionText;
    for (int i = 0; i < 4; i++) {
        delete[] options[i];
    }
}

void SingleChoiceQuestion::ask() const {
    if (questionText) {
        std::cout << questionText << "\n";
    }
    for (int i = 0; i < 4; i++) {
        if (options[i]) {
            std::cout << (i + 1) << ") " << options[i] << "\n";
        }
    }
    std::cout << "Your choice (1-4): ";
}

bool SingleChoiceQuestion::checkAnswer(const char* userAnswer) const {
    if (!userAnswer) return false;
    int answer = atoi(userAnswer) - 1;
    return answer == correctOption;
}

void SingleChoiceQuestion::saveToFile(std::ofstream& out) const {
    out << "SC\n";
    out << (questionText ? questionText : "") << "\n";
    for (int i = 0; i < 4; i++) {
        out << (options[i] ? options[i] : "") << "\n";
    }
    out << correctOption << "\n";
    out << score << "\n";
}

void SingleChoiceQuestion::loadFromFile(std::ifstream& in) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    delete[] questionText;
    questionText = nullptr;
    for (int i = 0; i < 4; i++) {
        delete[] options[i];
        options[i] = nullptr;
    }

    if (in.getline(buffer, BUFFER_SIZE)) {
        questionText = new char[strlen(buffer) + 1];
        strcpy(questionText, buffer);
    }

    for (int i = 0; i < 4; i++) {
        if (in.getline(buffer, BUFFER_SIZE)) {
            options[i] = new char[strlen(buffer) + 1];
            strcpy(options[i], buffer);
        }
    }

    if (in.getline(buffer, BUFFER_SIZE)) {
        correctOption = atoi(buffer);
    }

    if (in.getline(buffer, BUFFER_SIZE)) {
        score = atoi(buffer);
    }
}

int SingleChoiceQuestion::getScore() const {
    return score;
}

void SingleChoiceQuestion::setScore(int score)
{
    this->score = score;
}
Question* SingleChoiceQuestion::clone() const {
    return new SingleChoiceQuestion(*this);
}