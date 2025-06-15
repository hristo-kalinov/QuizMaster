#ifndef QUIZ_H
#define QUIZ_H
#include "Question.h" 
#include <fstream>
#include <iostream>

class Quiz {
private:
    int id;
    char* name;
    Question** questions; // Array of pointers to base Question objects
    int numOfQuestions;
    char* author;

public:
    Quiz(int id);
    Quiz(int id, const char* name, const char* author);
    ~Quiz();

    void addQuestion(Question * question);

    int start() const;

    void saveToFile(std::ofstream& out) const;
    void setLastQuestionScore(int score);
    bool loadFromFile(std::ifstream& in, int targetId);

};
#endif