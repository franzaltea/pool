#include "DrawEngine.h"

/*Main Drawing Engine which draws all the moving objects- the balls and poolstick*/
void DrawEngine(sf::RenderWindow *window, GameState *state, stick *poolstick,sf::Font *font)
{
	//adds powerbar
	powerbar(window, *poolstick);

	//draws all the balls
	drawball(window, state->allballs());

	//draw an aim indicator
	drawaim(window, state, poolstick);

	//endgame
	endgame(window, state, font);
}

//draws a power bar at some position in the bottom
void powerbar(sf::RenderWindow *window, stick poolstick)
{
	//draws the outline
	sf::RectangleShape poweroutline(sf::Vector2f(300, 10));
	poweroutline.setOutlineColor(sf::Color(0, 50, 250));
	poweroutline.setFillColor(sf::Color(250, 0, 0));
	poweroutline.setOutlineThickness(4);
	poweroutline.setPosition(10, 470);
	window->draw(poweroutline);

	//draws the value of the powerbar
	sf::RectangleShape bar;
	bar.setSize(sf::Vector2f(300 * (float)poolstick.getpower()/10, 10));
	bar.setFillColor(sf::Color(0, 255, 0));
	bar.setPosition(poweroutline.getPosition());
	window->draw(bar);
}

//draws the balls
void drawball(sf::RenderWindow *window, std::vector <ball*> allballs)
{
	//moveball(balls);
	for (int i = 0; i <= 15; i++)
	{
		if (!allballs.at(i)->getisinhole())
		{
			window->draw(*allballs.at(i));
		}
	}
}

//draws the poolstick and if cueball falls into hall, resets the position of the cueball
void drawpoolstick(sf::RenderWindow *window, GameState *state, stick *poolstick, sf::Texture *handcursortext1, sf::Texture *handcursortext2)
{
	std::vector<ball*> allballs = state->allballs();
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	
	Cueball *cueball = &state->cueball;
	//calculates the total speed of the balls combined together for checking if all the balls have the stopped
	float totalspeed = 0;
	for (unsigned int i = 0; i < allballs.size(); i++)
	{
		totalspeed += magnitude(allballs.at(i)->velocity);
	}
	//draws poolstick if allowed
	if (poolstick->getshowstick())
	{
		//if shooting is allowed, shoots the ball when mouse button is pressed
		if (poolstick->getshootstick())
		{
			//if mouse button is pressed, it stores power to shoot the cue ball
			if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) && (poolstick->getpower() <= 10))
			{
				poolstick->setpower(poolstick->getpower() + 0.08);
				poolstick->setmovestick(false);
			}
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//hits the ball with some power
				poolstick->setmovestick(true);
				if (poolstick->getpower() != 0)
				{
					shootcueball(poolstick, cueball,player1,player2);
					poolstick->setpower(0);
				}
			}
		}
		//moves the poolstick if allowed
		if (poolstick->getmovestick())
		{
			stickmovement(window, poolstick, cueball);
		}

		//draws it
		window->draw(*poolstick);
	}
	//else waits for all the balls to stop and then allows drawing of poolstick and calculates the next game turn
	else if ((totalspeed == 0) &&(cueball->getWasShot()))
	{
		
		poolstick->setshowstick(true);
		//sets the next player's turn
		poolgame(window,state,poolstick);
	}
	//moves the cueball if allowed
	movecueball(window, state, poolstick, handcursortext1, handcursortext2);
}


//displays player's name and which player's turn it is
void drawplayername(sf::RenderWindow *window,Player player1, Player player2,sf::Texture *balltext)
{
	sf::Font arial;
	if (!arial.loadFromFile("arial.ttf"))
	{
		std::cout << "Text failed to Load" << std::endl;
	}

	std::string p1name = player1.getName();
	std::string p2name = player2.getName();
	std::string type1 = player1.getBalltype();
	std::string type2 = player2.getBalltype();

	//player1 text
	sf::Text p1text;
	p1text.setFont(arial);
	p1text.setCharacterSize(20);
	p1text.setColor(sf::Color(0, 0, 255));
	p1text.setString(p1name);
	p1text.setOrigin(0, 10);
	p1text.setPosition(20, 415);

	//player2 text
	sf::Text p2text;
	p2text.setFont(arial);
	p2text.setCharacterSize(20);
	p2text.setColor(sf::Color(255, 0, 0));
	p2text.setString(p2name);
	p2text.setOrigin(0,10);
	p2text.setPosition(20, 435);

	//creates an indicator to show whose turn it is
	sf::CircleShape indicator(5);
	indicator.setFillColor(sf::Color(210, 220, 0));
	if (player1.isTurn())
	{
		indicator.setPosition(p1text.getPosition() - sf::Vector2f(12, 2));
	}
	else if (player2.isTurn())
	{
		indicator.setPosition(p2text.getPosition() - sf::Vector2f(12, 2));
	}

	//creates a ball sprite to show what ball type the player has to hit
	ball balltyper = redball(balltext);
	ball balltypeb = blueball(balltext);

	if (type1 == "red")
	{
		balltyper.setPosition(p1text.getPosition() + sf::Vector2f(100, 0));
		balltypeb.setPosition(p2text.getPosition() + sf::Vector2f(100, 0));

	}
	else if (type1 == "blue")
	{
		balltypeb.setPosition(p1text.getPosition() + sf::Vector2f(100, 0));
		balltyper.setPosition(p2text.getPosition() + sf::Vector2f(100, 0));
	}


	window->draw(p1text);
	window->draw(p2text);
	window->draw(indicator);
	if (!player1.getNoBalltype())
	{
		window->draw(balltyper);
		window->draw(balltypeb);
	}
}

//displays an indicator which predicts where the cueball is aimed at
void drawaim(sf::RenderWindow *window, GameState *state, stick *poolstick)
{
	
	if (poolstick->getshowstick())
	{
		//declares needed variables
		Cueball *cueball = &state->cueball;
		sf::Vector2f cuepos = cueball->getPosition();
		float rad = cueball->getRadius();
		sf::Vector2f stickpos = poolstick->getPosition();
		std::vector<ball*> allballs = state->allballs();

		//creates a direction unit vector where ball is currently aimed
		sf::Vector2f aimvector = cuepos - stickpos;
		aimvector /= magnitude(aimvector);

		//creates an aim indicator from circleshape
		sf::CircleShape aim(rad-1);
		aim.setOutlineColor(sf::Color(100, 100, 200, 225));
		aim.setOutlineThickness(1);
		aim.setOrigin(rad, rad);
		aim.setFillColor(sf::Color(0, 0, 0, 0));

		//calculates the position of the ball
		sf::Vector2f aimpos = aimindicatorposition(aimvector, cuepos, allballs);

		//creates a line for the aim indicator
		sf::VertexArray aimline(sf::Lines, 2);
		aimline.append(sf::Vertex(cuepos, sf::Color(100, 100, 200, 225)));
		aimline.append(sf::Vertex(aimpos, sf::Color(100, 100, 200, 225)));

		//sets the position and draws it
		aim.setPosition(aimpos);
		window->draw(aim);
		window->draw(aimline);
	}

}

//if a player gets a fault, it changes the cursor when near the cueball and allows him to move the ball
void movecueball(sf::RenderWindow *window, GameState *state,stick *poolstick,sf::Texture *handcursortext1,sf::Texture *handcursortext2)
{
	Cueball *cueball = &state->cueball;
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	sf::Vector2f cuepos = cueball->getPosition();
	std::vector<ball*> allballs = state->allballs();
	float totalspeed = 0;
	for (unsigned int i = 0; i < allballs.size(); i++)
	{
		totalspeed += magnitude(allballs.at(i)->velocity);
	}
	if (((player1->isFault()) || (player2->isFault())) && (totalspeed==0))
	{
		//creates a new cursor
		sf::Sprite handcursor1;
		sf::Sprite handcursor2;
		handcursor1.setTexture(*handcursortext1);
		handcursor2.setTexture(*handcursortext2);
		handcursor1.setScale(sf::Vector2f (0.2f,0.2f));
		handcursor1.setOrigin(12, 6);
		handcursor2.setScale(sf::Vector2f(0.2f, 0.2f));
		handcursor2.setOrigin(12, 6);

		//get current mouse position
		sf::Vector2f mousepos(sf::Mouse::getPosition(*window));
		//if close to cueball, change cursor to hand and allows it to move the ball
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (distance(mousepos, cueball->getPosition()) <= (cueball->getRadius() + 15.0f))
			{
				window->setMouseCursorVisible(false); // Hide cursor
				poolstick->setshowstick(false);
				poolstick->setshootstick(false);
				handcursor2.setPosition(mousepos);
				window->draw(handcursor2);
				cueball->setPosition(mousepos);
			}
			else
			{
				window->setMouseCursorVisible(true);
			}
			
		}
		//case when mouse is not dragging ball, but could click to drag ball
		else if (distance(mousepos, cueball->getPosition()) <= (cueball->getRadius() + 15.0f))
		{
			window->setMouseCursorVisible(false); // Hide cursor
			poolstick->setshowstick(false);
			poolstick->setshootstick(false);
			handcursor1.setPosition(mousepos);
			window->draw(handcursor1);
			//checks if current position is valid and commits it, or returns it
			if (validPosition(cueball, allballs))
			{
				cueball->commitposition();
			}
			else
			{
				cueball->resetposition();
			}
		}
		else
		{
			window->setMouseCursorVisible(true);
			poolstick->setshowstick(true);
			poolstick->setshootstick(true);
			//checks if current position is valid and commits it, or returns it
			if (validPosition(cueball, allballs))
			{
				cueball->commitposition();
			}
			else
			{
				cueball->resetposition();
			}
		}
	}
	else
	{
		window->setMouseCursorVisible(true);
	}
}

//if a player ins two different balls at the same time, gives the player a choice between two ball types
void chooseballtype(sf::RenderWindow *window, GameState *state,stick *poolstick)
{
	Player *player1 = &state->player1;
	Player *player2 = &state->player2;
	
	if ((player1->isTurn()) && (player1->getNoBalltype()))
	{
		//pauses the game until a choice has been made
		poolstick->setshowstick(false);

		//creates a red and blue circle to choose from
		sf::CircleShape rchoice(20);
		sf::CircleShape bchoice(20);
		rchoice.setOutlineThickness(2);
		rchoice.setOutlineThickness(2);
		rchoice.setOutlineColor(sf::Color::Black);
		bchoice.setOutlineColor(sf::Color::Black);
		rchoice.setFillColor(sf::Color(255, 0, 0));
		bchoice.setFillColor(sf::Color(0, 0, 255));
		sf::Vector2f rpos = sf::Vector2f(350, 400);
		sf::Vector2f bpos = sf::Vector2f(450, 400);
		rchoice.setPosition(rpos);
		bchoice.setPosition(bpos);

		window->draw(rchoice);
		window->draw(bchoice);

		//takes mouse click as an input choice
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f mousepos(sf::Mouse::getPosition(*window));
			//checks if user chooses red or blue
			if (distance(mousepos, rpos) <= 20)
			{
				poolstick->setshowstick(true);
				player1->setBalltype("red");
				player2->setBalltype("blue");
			}
			else if (distance(mousepos, rpos) <= 20)
			{
				poolstick->setshowstick(true);
				player1->setBalltype("blue");
				player2->setBalltype("red");
			}
		}

	}
	else if ((player2->isTurn()) && (player2->getNoBalltype()))
	{
		//pauses the game until a choice has been made
		poolstick->setshowstick(false);

		//creates a red and blue circle to choose from
		sf::CircleShape rchoice(20);
		sf::CircleShape bchoice(20);
		rchoice.setOutlineThickness(2);
		rchoice.setOutlineThickness(2);
		rchoice.setOutlineColor(sf::Color::Black);
		bchoice.setOutlineColor(sf::Color::Black);
		rchoice.setFillColor(sf::Color(255, 0, 0));
		bchoice.setFillColor(sf::Color(0, 0, 255));
		sf::Vector2f rpos = sf::Vector2f(350, 400);
		sf::Vector2f bpos = sf::Vector2f(450, 400);
		rchoice.setPosition(rpos);
		bchoice.setPosition(bpos);

		window->draw(rchoice);
		window->draw(bchoice);

		//takes mouse click as an input choice
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f mousepos(sf::Mouse::getPosition(*window));
			//checks if user chooses red or blue
			if (distance(mousepos, rpos) <= 20)
			{
				poolstick->setshowstick(true);
				player2->setBalltype("red");
				player1->setBalltype("blue");
			}
			else if (distance(mousepos, rpos) <= 20)
			{
				poolstick->setshowstick(true);
				player2->setBalltype("blue");
				player1->setBalltype("red");
			}
		}
	}
}

//player 1 wins
void player1wins(sf::RenderWindow &window, GameState &state, sf::Font &font)
{
	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(40);
	winner.setColor(sf::Color::Black);
	winner.setStyle(sf::Text::Bold);
	std::string win;
	win = state.player1.getName();
	win.append(" Wins");
	winner.setString(win);
	winner.setOrigin(0, 20);
	winner.setPosition(300, 225);
	window.draw(winner);

	state.setEndgame(true);

}

//player 2 wins
void player2wins(sf::RenderWindow &window, GameState &state, sf::Font &font)
{
	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(40);
	winner.setColor(sf::Color::Black);
	winner.setStyle(sf::Text::Bold);
	std::string win;
	win = state.player2.getName();
	win.append(" Wins");
	winner.setString(win);
	winner.setPosition(300, 225);
	winner.setPosition(300, 225);
	window.draw(winner);

	state.setEndgame(true);
	
}

//displays the winner of the game when a black ball goes in
void endgame(sf::RenderWindow *window, GameState *state,sf::Font *font)
{
	std::vector<ball*> allballs = state->allballs();
	Player &player1 = state->player1;
	Player &player2 = state->player2;
	Cueball &cueball = state->cueball;
	if (allballs[8]->getisinhole())
	{
		//if Player 1 turn
		if (player1.isTurn())
		{
			if (cueball.getisinhole())
			{
				player2wins(*window,*state,*font);
			}
			else if ((player1.getBalltype() == "red") && (state->getRedball()==7))
			{
				player1wins(*window, *state, *font);
			}
			else if ((player1.getBalltype() == "blue") && (state->getBlueball() == 7))
			{
				player1wins(*window, *state, *font);
			}
			else
			{
				player2wins(*window, *state, *font);
			}
			
		}
		else if (player2.isTurn())
		{
			if (cueball.getisinhole())
			{
				player1wins(*window, *state, *font);
			}
			else if ((player2.getBalltype() == "red") && (state->getRedball() == 7))
			{
				player2wins(*window, *state, *font);
			}
			else if ((player2.getBalltype() == "blue") && (state->getBlueball() == 7))
			{
				player2wins(*window, *state, *font);
			}
			else
			{
				player1wins(*window, *state, *font);
			}
		}
	}
	
}