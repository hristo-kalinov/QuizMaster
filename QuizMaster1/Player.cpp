#include "Player.h"
#include <iostream>

Player::Player() : User(), xp(0) {
	username = new char[1];
	username[0] = '\0';
}

Player::Player(const char* username, const int xp)
{
	this->username = new char[strlen(username)+1];
	strcpy_s(this->username, strlen(username) + 1, username);
	this->xp = xp;
}

Player::~Player()
{
}

Player::Player(const Player& other) : User(other), xp(other.xp) {
    username = new char[strlen(other.username) + 1];
    strcpy_s(username, strlen(other.username) + 1, other.username);
}

Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;

    User::operator=(other);
    delete[] username;
    username = new char[strlen(other.username) + 1];
    strcpy_s(username, strlen(other.username) + 1, other.username);
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