#include "Player.h"

//default constructor
Player::Player()
{
	turn = false;
	human = true;
	fault = false;
	scoreperturn = 0;
	noballtype = true;
}

//overloaded constructor with player name
Player::Player(std::string pname)
{
	name = pname;
	turn = false;
	human = true;
	fault = false;
	scoreperturn = 0;
	noballtype = true;
}

//default destructor
Player::~Player()
{
}

//returns name of the player
std::string Player::getName()
{
	return name;
}

//sets the name of the player
void Player::setName(std::string somename)
{
	name = somename;
}

//returns if it is the player's turn or not
bool Player::isTurn()
{
	return turn;
}

//sets if it is the player's turn or not
void Player::setTurn(bool boolean)
{
	turn = boolean;
}

//returns whether the player is a human player or not
bool Player::isHuman()
{
	return human;
}

//sets whether the player is a human player or not
void Player::setHuman(bool ishuman)
{
	human = ishuman;
}

//returns true if no ball type has been decided yet
bool Player::getNoBalltype()
{
	return noballtype;
}


//returns the type of ball the player has to hole to win the game
std::string Player::getBalltype()
{
	return balltype;
}

//sets the type of ball the player has to hole to win the game
void Player::setBalltype(std::string type)
{
	balltype = type;
	noballtype = false;
}

//returns the number of balls the player managed to hole last turn
int Player::getScore()
{
	return scoreperturn;
}

//sets the number of balls the player managed to hole last turn
void Player::setScore(int score)
{
	scoreperturn = score;
}

//returns whether the player committed any fault last turn
bool Player::isFault()
{
	return fault;
}

//sets whether the player commtied any fault last turn
void Player::setFault(bool mybad)
{
	fault = mybad;
}