#include "gamerules.h"


/*initializes the position of vector of pool balls - note that this requires them all to be the same radius*/
void initialposition(std::vector <ball*> balls, stick *poolstick)
{
	float r = balls.at(0)->getRadius();
	//creates an array of randomized positions for initial position
	std::vector<sf::Vector2f> iposition;
	iposition.push_back(sf::Vector2f(200, 200 + 2 * (r+0.01)));
	iposition.push_back(sf::Vector2f(200, 200 - 2 * r));
	iposition.push_back(sf::Vector2f(200 + sqrt(pow(2 * r, 2) - pow(r, 2)) + 0.01, 200 + (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 + sqrt(pow(2 * r, 2) - pow(r, 2)) + 0.01, 200 - (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.01, 200 + (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.01, 200 - (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.01, 200 + 3 * (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.01, 200 - 3 * (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.02, 200));
	iposition.push_back(sf::Vector2f(200 - 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.02, 200 + 2 * (r + 0.01)));
	iposition.push_back(sf::Vector2f(200 - 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.02, 200 - 2 * (r + 0.01)));

	//sets initial position of the balls by setting specific positions for certain ball and randomizing the rest
	int rnum;
	/* initialize random seed: */
	srand((unsigned int)time(NULL));
	for (int i = 0; i <= 15; i++)
	{
		if (i == 0)
		{
			balls.at(i)->setPosition(600.03, 200);
		}
		else if (i == 8)
		{
			balls.at(i)->setPosition(200, 200);
		}
		else if (i == 1)
		{
			balls.at(i)->setPosition(200 + 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) + 0.02, 200);
		}
		else if (i == 2)
		{
			balls.at(i)->setPosition(200 - 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.02, 200 + 4 * (r + 0.01));
		}
		else if (i == 9)
		{
			balls.at(i)->setPosition(200 - 2 * sqrt(pow(2 * r, 2) - pow(r, 2)) - 0.02, 200 - 4 * (r + 0.01));
		}
		else
		{
			//randomizes which position the ball gets
			rnum = rand() % iposition.size();
			balls.at(i)->setPosition(iposition[rnum]);
			iposition.erase(iposition.begin()+rnum);
		}
	}

	//initializes the position of the ppolstick to be 10pixels to the right of cueball
	poolstick->rotate(-90);
	poolstick->setPosition(625, 200);
}

//Redraws the poolstick based on user input(allows user to rotate it to follow mouse movement)
void stickmovement(sf::Window *window,stick *poolstick, ball *cueball)
{
	//gets position of the mouse relative to window
	sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
	//converts to float
	sf::Vector2f mousePos((float)localPosition.x, (float)localPosition.y);
	
	//finds the displacement vector from mouse current position of cueball
	sf::Vector2f stickdirection;
	sf::Vector2f cuepos = cueball->getPosition();
	stickdirection = mousePos - cuepos;
	
	//finds the angle needed to rotate the poolstick by
	double angle = (atan2(stickdirection.y,stickdirection.x)*360/(2*M_PI))+270;

	//rotates and moves the stick to its new position
	poolstick->setRotation((float)angle);
	stickdirection = stickdirection*(15.0f / magnitude(stickdirection));
	sf::Vector2f stickposition = cuepos + stickdirection;
	poolstick->setPosition(stickposition);

}

//shoots the cueball with some power
void shootcueball(stick *poolstick, Cueball *cueball,Player *player1,Player *player2)
{
	cueball->setWasShot(true);
	double p = poolstick->getpower();
	sf::Vector2f stickpos= poolstick->getPosition();
	sf::Vector2f cuepos = cueball->getPosition();
	//unit vector for direction of hit
	sf::Vector2f u = -1.0f*(stickpos - cuepos);
	u = u / magnitude(u);

	//stops the drawing of poolstick
	poolstick->setshowstick(false);

	//adds velocity based on power between 0 and 2m/s
	cueball->velocity = u * ((float)p/5.0f);
}

//Initializes which player's turn it is randomly
void initializeplayer(Player *player1, Player *player2)
{
	/* initialize random seed: */
	srand((unsigned int)time(NULL));
	int rnum = rand() % 2;
	if (rnum == 0)
	{
		player1->setTurn(true);
	}
	else
	{
		player2->setTurn(true);
	}
}


//checks if ball's current position is valid - mainly for moving cueball
bool validPosition(ball *ball1, std::vector<ball*> allballs)
{
	sf::Vector2f ballpos = ball1->getPosition();
	float rad1 = ball1->getRadius();
	//checks if inside boundary
	if ((ballpos.x<(20 + rad1)) || (ballpos.x>(780 - rad1)) || (ballpos.y<(20 + rad1)) || (ballpos.y>(380 - rad1)))
	{
		return false;
	}
	//checks inside hole
	if ((distance(ballpos, sf::Vector2f(20, 20)) < (rad1 + 15)) ||
		(distance(ballpos, sf::Vector2f(400, 20)) < (rad1 + 15)) ||
		(distance(ballpos, sf::Vector2f(780, 20)) < (rad1 + 15)) ||
		(distance(ballpos, sf::Vector2f(20, 380)) < (rad1 + 15)) ||
		(distance(ballpos, sf::Vector2f(400, 380)) < (rad1 + 15)) ||
		(distance(ballpos, sf::Vector2f(780, 380)) < (rad1 + 15)))
	{
		return false;
	}
	//checks if overlapping with another ball's position
	for (unsigned int i = 0; i < allballs.size(); i++)
	{
		sf::Vector2f ballpos2 = allballs.at(i)->getPosition();
		float rad2 = allballs.at(i)->getRadius();
		if ((allballs.at(i) != ball1) && (distance(ballpos, ballpos2) < (rad1 + rad2)))
		{
			return false;
		}
	}
	//if none of the above, return true
	return true;
}

//Resets the position of the cueball if it falls into a hole
void resetcueball(Cueball *cueball,Player *player1, Player *player2)
{
	if (player1->isTurn())
	{
		player1->setFault(true);
	}
	else if (player2->isTurn())
	{
		player2->setFault(true);
	}
	cueball->setPosition(600, 200);
	cueball->setisinhole(false);
}

//sets whose turn it is to play
void setPlayerTurn(GameState *state)
{
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	//checks whose turn it currently is and sets new turn
	if (player1->isTurn())
	{
		//removes last turn's fault for other player
		player2->setFault(false);
		//if player1 did not get any ball or committed a fault, then switch to player2's turn, otherwise keep player1's turn
		if (player1->isFault())
		{
			player1->setTurn(false);
			player2->setTurn(true);
		}
		else if (player1->getScore() == 0)
		{
			player1->setTurn(false);
			player2->setTurn(true);
		}
	}
	else if (player2->isTurn())
	{
		//removes last turn's fault for other player
		player1->setFault(false);
		//if player2 did not get any ball in then switch to player1's turn, otherwise keep player1's turn
		if (player2->isFault())
		{
			player2->setTurn(false);
			player1->setTurn(true);
		}
		else if (player2->getScore() == 0)
		{
			player2->setTurn(false);
			player1->setTurn(true);
		}
	}
}

//sets the balltype player has to get in to win
void setBallType(sf::RenderWindow *window, GameState *state, stick *poolstick)
{
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;

	//checks if  a player already has a balltype
	if (player1->getNoBalltype())
	{
		int bluein = state->getBlueball() - state->getoldBlueball();
		int redin = state->getRedball() - state->getoldRedball();
		//checks whose turn it is and sets the balltype if they get any ball in
		if (player1->isTurn())
		{
			//checks which ball the player got in
			if ((bluein > 0) && (redin > 0))
			{
				//activate choice if you get more than one type of ball in the same turn
				chooseballtype(window, state, poolstick);
			}
			else if ((bluein > 0) && (redin==0))
			{
				player1->setBalltype("blue");
				player2->setBalltype("red");
			}
			else if ((redin > 0) && (bluein == 0))
			{
				player1->setBalltype("red");
				player2->setBalltype("blue");
			}
		}
		else if (player2->isTurn())
		{
			//checks which ball the player got in
			if ((bluein > 0) && (redin > 0))
			{
				//activate choice if you get more than one type of ball in the same turn
				chooseballtype(window, state, poolstick);
			}
			else if ((bluein > 0) && (redin==0))
			{
				player2->setBalltype("blue");
				player1->setBalltype("red");
			}
			else if ((redin > 0) && (bluein == 0))
			{
				player2->setBalltype("red");
				player1->setBalltype("blue");
			}
		}
	}
}

//checks for fault if you hit the wrong ball type
void checkballtype(sf::RenderWindow *window, GameState *state, stick *poolstick)
{
	std::vector<ball*> allballs = state->allballs();
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	Cueball *cueball = &state->cueball;

	//checks which player's turn it is
	if (player1->isTurn())
	{
		//checks whether it hit anything at all
		if (cueball->getFirstCollision() == NULL)
		{
			player1->setFault(true);
		}
		//checks whether the ball it hit is the same as balltype if there is a balltype
		else if (!player1->getNoBalltype())
		{
			if (player1->getBalltype() == "red")
			{
				player1->setFault(true);
				//checks if the first collision of cueball is red and if so sets fault to false
				for (int i = 1; i <= 7; i++)
				{
					if (cueball->getFirstCollision() == allballs[i])
					{
						player1->setFault(false);
					}
				}
			}
			else if (player1->getBalltype() == "blue")
			{
				player1->setFault(true);
				//checks if the first collision of cueball is blue and if so sets fault to false
				for (int i = 9; i <= 15; i++)
				{
					if (cueball->getFirstCollision() == allballs[i])
					{
						player1->setFault(false);
					}
				}
			}
		}
		else
		{
			//sets the ball type if not set already
			setBallType(window,state,poolstick);
		}
	}
	else if (player2->isTurn())
	{
		//checks whether it hit anything at all
		if (cueball->getFirstCollision() == NULL)
		{
			player2->setFault(true);
		}
		//checks whether the ball it hit is the same as balltype if there is a balltype
		else if (!player2->getNoBalltype())
		{
			if (player2->getBalltype() == "red")
			{
				player2->setFault(true);
				//checks if the first collision of cueball is red and if so sets fault to false
				for (int i = 1; i <= 7; i++)
				{
					if (cueball->getFirstCollision() == allballs[i])
					{
						player2->setFault(false);
					}
				}
			}
			else if (player2->getBalltype() == "blue")
			{
				player2->setFault(true);
				//checks if the first collision of cueball is blue and if so sets fault to false
				for (int i = 9; i <= 15; i++)
				{
					if (cueball->getFirstCollision() == allballs[i])
					{
						player2->setFault(false);
					}
				}
			}
		}
		else
		{
			//sets the ball type if not set already
			setBallType(window, state, poolstick);
		}
	}
}

//does one turn of pool game with its various rules
void poolgame(sf::RenderWindow *window, GameState *state, stick *poolstick)
{
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	Cueball *cueball = &state->cueball;
	//starts a new turn
	//commits the current position of cueball
	//updates the current ballcount inside the game state
	state->updateballcount();
	cueball->commitposition();

	cueball->setWasShot(false);
	//checks whether the player hit the correct first ball and sets ball type if not set already
	checkballtype(window,state,poolstick);
	state->updateScore();
	//resets the position of cueball if it falls into hole and consider that a fault
	if (cueball->getisinhole())
	{
		resetcueball(cueball, player1, player2);
	}
	//sets whcih player's turn it is
	setPlayerTurn(state);
	//resets the first collision of the cueball
	cueball->setFirstCollision(NULL);
}