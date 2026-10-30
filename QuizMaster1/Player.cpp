#include "Player.h"
#include <iostream>
#include <cstring>

Player::Player() : User(), xp(0) {
}

Player::Player(const char* username, const int xp) : User(username, nullptr), xp(xp) {
}

Player::~Player() {
}

Player::Player(const Player& other) : User(other), xp(other.xp) {
}

Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;

    User::operator=(other);
    xp = other.xp;
    return *this;
}

void Player::viewProfile() const {}
void Player::viewChallenges() const {}
void Player::viewFinishedChallenges() const {}
void Player::viewUser(int id) const {}
void Player::startQuiz(int id, bool test, bool shuffle) {}
void Player::savePlayerToFile(const Player& player) {}
int Player::getXp() const
{
	return xp;
}

void Player::setXp(int xp)
{
	this->xp = xp;
}
