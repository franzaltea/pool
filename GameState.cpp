#include "GameState.h"

//=======================================================================
//Public Functions
//=======================================================================
//default constructor
GameState::GameState()
{
	newredballin = 0;
	newblueballin = 0;
	movablecue = false;
}

//overriden constructor
GameState::GameState(std::vector<ball> ballvector, Cueball cue, Player p1, Player p2)
{
	balls = ballvector;
	player1 = p1;
	player2 = p2;
	cueball = cue;
	movablecue = false;

	//counts the number of red and blue balls in holes
	newredballin = countredballs();
	newblueballin = countblueballs();
}

//default destructor
GameState::~GameState()
{
}

//Returns a vector of pointer of balls including cueball - cueball at 0 red at 1-7,black 8, and blue at 9-15
std::vector<ball*> GameState::allballs()
{
	std::vector<ball*> all;
	all.push_back(&cueball);
	for (unsigned int i = 0; i < balls.size(); i++)
	{
		all.push_back(&balls.at(i));
	}
	return all;
}


//sets new state of the balls in the game
void GameState::updateballcount()
{
	redballin = newredballin;
	blueballin = newblueballin;
	newredballin = countredballs();
	newblueballin = countblueballs();
}

//Returns the previous number of red balls in hole
int GameState::getoldRedball()
{
	return redballin;
}

//Returns the previous number of blue balls in holes
int GameState::getoldBlueball()
{
	return blueballin;
}

//Returns the current number of red balls in holes
int GameState::getRedball()
{
	return newredballin;
}

//Returns the current number of blue balls in holes
int GameState::getBlueball()
{
	return newblueballin;
}

//Returns whether cueball is currently movable or not
bool GameState::getMovablecue()
{
	return movablecue;
}

//Sets whether the cueball is currently movable or not
void GameState::setMovablecue(bool movable)
{
	movablecue = movable;
}

//updates the scorecount of players for this turn
void GameState::updateScore()
{
	int redballin = getRedball() - getoldRedball();
	int blueballin = getBlueball() - getoldBlueball();
	if ((player1.isTurn()) && (!player1.getNoBalltype()))
	{
		if (player1.getBalltype() == "red")
		{
			player1.setScore(redballin);
		}
		else if (player1.getBalltype() == "blue")
		{
			player1.setScore(blueballin);
		}
	}
	else if ((player2.isTurn()) && (!player1.getNoBalltype()))
	{
		if (player2.getBalltype() == "red")
		{
			player2.setScore(redballin);
		}
		else if (player2.getBalltype() == "blue")
		{
			player2.setScore(blueballin);
		}
	}
}

//setter and getter function for endgame
bool GameState::getEndgame()
{
	return endgame;
}
void GameState::setEndgame(bool theend)
{
	endgame = theend;
}

//=======================================================================
//Private Functions
//================================================================
//calculates the red balls in holes
int GameState::countredballs()
{
	std::vector<ball*> ballvector = allballs();
	int count = 0;
	for (int i = 1; i <= 7; i++)
	{
		if (ballvector[i]->getisinhole())
		{
			count += 1;
		}
	}

	return count;
}

int GameState::countblueballs()
{
	std::vector<ball*> ballvector = allballs();
	int count = 0;
	for (int i = 9; i <= 14; i++)
	{
		if (ballvector[i]->getisinhole())
		{
			count += 1;
		}
	}
	return count;
}