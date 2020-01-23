/*Main Program
8ball pool game
This program uses SFML to create a 2d pool game*/


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/System.hpp>
#include <vector>
#include <math.h>
#include <sstream>

#include "customsprite.h"
#include "DrawEngine.h"
#include "gamerules.h"
#include "Player.h"
#include "GameState.h"


//==========================================================================================
//Constants
//==========================================================================================
//Frames per second of physics in Hz
int freq = 120;
float dt = 1.0f / freq;

//============================================================================================
//Main Program
//============================================================================================

int main()
{	//creates drawing window
	sf::RenderWindow window(sf::VideoMode(800, 500), "Pool", sf::Style::Titlebar |sf::Style::Close);

	//load font
	sf::Font arial;
	if (!arial.loadFromFile("arial.ttf"))
	{
		return 1;
	}


	
	//loads textures
	sf::Texture tabletex;
	if (!tabletex.loadFromFile("pooltable.png"))
		return 1;
	sf::Texture balltex;
	if (!balltex.loadFromFile("balltexture1.png"))
		return 1;
	sf::Texture sticktex;
	if (!sticktex.loadFromFile("poolstick.png"))
		return 1;
	
	sf::Texture handcursortext1;
	if (!handcursortext1.loadFromFile("handcursor1.png"))
		return 1;
	sf::Texture handcursortext2;
	if (!handcursortext2.loadFromFile("handcursor2.png"))
		return 1;

	//creates an array of 7 redballs and 7 blueballs
	ball red[7];
	ball blue[7];
	for (int i = 0; i < 7; i++)
	{
		red[i] = redball(&balltex);
		blue[i] = blueball(&balltex);
	}

	//Creates white cue ball and black 8th ball
	Cueball cueball = whitecueball(&balltex);
	ball finball = blackball(&balltex);


	/*puts all the balls into a vector -  red between 0-6, black in 7, blue in 8-14*/
	std::vector <ball> balls;
	for (int i = 0; i < 7; i++)
	{
		balls.push_back(red[i]);
	}
	balls.push_back(finball);
	for (int i = 0; i < 7; i++)
	{
		balls.push_back(blue[i]);
	}

	//creates poolstick
	stick poolstick(&sticktex);

	//creates player
	Player player1("Player 1");
	Player player2("Player 2");

	//initializes which player goes first
	initializeplayer(&player1, &player2);

	//creates a new game state
	GameState gamestate(balls, cueball, player1, player2);

	//initializes all of the ball's position
	initialposition(gamestate.allballs(), &poolstick);


	//FPS text
	sf::Text fps;
	fps.setFont(arial);
	fps.setCharacterSize(12);
	fps.setColor(sf::Color(100, 100, 100));

	//measures time and sets frame rate
	sf::Clock clock; // starts the clock
	double accumulator = 0;	//measures physics time
	unsigned int framerate;
	framerate = 60;
	//window.setFramerateLimit(framerate);
	window.setVerticalSyncEnabled(true);


	//creates a boolean for checking if it is the focus window
	bool focus = true;

	//run program as long as window is open
	while (window.isOpen())
	{
		//check all the event's that were triggered in last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			//close window if requested
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
			if (event.type == sf::Event::LostFocus)
			{
				focus = false;
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				focus = true;
			}
		}

		//start drawing
		// clear the window with white color
		window.clear(sf::Color (255,255,255));

		//adds pool table
		sf::Sprite pooltable;
		pooltable.setTexture(tabletex);
		window.draw(pooltable);
		
		//calculates FPS and display it as text
		sf::Time elapsed1 = clock.getElapsedTime();
		accumulator += elapsed1.asSeconds();
		
		std::string fpsstring = "FPS: ";
		double FPS = 1.0 / elapsed1.asSeconds();
		std::stringstream convert;
		convert << ceil(FPS*10)/10;
		fpsstring.append(convert.str());
		fps.setString(fpsstring);
		fps.setPosition(730, 475);
		window.draw(fps);

		//does the physics movement
		while ((float)accumulator >= dt)
		{
			setacceleration(gamestate.allballs());
			moveball(gamestate.allballs());
			accumulator -= dt;
		}
		//restarts FPS clock
		clock.restart();

		//draws the player names
		drawplayername(&window, gamestate.player1, gamestate.player2, &balltex);

		//draws the objects
		DrawEngine(&window,&gamestate, &poolstick,&arial);


		//draw the poolstick only while focus window
		if ((focus) && (!gamestate.getEndgame()))
		{
			drawpoolstick(&window, &gamestate, &poolstick,&handcursortext1,&handcursortext2);
		}
		
		// end the current frame
		window.display();
	}


	return 0;
}