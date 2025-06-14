#include "TrueOrFalseQuiz.h"
#include <cstring>
TrueOrFalseQuiz::TrueOrFalseQuiz(int id, char* name, char* description, int numOfQuestions, char** questions, bool* answers)
    : Quiz()  // Initialize base class
{
    // Set base class members
    setId(id);
    setName(name);
    setDescription(description);

    // Initialize derived class members
    this->numOfQuestions = (numOfQuestions > 0) ? numOfQuestions : 0;
    this->questions = nullptr;
    this->answers = nullptr;

    // Handle questions array
    if (questions != nullptr && this->numOfQuestions > 0)
    {
        this->questions = new char* [this->numOfQuestions];
        for (int i = 0; i < this->numOfQuestions; ++i)
        {
            if (questions[i] != nullptr)
            {
                this->questions[i] = new char[strlen(questions[i]) + 1];
                strcpy_s(this->questions[i], strlen(questions[i]) + 1, questions[i]);
            }
            else
            {
                this->questions[i] = new char[1];
                this->questions[i][0] = '\0';
            }
        }
    }

    // Handle answers array
    if (answers != nullptr && this->numOfQuestions > 0)
    {
        this->answers = new bool[this->numOfQuestions];
        for (int i = 0; i < this->numOfQuestions; ++i)
        {
            this->answers[i] = answers[i];
        }
    }
}

TrueOrFalseQuiz::~TrueOrFalseQuiz() {
    if (questions != nullptr) {
        for (int i = 0; i < numOfQuestions; ++i) {
            delete[] questions[i];
        }
        delete[] questions;
    }
    delete[] answers;
}
void TrueOrFalseQuiz::saveToFile(std::ofstream& out) const {
    // Save id
    out << id << "\n";

    // Save name
    out << name << "\n";

    // Save description
    out << description << "\n";

    // Save numOfQuestions
    out << numOfQuestions << "\n";

    // Save questions and answers
    for (int i = 0; i < numOfQuestions; ++i) {
        out << questions[i] << "\n";
        out << (answers[i] ? "true" : "false") << "\n";
    }
}

bool TrueOrFalseQuiz::loadFromFile(std::ifstream& in) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // Load ID
    if (!in.getline(buffer, BUFFER_SIZE)) return false;
    id = std::atoi(buffer);

    // Load name
    if (!in.getline(buffer, BUFFER_SIZE)) return false;
    delete[] name;
    name = new char[std::strlen(buffer) + 1];
    strcpy_s(name, std::strlen(buffer) + 1, buffer);

    // Load description
    if (!in.getline(buffer, BUFFER_SIZE)) return false;
    delete[] description;
    description = new char[std::strlen(buffer) + 1];
    strcpy_s(description, std::strlen(buffer) + 1, buffer);

    // Load number of questions
    if (!in.getline(buffer, BUFFER_SIZE)) return false;

    int oldCount = numOfQuestions;
    numOfQuestions = std::atoi(buffer);

    // Clean up old questions and answers
    if (questions) {
        for (int i = 0; i < oldCount; ++i) {
            delete[] questions[i];
        }
        delete[] questions;
        questions = nullptr;
    }
    delete[] answers;
    answers = nullptr;

    if (numOfQuestions <= 0) return true;

    questions = new char* [numOfQuestions];
    answers = new bool[numOfQuestions];

    for (int i = 0; i < numOfQuestions; ++i) {
        // Load question
        if (!in.getline(buffer, BUFFER_SIZE)) return false;
        questions[i] = new char[std::strlen(buffer) + 1];
        strcpy_s(questions[i], std::strlen(buffer) + 1, buffer);

        // Load answer
        if (!in.getline(buffer, BUFFER_SIZE)) return false;
        answers[i] = (std::strcmp(buffer, "true") == 0);
    }

    return true;
}
