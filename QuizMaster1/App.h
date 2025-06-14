#pragma once
#include "Player.h"

class App {
private:
    Player currentUser;
    char currentUsername[32];

public:
    App();
    void run();

private:
    void handleCommand(const char* input);
    void playQuiz(int quizId, bool test, bool shuffle);
    void viewQuizzes();
    bool login(char* username, char* password);
    void signup(char* username, char* password);
    void handleInput(const char* input, char tokens[][32], int maxTokens, int maxTokenLen, int& tokenCount);
    void createQuiz();
    int getNextQuizId();
};
