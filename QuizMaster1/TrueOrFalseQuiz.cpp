#include "TrueOrFalseQuiz.h"
#include "Player.h"
#include <cstring>
#include <iostream>
TrueOrFalseQuiz::TrueOrFalseQuiz(int id, char* name, int numOfQuestions, char** questions, bool* answers, int* questionScore)
    : Quiz()  // Initialize base class
{
    // Set base class members
    setId(id);
    setName(name);

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

    if (questionScore != nullptr && this->numOfQuestions > 0)
    {
        this->questionScore = new int[this->numOfQuestions];
        for (int i = 0; i < this->numOfQuestions; ++i)
        {
            this->questionScore[i] = questionScore[i];
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
    delete[] questionScore;
}
void TrueOrFalseQuiz::saveToFile(std::ofstream& out) const {
    // Save id
    out << id << "\n";

    // Save name
    out << name << "\n";

    // Save numOfQuestions
    out << numOfQuestions << "\n";

    // Save questions and answers
    for (int i = 0; i < numOfQuestions; ++i) {
        out << questions[i] << "\n";
        out << (answers[i] ? "true" : "false") << "\n";
		out << questionScore[i] << "\n";  // Save question score
    }
}

bool TrueOrFalseQuiz::loadFromFile(std::ifstream& in, int desiredId) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (!in.eof()) {
        std::streampos startPos = in.tellg();

        // Read ID
        if (!in.getline(buffer, BUFFER_SIZE)) break;
        int fileId = std::atoi(buffer);

        // Read name
        if (!in.getline(buffer, BUFFER_SIZE)) break;
        char* tempName = new char[std::strlen(buffer) + 1];
        strcpy_s(tempName, std::strlen(buffer) + 1, buffer);

        // Read number of questions
        if (!in.getline(buffer, BUFFER_SIZE)) {
            delete[] tempName;
            break;
        }
        int tempNumQuestions = std::atoi(buffer);

        // Temp storage for questions, answers, and scores
        char** tempQuestions = nullptr;
        bool* tempAnswers = nullptr;
        int* tempQuestionScores = nullptr;
        if (tempNumQuestions > 0) {
            tempQuestions = new char* [tempNumQuestions];
            tempAnswers = new bool[tempNumQuestions];
            tempQuestionScores = new int[tempNumQuestions];

            bool failed = false;
            for (int i = 0; i < tempNumQuestions; ++i) {
                // Read question
                if (!in.getline(buffer, BUFFER_SIZE)) { failed = true; break; }
                tempQuestions[i] = new char[std::strlen(buffer) + 1];
                strcpy_s(tempQuestions[i], std::strlen(buffer) + 1, buffer);

                // Read answer
                if (!in.getline(buffer, BUFFER_SIZE)) { failed = true; break; }
                tempAnswers[i] = (std::strcmp(buffer, "true") == 0);

                // Read question score
                if (!in.getline(buffer, BUFFER_SIZE)) { failed = true; break; }
                tempQuestionScores[i] = std::atoi(buffer);
            }

            if (failed) {
                for (int i = 0; i < tempNumQuestions; ++i) delete[] tempQuestions[i];
                delete[] tempQuestions;
                delete[] tempAnswers;
                delete[] tempQuestionScores;
                delete[] tempName;
                break;
            }
        }

        // If this is the one, assign to current object
        if (fileId == desiredId) {
            // Clean up old data
            if (questions) {
                for (int i = 0; i < numOfQuestions; ++i) delete[] questions[i];
                delete[] questions;
            }
            delete[] answers;
            delete[] questionScore;
            delete[] name;

            id = fileId;
            name = tempName;
            numOfQuestions = tempNumQuestions;
            questions = tempQuestions;
            answers = tempAnswers;
            questionScore = tempQuestionScores;

            return true;
        }

        // If not matched, clean up and keep scanning
        delete[] tempName;
        for (int i = 0; i < tempNumQuestions; ++i) delete[] tempQuestions[i];
        delete[] tempQuestions;
        delete[] tempAnswers;
        delete[] tempQuestionScores;
    }

    return false;
}

int TrueOrFalseQuiz::startQuiz() const {
    int totalQuizScore = 0;
    for (int i = 0; i < numOfQuestions; i++)
    {
		totalQuizScore += questionScore[i];
    }
    int totalScore = 0;
    for (int i = 0; i < numOfQuestions; ++i) {
        std::cout << "Question " << (i + 1) << ": " << questions[i] << " (True/False): ";
        char answer[6]; // "True" or "False"
        std::cin >> answer;
        bool userAnswer = (std::strcmp(answer, "True") == 0);
        if (userAnswer == answers[i]) {
            std::cout << "Correct! You earned " << questionScore[i] << " points.\n";
            totalScore += questionScore[i];
        } else {
            std::cout << "Incorrect! The correct answer was: " << (answers[i] ? "True" : "False") << "\n";
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

	std::cout << "Quiz finished! Your score is: " << totalScore << "/" << totalQuizScore << "\n";
    return totalScore;
}