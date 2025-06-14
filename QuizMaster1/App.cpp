#include <iostream>
#include <cstring>
#include <fstream>
#include "App.h"
#include "TrueOrFalseQuiz.h"
#include "User.h"
#include "Player.h"
#include "Quiz.h"
using namespace std;

App::App()
{
	currentUsername[0] = '\0'; // Initialize currentUsername to an empty string
}
void App::handleInput(const char* input, char tokens[][32], int maxTokens, int maxTokenLen, int& tokenCount) {
    // Initialize tokens
    for (int i = 0; i < maxTokens; i++) {
        tokens[i][0] = '\0'; // Empty string
    }

    tokenCount = 0;
    int inputPos = 0;
    bool inWord = false;

    while (input[inputPos] != '\0' && tokenCount < maxTokens) {
        if (input[inputPos] != ' ' && input[inputPos] != '\t') {
            // Start of a new word
            if (!inWord) {
                inWord = true;
                int tokenPos = 0;

                // Copy characters until space or end of string
                while (input[inputPos] != '\0' &&
                    input[inputPos] != ' ' &&
                    input[inputPos] != '\t' &&
                    tokenPos < maxTokenLen - 1) {
                    tokens[tokenCount][tokenPos++] = input[inputPos++];
                }
                tokens[tokenCount][tokenPos] = '\0'; // Null-terminate
                tokenCount++;
            }
        }
        else {
            // Skip whitespace
            inWord = false;
            inputPos++;
        }
    }
}

int App::getNextQuizId() {
    std::ifstream infile("quizzes.dat");
    if (!infile) return 1;

    int lastId = 0;

    while (true) {
        TrueOrFalseQuiz temp(0, nullptr, nullptr, 0, nullptr, nullptr);
        if (!temp.loadFromFile(infile)) break;
        lastId = temp.getId();
    }

    return lastId + 1;
}


void App::signup(char* username, char* password) {
    // Check if username already exists
    ifstream infile("users.dat", ios::binary);
    User tempUser;
    char tempUsername[32];
    char tempPassword[32];

    while (infile.read((char*)&tempUser, sizeof(User))) {
        infile.read(tempUsername, 32);
        infile.read(tempPassword, 32);

        if (strcmp(tempUsername, username) == 0) {
            cout << "Username already exists. Please choose a different username.\n";
            infile.close();
            return;
        }
    }
    infile.close();

    // Create new user
    User newUser;

    // Open file in append mode
    ofstream outfile("users.dat", ios::binary | ios::app);

    if (!outfile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    // Write user data to binary file
    outfile.write((char*)&newUser, sizeof(User));
    outfile.write(username, 32);
    outfile.write(password, 32);

    outfile.close();

    cout << "User registered successfully!\n";
}

bool App::login(char* username, char* password) {
    ifstream infile("users.dat", ios::binary);

    if (!infile) {
        cerr << "Error opening users database.\n";
        return false;
    }

    User tempUser;
    char tempUsername[32];
    char tempPassword[32];

    while (infile.read((char*)&tempUser, sizeof(User))) {
        infile.read(tempUsername, 32);
        infile.read(tempPassword, 32);

        if (strcmp(tempUsername, username) == 0 && strcmp(tempPassword, password) == 0) {
            // Successful login
            currentUser = Player();
            strcpy_s(currentUsername, sizeof(currentUsername), username);
            currentUsername[sizeof(currentUsername) - 1] = '\0';
            infile.close();

            cout << "Login successful! Welcome, " << username << ".\n";
            return true;
        }
    }

    infile.close();
    cout << "Invalid username or password. Please try again.\n";
    return false;
}

void App::viewQuizzes()
{
    ifstream infile("quizzes.dat", ios::binary);
    if (!infile) {
        cerr << "Error opening quizzes database.\n";
        return;
    }
    Quiz quiz;
    char quizName[32];
    char quizDescription[256];
    cout << "Available quizzes:\n";
    while (infile.read((char*)&quiz, sizeof(Quiz)))
    {
        infile.read(quizName, 32);
        infile.read(quizDescription, 256);
        cout << "Quiz ID: " << quiz.getId() << ", Name: " << quizName
             << ", Description: " << quizDescription << "\n";
    }
	infile.close();
    
}

void App::createQuiz()
{
    cout << "Enter quiz title: ";
    char name[32];
    cin.getline(name, 32);

    cout << "Enter number of questions: ";
    int numOfQuestions;
    cin >> numOfQuestions;
    cin.ignore();

    cout << "Enter quiz description: ";
    char description[256];
    cin.getline(description, 256);

    char** questions = new char* [numOfQuestions];
    bool* answers = new bool[numOfQuestions];

    for (int i = 0; i < numOfQuestions; ++i) {
        questions[i] = new char[256];
        cout << "Enter question " << (i + 1) << ": ";
        cin.getline(questions[i], 256);

        while (true) {
            cout << "Enter correct answer (True/False): ";
            char answer[6];
            cin.getline(answer, 6);

            if (strcmp(answer, "True") == 0) {
                answers[i] = true;
                break;
            }
            else if (strcmp(answer, "False") == 0) {
                answers[i] = false;
                break;
            }
            else {
                cout << "Invalid input. Please enter True or False.\n";
            }
        }
    }

    int quizId = getNextQuizId();

    TrueOrFalseQuiz newQuiz(quizId, name, description, numOfQuestions, questions, answers);
	ofstream outfile("quizzes.dat", ios::binary | ios::app);
    if(!outfile) {
        cerr << "Error opening quizzes database for writing.\n";
        return;
	}
    newQuiz.saveToFile(outfile);
    cout << "Quiz created with ID: " << quizId << endl;

    for (int i = 0; i < numOfQuestions; ++i) {
        delete[] questions[i];
    }
    delete[] questions;
    delete[] answers;
}

void App::run() {
    std::cout << "Welcome to QuizMaster!\n";
    const int MAX_INPUT_SIZE = 256;
    const int MAX_ARGS = 10;
    const int MAX_TOKEN_LEN = 32;

    char input[MAX_INPUT_SIZE];
    char tokens[MAX_ARGS][MAX_TOKEN_LEN];

    while (true) 
    {
        std::cout << "> ";
        std::cin.getline(input, MAX_INPUT_SIZE);

        int tokenCount = 0;
        handleInput(input, tokens, MAX_ARGS, MAX_TOKEN_LEN, tokenCount);

        // Execute command based on tokens[0]
        if (tokenCount == 0) 
        {
            continue; // Empty input
        }
        else if (strcmp(tokens[0], "login") == 0) 
        {
            if (tokenCount == 3)
            {
                std::cout << "Logging in with username: " << tokens[1]
                    << ", password: " << tokens[2] << "\n";
                login(tokens[1], tokens[2]);
            }
            else {
                cout << "Usage: login <username> <password>\n";
            }
        }
        else if (strcmp(tokens[0], "signup") == 0) 
        {
            if (tokenCount == 3) 
            {
                signup(tokens[1], tokens[2]);
            }
            else 
            {
                cout << "Usage: signup <username> <password>\n";
            }
        }
        else if (strcmp(tokens[0], "quizzes") == 0)
        {
			viewQuizzes();
        }

        else if (strcmp(tokens[0], "create-quiz") == 0)
        {
			createQuiz();
        }

        else if (strcmp(tokens[0], "exit") == 0) {
            std::cout << "Goodbye!\n";
            break;
        }
        else {
            std::cout << "Unknown command: " << tokens[0] << ". Write help to get all commands and how to use them\n";
        }
    }
}