#include "Quiz.h"
#include <cstring>
#include <cstdlib> 
#include <iostream>
#include <limits>
#include "TrueOrFalseQuestion.h"
#include "SingleChoiceQuestion.h"

Quiz::Quiz(int id)
    : id(id), name(nullptr), questions(nullptr), numOfQuestions(0), author(nullptr)
{
}

Quiz::Quiz(int id, const char* name, const char* author)
    : id(id), numOfQuestions(0), questions(nullptr), name(nullptr), author(nullptr) {
    if (name) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }
    if (author) {
        this->author = new char[strlen(author) + 1];
        strcpy(this->author, author);
    }
}

Quiz::Quiz(const Quiz& other)
    : id(other.id), numOfQuestions(other.numOfQuestions), name(nullptr), author(nullptr), questions(nullptr) {
    if (other.name) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }
    if (other.author) {
        author = new char[strlen(other.author) + 1];
        strcpy(author, other.author);
    }
    if (other.questions && numOfQuestions > 0) {
        questions = new Question*[numOfQuestions];
        for (int i = 0; i < numOfQuestions; ++i) {
            questions[i] = other.questions[i] ? other.questions[i]->clone() : nullptr;
        }
    }
}

Quiz& Quiz::operator=(const Quiz& other) {
    if (this == &other) return *this;

    for (int i = 0; i < numOfQuestions; ++i) {
        delete questions[i];
    }
    delete[] questions;
    delete[] name;
    delete[] author;

    id = other.id;
    numOfQuestions = other.numOfQuestions;
    name = nullptr;
    author = nullptr;
    questions = nullptr;

    if (other.name) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }
    if (other.author) {
        author = new char[strlen(other.author) + 1];
        strcpy(author, other.author);
    }
    if (other.questions && numOfQuestions > 0) {
        questions = new Question*[numOfQuestions];
        for (int i = 0; i < numOfQuestions; ++i) {
            questions[i] = other.questions[i] ? other.questions[i]->clone() : nullptr;
        }
    }
    return *this;
}

Quiz::~Quiz() {
    for (int i = 0; i < numOfQuestions; ++i) {
        delete questions[i];
    }
    delete[] questions; 
    delete[] name;
    delete[] author;
}

void Quiz::addQuestion(Question* question) {
    // Create a new, larger array
    Question** newArray = new Question * [numOfQuestions + 1];

    // Copy existing question pointers to the new array
    for (int i = 0; i < numOfQuestions; ++i) {
        newArray[i] = questions[i];
    }

    // Add the new question at the end
    newArray[numOfQuestions] = question;

    delete[] questions;
    questions = newArray;
    ++numOfQuestions;
}

// start method implementation
int Quiz::start() const {
    int totalScore = 0;
    for (int i = 0; i < numOfQuestions; ++i) {
        if (!questions[i]) continue;
        questions[i]->ask();
        char userInput[1024];
        std::cin.getline(userInput, 1024);

        if (questions[i]->checkAnswer(userInput)) { 
            std::cout << "Correct!\n";
            totalScore += questions[i]->getScore();
        }
        else {
            std::cout << "Incorrect.\n";
        }
    }
    std::cout << "Total Score: " << totalScore << "\n";
    return totalScore;
}

void Quiz::saveToFile(std::ofstream& out) const {
    out << id << "\n";
    out << (name ? name : "") << "\n";
    out << numOfQuestions << "\n";
    for (int i = 0; i < numOfQuestions; ++i) {
        if (questions[i]) {
            questions[i]->saveToFile(out);
        }
    }
}

bool Quiz::loadFromFile(std::ifstream& in, int targetId) {
    for (int i = 0; i < numOfQuestions; ++i) {
        delete questions[i];
    }
    delete[] questions;
    delete[] name;
    delete[] author;

    questions = nullptr;
    numOfQuestions = 0;
    name = nullptr;
    author = nullptr;

    int readId;
    while (in >> readId) {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char buffer[1024];
        if (!in.getline(buffer, 1024)) return false;

        int numQ = 0;
        if (!(in >> numQ)) return false;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (readId == targetId) {
            this->id = readId;
            name = new char[strlen(buffer) + 1];
            strcpy(name, buffer);

            numOfQuestions = numQ;
            questions = new Question * [numOfQuestions];
            for (int i = 0; i < numOfQuestions; ++i) {
                questions[i] = nullptr;
            }

            for (int i = 0; i < numOfQuestions; ++i) {
                char questionType[32];
                if (!in.getline(questionType, 32)) return false;

                if (strcmp(questionType, "T/F") == 0) {
                    questions[i] = new TrueOrFalseQuestion(nullptr, true, 0);
                }
                else if (strcmp(questionType, "SC") == 0) {
                    questions[i] = new SingleChoiceQuestion();
                }
                else {
                    std::cerr << "Unknown question type: " << questionType << std::endl;
                    return false;
                }

                questions[i]->loadFromFile(in);
            }
            return true;
        } else {
            // Skip questions for this non-target quiz
            for (int i = 0; i < numQ; ++i) {
                char questionType[32];
                if (!in.getline(questionType, 32)) return false;
                if (strcmp(questionType, "T/F") == 0) {
                    TrueOrFalseQuestion dummy(nullptr, true, 0);
                    dummy.loadFromFile(in);
                } else if (strcmp(questionType, "SC") == 0) {
                    SingleChoiceQuestion dummy;
                    dummy.loadFromFile(in);
                } else {
                    return false;
                }
            }
        }
    }

    return false;
}

void Quiz::setLastQuestionScore(int score)
{
    if (numOfQuestions > 0 && questions != nullptr && questions[numOfQuestions - 1] != nullptr) {
        questions[numOfQuestions - 1]->setScore(score);
    }
}