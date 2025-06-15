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

    // Delete the old array
    delete[] questions;

    // Point to the new array
    questions = newArray;

    // Increment the count of questions
    ++numOfQuestions;
}

// start method implementation
int Quiz::start() const {
    int totalScore = 0;
    for (int i = 0; i < numOfQuestions; ++i) {
        questions[i]->ask(); // Call the virtual ask method
        char userInput[1024];
        std::cin.getline(userInput, 1024); // Get user input

        if (questions[i]->checkAnswer(userInput)) { // Call the virtual checkAnswer method
            std::cout << "Correct!\n";
            totalScore += questions[i]->getScore(); // Call the virtual getScore method
        }
        else {
            std::cout << "Incorrect.\n";
        }
    }
    std::cout << "Total Score: " << totalScore << "\n";
    return totalScore;
}

// saveToFile method implementation
void Quiz::saveToFile(std::ofstream& out) const {
    out << id << "\n";
    out << name << "\n";
    out << numOfQuestions << "\n";
    for (int i = 0; i < numOfQuestions; ++i) {
        questions[i]->saveToFile(out);
    }
}

bool Quiz::loadFromFile(std::ifstream& in, int targetId) {
    // First, clear any existing data
    for (int i = 0; i < numOfQuestions; ++i) {
        delete questions[i];
    }
    delete[] questions;
    delete[] name;
    if (author) delete[] author;

    questions = nullptr;
    numOfQuestions = 0;
    name = nullptr;

    // Read the quiz ID
    int readId;
    in >> readId;
    in.ignore(); // Skip the newline after the ID

    // Check if this is the quiz we're looking for
    if (readId != targetId) {
        // Not the right quiz, rewind and return false
        in.seekg(0, std::ios::beg);
        return false;
    }

    // Read the quiz name
    char buffer[1024];
    in.getline(buffer, 1024);
    name = new char[strlen(buffer) + 1];
    strcpy_s(name, strlen(buffer) + 1, buffer);

    // Read number of questions
    in >> numOfQuestions;
    in.ignore(); // Skip the newline

    // Allocate array for questions
    questions = new Question * [numOfQuestions];

    // Read each question
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
            // Unknown question type - handle error
            std::cerr << "Unknown question type: " << questionType << std::endl;
            return false;
        }

        // Load the question data
        questions[i]->loadFromFile(in);
    }

   

    return true;
}

void Quiz::setLastQuestionScore(int score)
{
    questions[numOfQuestions-1]->setScore(score);
}