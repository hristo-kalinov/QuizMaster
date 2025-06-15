#include "Quiz.h"
#include <cstring>
#include <cstdlib> 
#include <iostream>
#include "TrueOrFalseQuestion.h"
#include "SingleChoiceQuestion.h"
Quiz::Quiz(int id)
{
    this->id = id;
}
Quiz::Quiz(int id, const char* name, const char* author)
    : id(id), numOfQuestions(0), questions(nullptr) {
    this->name = new char[strlen(name) + 1];
    strcpy_s(this->name, strlen(name) + 1, name);
    this->author = new char[strlen(author) + 1];
	strcpy_s(this->author, strlen(author)+1, author);
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
    out << name << "\n";
    out << numOfQuestions << "\n";
    for (int i = 0; i < numOfQuestions; ++i) {
        questions[i]->saveToFile(out);
    }
}

bool Quiz::loadFromFile(std::ifstream& in, int targetId) {
    for (int i = 0; i < numOfQuestions; ++i) {
        delete questions[i];
    }
    delete[] questions;
    delete[] name;
    if (author) delete[] author;

    questions = nullptr;
    numOfQuestions = 0;
    name = nullptr;

    int readId;
    in >> readId;
    in.ignore();

    if (readId != targetId) {
        in.seekg(0, std::ios::beg);
        return false;
    }

    char buffer[1024];
    in.getline(buffer, 1024);
    name = new char[strlen(buffer) + 1];
    strcpy_s(name, strlen(buffer) + 1, buffer);

    in >> numOfQuestions;
    in.ignore();

    questions = new Question * [numOfQuestions];

    for (int i = 0; i < numOfQuestions; ++i) {
        char questionType[32];
        in.getline(questionType, 32);

        if (strcmp(questionType, "T/F") == 0) {
            questions[i] = new TrueOrFalseQuestion(nullptr, true, 0);
        }

        else if (strcmp(questionType, "SC") == 0){
            questions[i] = new SingleChoiceQuestion();
        }
        else {
            std::cerr << "Unknown question type: " << questionType << std::endl;
            return false;
        }

        questions[i]->loadFromFile(in);
    }

    return true;
}

void Quiz::setLastQuestionScore(int score)
{
    questions[numOfQuestions-1]->setScore(score);
}