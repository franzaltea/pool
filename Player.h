#pragma once
#include <string>


class Player
{
public:
	//constructor
	Player();
	Player(std::string pname);
	//destructor
	~Player();
	//returns the name of the player
	std::string getName();
	//sets the name of the player
	void setName(std::string somename);
	//returns whether it is the player's turn or not
	bool isTurn();
	//sets whether it is the player's turn or not
	void setTurn(bool boolean);
	//returns whether the player is human or not
	bool isHuman();
	//sets whether the player is human or not
	void setHuman(bool ishuman);
	//returns whether there is a current ball type that it has to hit or not
	bool getNoBalltype();
	//returns the ball type that it has to hit or not
	std::string getBalltype();
	//sets the ball type that it has not hit or not
	void setBalltype(std::string type);
	//getter function for the number of balls it hit
	int getScore();
	//updates the score for the number of balls it hit
	void setScore(int score);
	//getter function for fault
	bool isFault();
	//settter function for fault
	void setFault(bool mybad);

private:
	std::string name;
	bool turn;
	bool human;
	//color or type of balls player has to hit
	bool noballtype;
	std::string balltype;
	//number of balls the player managed to score last turn
	int scoreperturn;
	//whether the player had a fault last turn or not
	bool fault;
};

