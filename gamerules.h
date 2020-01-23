#pragma once
#define _USE_MATH_DEFINES
#include "customsprite.h"
#include "physics.h"
#include "Player.h"
#include "GameState.h"
#include "DrawEngine.h"

#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

/*This program sets up the rules for the game of pool*/

//Initializes the position of the balls and poolstick
void initialposition(std::vector <ball*> balls,stick *poolstick);

//Redraws the poolstick based on user input(allows user to rotate it to follow mouse movement)
void stickmovement(sf::Window *window,stick *poolstick, ball *cueball);

//shoots the cueball with some power
void shootcueball(stick *poolstick, Cueball *cueball, Player *player1, Player *player2);

//Initializes which player's turn it is randomly
void initializeplayer(Player *player1, Player *player2);




//checks if ball's current position is valid - mainly for moving cueball
bool validPosition(ball *ball1, std::vector<ball*> allballs);

//Resets the position of the cueball if it falls into the hole and consider that a fault
void resetcueball(Cueball *cueball, Player *player1, Player *player2);

//sets whose turn it is to play
void setTurn(GameState *state);

//sets which ball the players have to hole in order to win the game
void setBallType(sf::RenderWindow *window,GameState *state,stick *poolstick);

//checks for fault if you hit the wrong ball type
void checkballtype(sf::RenderWindow *window, GameState *state, stick *poolstick);

//manages all the rules and turns of the poolgame
void poolgame(sf::RenderWindow *window,GameState *state,stick *poolstick);