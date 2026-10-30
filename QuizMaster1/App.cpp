#include <iostream>
#include <cstring>
#include <fstream>
#include <limits>
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

    while (input[inputPos] != '\0' && tokenCount < maxTokens) {
        if (input[inputPos] != ' ' && input[inputPos] != '\t') {
            int tokenPos = 0;

            // Copy characters until space or end of string
            while (input[inputPos] != '\0' &&
                input[inputPos] != ' ' &&
                input[inputPos] != '\t') {
                if (tokenPos < maxTokenLen - 1) {
                    tokens[tokenCount][tokenPos++] = input[inputPos];
                }
                inputPos++;
            }
            tokens[tokenCount][tokenPos] = '\0'; // Null-terminate
            tokenCount++;
        }
        else {
            // Skip whitespace
            inputPos++;
        }
    }
}

int App::getNextQuizId() {
    std::ifstream infile("quizzes.txt");
    if (!infile) return 1;

    int maxId = 0;
    int readId;

    while (infile >> readId) {
        if (readId > maxId) maxId = readId;
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char buffer[1024];
        if (!infile.getline(buffer, 1024)) break;

        int numQ = 0;
        if (!(infile >> numQ)) break;
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int i = 0; i < numQ; ++i) {
            char questionType[32];
            if (!infile.getline(questionType, 32)) break;
            if (strcmp(questionType, "T/F") == 0) {
                TrueOrFalseQuestion dummy(nullptr, true, 0);
                dummy.loadFromFile(infile);
            } else if (strcmp(questionType, "SC") == 0) {
                SingleChoiceQuestion dummy;
                dummy.loadFromFile(infile);
            }
        }
    }

    infile.close();
    return maxId + 1;
}

void App::signup(char* username, char* password) {
    // Check if username already exists
    ifstream infile("users.dat", ios::binary);
    char dummyUser[sizeof(User)];
    char tempUsername[32];
    char tempPassword[32];

    if (infile) {
        while (infile.read(dummyUser, sizeof(dummyUser))) {
            infile.read(tempUsername, 32);
            tempUsername[31] = '\0';
            infile.read(tempPassword, 32);
            tempPassword[31] = '\0';

            if (strcmp(tempUsername, username) == 0) {
                cout << "Username already exists. Please choose a different username.\n";
                infile.close();
                return;
            }
        }
        infile.close();
    }

    // Open file in append mode
    ofstream outfile("users.dat", ios::binary | ios::app);

    if (!outfile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    // Write user data to binary file
    char dummy[sizeof(User)] = {0};
    char safeUser[32] = {0};
    char safePass[32] = {0};
    strncpy(safeUser, username, 31);
    strncpy(safePass, password, 31);

    outfile.write(dummy, sizeof(dummy));
    outfile.write(safeUser, 32);
    outfile.write(safePass, 32);

    outfile.close();

    cout << "User registered successfully!\n";
}

bool App::login(char* username, char* password) {
    ifstream infile("users.dat", ios::binary);

    if (!infile) {
        cerr << "Error opening users database.\n";
        return false;
    }

    char dummyUser[sizeof(User)];
    char tempUsername[32];
    char tempPassword[32];

    while (infile.read(dummyUser, sizeof(dummyUser))) {
        infile.read(tempUsername, 32);
        tempUsername[31] = '\0';
        infile.read(tempPassword, 32);
        tempPassword[31] = '\0';

        if (strcmp(tempUsername, username) == 0 && strcmp(tempPassword, password) == 0) {
            currentUser = Player(tempUsername, 0);
            strcpy(currentUsername, username);
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

    int id;
    while (infile >> id) {
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char name[128];
        if (!infile.getline(name, sizeof(name))) break;

        int numQuestions = 0;
        if (!(infile >> numQuestions)) break;
        infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Quiz ID: " << id
            << ", Name: " << name
            << ", Questions: " << numQuestions << "\n";

        for (int i = 0; i < numQuestions; ++i) {
            char questionType[32];
            if (!infile.getline(questionType, sizeof(questionType))) break;
            if (strcmp(questionType, "T/F") == 0) {
                TrueOrFalseQuestion dummy(nullptr, true, 0);
                dummy.loadFromFile(infile);
            } else if (strcmp(questionType, "SC") == 0) {
                SingleChoiceQuestion dummy;
                dummy.loadFromFile(infile);
            }
        }
    }
    infile.close();
}

void App::createQuiz()
{
    cout << "Enter quiz title: ";
    char name[32];
    cin.getline(name, 32);

    int numOfQuestions = 0;
    while (true) {
        cout << "Enter number of questions: ";
        if (cin >> numOfQuestions && numOfQuestions > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cout << "Invalid number of questions. Please enter a positive integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int quizId = getNextQuizId();
    Quiz newQuiz(quizId, name, currentUsername);
    for (int i = 0; i < numOfQuestions; ++i) {
        char questionType[16];
        while (true) {
            cout << "Enter question " << (i + 1) << " type (T/F, SC): ";
            cin.getline(questionType, sizeof(questionType));
            if (strcmp(questionType, "T/F") == 0 || strcmp(questionType, "SC") == 0) {
                break;
            }
            cout << "Invalid question type. Please enter T/F or SC.\n";
        }

        cout << "Enter question " << (i + 1) << ": ";
        char questionText[256];
        cin.getline(questionText, sizeof(questionText));
        int score = 0;

        if (strcmp(questionType, "T/F") == 0)
        {
            bool QuestionsAnswer = true;
            while (true) {
                cout << "Enter correct answer (True/False): ";
                char answer[16];
                cin.getline(answer, sizeof(answer));

                if (strcmp(answer, "True") == 0 || strcmp(answer, "true") == 0) {
                    QuestionsAnswer = true;
                    break;
                }
                else if (strcmp(answer, "False") == 0 || strcmp(answer, "false") == 0) {
                    QuestionsAnswer = false;
                    break;
                }
                else {
                    cout << "Invalid input. Please enter True or False.\n";
                }
            }
            TrueOrFalseQuestion* question = new TrueOrFalseQuestion(questionText, QuestionsAnswer, score);
            newQuiz.addQuestion(question);
        }
        else if (strcmp(questionType, "SC") == 0)
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
                if (cin >> correctOption && correctOption >= 1 && correctOption <= 4) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                else {
                    cout << "Invalid input. Please enter a number between 1 and 4.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            SingleChoiceQuestion* question = new SingleChoiceQuestion(
                questionText, option1, option2, option3, option4, correctOption - 1, score);
            newQuiz.addQuestion(question);
        }

        while (true)
        {
            cout << "Enter score for question " << (i + 1) << ": ";
            if (cin >> score && score > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Score must be a positive integer. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        newQuiz.setLastQuestionScore(score);
    }

    ofstream outfile("quizzes.txt", ios::out | ios::app);
    if (!outfile) {
        cerr << "Error opening quizzes file for writing.\n";
        return;
    }
    newQuiz.saveToFile(outfile);
    outfile.close();
    cout << "Quiz created with ID: " << quizId << endl;
}
bool App::isLoggedIn() const {
    bool loggedIn = (currentUsername[0] != '\0');
    if (!loggedIn)
    {
        cout << "You must be logged in to perform this action. Please log in or sign up.\n";
    }
    return loggedIn;
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
            if (!isLoggedIn())
                continue;
            viewQuizzes();
        }

        else if (strcmp(tokens[0], "create-quiz") == 0)
        {
            if (!isLoggedIn())
                continue;
            createQuiz();
        }

        else if (strcmp(tokens[0], "exit") == 0) {
            std::cout << "Goodbye!\n";
            break;
        }
        
        else if (strcmp(tokens[0], "start-quiz") == 0)
        {
            if (!isLoggedIn())
                continue;

            if (tokenCount == 2) 
            {
                int quizId = atoi(tokens[1]);
                std::ifstream infile("quizzes.txt");
                if (!infile) {
                    cout << "Error opening quizzes database.\n";
                } else {
                    Quiz quiz(quizId);
                    if (quiz.loadFromFile(infile, quizId)) {
                        currentUser.setXp(currentUser.getXp() + quiz.start());
                        cout << "Quiz with ID " << quizId << " completed. Your current XP: " << currentUser.getXp() << endl;
                    } else {
                        cout << "Quiz with ID " << quizId << " not found.\n";
                    }
                    infile.close();
                }
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
            cout << "start-quiz <quiz_id> - Start a quiz\n";
            cout << "exit - Exit the application\n";
        }
        else {
            std::cout << "Unknown command: " << tokens[0] << ". Write help to get all commands and how to use them\n";
        }
    }
}