#pragma once
#include "User.h"

class Player : public User {
private:
    int xp;
public:
    Player(); 
    Player(const char* username, const int xp);
    Player(const Player& other);
    Player& operator=(const Player& other);
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
