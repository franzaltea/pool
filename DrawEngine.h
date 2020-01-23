#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "customsprite.h"
#include "gamerules.h"
#include "physics.h"
#include "Player.h"
#include "GameState.h"

/*Main Drawing Engine which draws all the moving objects- the balls and poolstick*/
void DrawEngine(sf::RenderWindow *window, GameState *state, stick *poolstick, sf::Font *font);

/*Stores all the functions used for drawing the engine*/
//draws a power bar at some position in the bottom
void powerbar(sf::RenderWindow *window, stick poolstick);

//draws the balls
void drawball(sf::RenderWindow *window, std::vector <ball*> allballs);

//draws the poolstick
void drawpoolstick(sf::RenderWindow *window, GameState *state, stick *poolstick, sf::Texture *handcursortext1, sf::Texture *handcursortext2);

//displays player's name and which player's turn it is
void drawplayername(sf::RenderWindow *window,Player player1, Player player2,sf::Texture *balltext);

//displays an indicator which predicts where the cueball is aimed at
void drawaim(sf::RenderWindow *window, GameState *state, stick *poolstick);

//if a player gets a fault, it changes the cursor when near the cueball and allows him to move the ball
void movecueball(sf::RenderWindow *window, GameState *state, stick *poolstick, sf::Texture *handcursortext1, sf::Texture *handcursortext2);

//if a player ins two different balls at the same time, gives the player a choice between two ball types
void chooseballtype(sf::RenderWindow *window, GameState *state,stick *poolstick);

//player 1 wins
void player1wins(sf::RenderWindow &window, GameState &state, sf::Font &font);

//player 2 wins
void player2wins(sf::RenderWindow &window, GameState &state, sf::Font &font);

//displays the winner of the game when a black ball goes in
void endgame(sf::RenderWindow *window, GameState *state,sf::Font *font);