#include <iostream>
#include <cstring>
#include <fstream>
#include "App.h"
#include "Quiz.h"
#include "User.h"
#include "Player.h"
#include "Quiz.h"
#include "TrueOrFalseQuestion.h"
#include "SingleChoiceQuestion.h"
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
    std::ifstream infile("quizzes.txt");
    if (!infile) return 1;

    int lastId = 1;

    while (true) {
        Quiz temp(lastId);
        if (!temp.loadFromFile(infile, lastId)) break;
        lastId++;
    }

    return lastId;
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
            currentUser = Player(tempUsername, 0);
            strcpy_s(currentUsername, sizeof(currentUsername), username);
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
    std::ifstream infile("quizzes.txt");
    if (!infile) {
        std::cerr << "Error opening quizzes database.\n";
        return;
    }

    std::cout << "Available quizzes:\n";

    const int LINE_SIZE = 512;
    char line[LINE_SIZE];

    while (infile.getline(line, LINE_SIZE)) {
        // Read quiz ID
        char id[64];
        strcpy_s(id, sizeof(id), line);
        id[sizeof(id) - 1] = '\0';

        // Read quiz title
        if (!infile.getline(line, LINE_SIZE)) break;
        char name[128];
        strncpy_s(name, line, sizeof(name));
        name[sizeof(name) - 1] = '\0';

        // Read number of questions
        if (!infile.getline(line, LINE_SIZE)) break;
        int numQuestions = atoi(line);

        // Read question type (but we don't need to store it for this function)
        if (!infile.getline(line, LINE_SIZE)) break;

        // Skip questions and answers lines
        for (int i = 0; i < numQuestions; ++i) {
            // Skip question text
            if (!infile.getline(line, LINE_SIZE)) break;
            // Skip answer
            if (!infile.getline(line, LINE_SIZE)) break;
        }

        // Skip points line
        if (!infile.getline(line, LINE_SIZE)) break;

        std::cout << "Quiz ID: " << id
            << ", Name: " << name
            << ", Questions: " << numQuestions << "\n";
    }
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

    int quizId = getNextQuizId();
    Quiz newQuiz(quizId, name, currentUsername);
    for (int i = 0; i < numOfQuestions; ++i) {
        cout << "Enter question " << i + 1 << " type(T/F, SC, MC, ShA, MP): ";
		char questionType[4];
		cin.getline(questionType, 4);
        if (strcmp(questionType, "T/F") != 0 && strcmp(questionType, "SC") != 0 &&
            strcmp(questionType, "MC") != 0 && strcmp(questionType, "ShA") != 0 &&
            strcmp(questionType, "MP") != 0) {
            cout << "Invalid question type. Please enter T/F, SC, MC, ShA, or MP.\n";
            i--; // Decrement i to repeat this question
            continue;
		}

        cout << "Enter question " << (i + 1) << ": ";
		char* questionText = new char[256];
        cin.getline(questionText, 256);
        int score = 0;
        if (strcmp(questionType, "T/F") == 0)
        {
            bool QuestionsAnswer = true;
            while (true) {
                cout << "Enter correct answer (True/False): ";
                char answer[6];
                cin.getline(answer, 6);

                if (strcmp(answer, "True") == 0) {
                    QuestionsAnswer = true;
                    break;
                }
                else if (strcmp(answer, "False") == 0) {
                    QuestionsAnswer  = false;
                    break;
                }
                else {
                    cout << "Invalid input. Please enter True or False.\n";
                }
            }
            TrueOrFalseQuestion* question = new TrueOrFalseQuestion(questionText, QuestionsAnswer, score);
            newQuiz.addQuestion(question);
        }

        if (strcmp(questionType, "SC") == 0)
        {
            char option1[100], option2[100], option3[100], option4[100];
            int correctOption;

            cout << "Enter option 1: ";
            cin.getline(option1, 100);

            cout << "Enter option 2: ";
            cin.getline(option2, 100);

            cout << "Enter option 3: ";
            cin.getline(option3, 100);

            cout << "Enter option 4: ";
            cin.getline(option4, 100);

            while (true) {
                cout << "Enter correct option number (1-4): ";
                cin >> correctOption;
                cin.ignore(); // To consume the newline character

                if (correctOption >= 1 && correctOption <= 4) {
                    break;
                }
                else {
                    cout << "Invalid input. Please enter a number between 1 and 4.\n";
                }
            }

            SingleChoiceQuestion* question = new SingleChoiceQuestion(
                questionText, option1, option2, option3, option4, correctOption - 1, score);
            newQuiz.addQuestion(question);
        }
        while (true)
        {
            cout << "Enter score for question " << (i + 1) << ": ";
            cin >> score;
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            if (score > 0) {
                break; // Valid score
            } else {
                cout << "Score must be a positive integer. Please try again.\n";
			}

        }

		newQuiz.setLastQuestionScore(score);


    }


	ofstream outfile("quizzes.txt", ios::out | ios::app);
    if(!outfile) {
        cerr << "No quizzes in database.\n";
        return;
	}
    newQuiz.saveToFile(outfile);
    cout << "Quiz created with ID: " << quizId << endl;
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
        
        else if (strcmp(tokens[0], "start-quiz") == 0)
        {
            //viewQuizzes();
            if(tokenCount == 2) 
            {
                int quizId = atoi(tokens[1]);
                std::ifstream infile("quizzes.txt");
				Quiz quiz(quizId);
                quiz.loadFromFile(infile, quizId);
				currentUser.setXp(currentUser.getXp() + quiz.start());
                infile.close();
				cout << "Quiz with ID " << quizId << " completed. Your current XP: " << currentUser.getXp() << endl;
            } else 
            {
                cout << "Usage: start-quiz <quiz_id>\n";
			}
        }

        else if (strcmp(tokens[0], "help") == 0) {
            cout << "Available commands:\n";
			cout << "login <username> <password> - Log in to the application\n";
            cout << "signup <username> <password> - Sign up for a new account\n";
            cout << "quizzes - View available quizzes\n";
            cout << "create-quiz - Create a new quiz\n";
            cout << "quizzes - View all quizzes\n";
			cout << "exit - Exit the application\n";
        }
        else {
            std::cout << "Unknown command: " << tokens[0] << ". Write help to get all commands and how to use them\n";
        }
    }
}