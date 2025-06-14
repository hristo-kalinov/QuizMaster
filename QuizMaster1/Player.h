#pragma once
#include "User.h"

class Player : public User {
private:
    int xp;
public:
    Player();
    void viewProfile() const;
    void viewChallenges() const;
    void viewFinishedChallenges() const;
    void viewUser(int id) const;
    void startQuiz(int id, bool test, bool shuffle);
    void savePlayerToFile(const Player& player);
};
