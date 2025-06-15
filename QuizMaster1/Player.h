#pragma once
#include "User.h"

class Player : public User {
private:
    int xp;
public:
    Player(); // Default constructor
    Player(const char* username, const int xp);
    Player(const Player& other); // Copy constructor
    Player& operator=(const Player& other); // Copy assignment
    ~Player();
    void viewProfile() const;
    void viewChallenges() const;
    void viewFinishedChallenges() const;
    void viewUser(int id) const;
    void startQuiz(int id, bool test, bool shuffle);
    void savePlayerToFile(const Player& player);
    int getXp() const;
	void setXp(int xpValue);
};
