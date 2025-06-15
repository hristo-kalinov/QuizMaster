#include "Player.h"
#include <iostream>

Player::Player()
{
	xp = 0;
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