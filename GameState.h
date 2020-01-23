#pragma once
#include <vector>
#include "customsprite.h"
#include "Player.h"

/*Class for current state of the game*/
class GameState
{
public:
	GameState();
	GameState(std::vector<ball> ballvector, Cueball cue, Player p1, Player p2);
	~GameState();

	//Variables stored in GameState
	std::vector<ball> balls;
	Cueball cueball;
	Player player1;
	Player player2;

	//Returns a vector pointer of balls including cueball - cueball at 0 red at 1-7,black 8, and blue at 9-15
	std::vector<ball*> allballs();


	//function used to update current ballcount
	void updateballcount();

	int getoldRedball();
	int getoldBlueball();
	int getRedball();
	int getBlueball();

	bool getMovablecue();
	void setMovablecue(bool movable);

	//updates the scorecount of players for this turn
	void updateScore();

	//setter and getter function for endgame
	bool getEndgame();
	void setEndgame(bool theend);

private:
	int redballin;
	int blueballin;
	int newredballin;
	int newblueballin;
	bool movablecue;
	bool endgame;

	//private function used for updating ballcount
	int countredballs();
	int countblueballs();
};

