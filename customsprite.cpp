#include "customsprite.h"

//=============================================================================
//BALL Class
//=============================================================================
/*Ball class creates a ball object with some radius r, given some texture*/
//constructor creates ball
ball::ball()
{
	//sets default value
	isinhole = false;
	mass = 160;
	velocity = sf::Vector2f(0, 0);
	acceleration = sf::Vector2f(0, 0);
	holetimer = 0;
}

//Sets Texture for ball, takes a pointer to the texture
//Use this instead of default setTexture and texture should be square
void ball::setballTexture(sf::Texture *texture)
{
	setTexture(*texture);
	sf::FloatRect bounds = getLocalBounds();
	radius = bounds.height/2;
	//sets the origin to the centre of the ball
	setOrigin(radius, radius);
}

//sets Radius of the ball in pixels
void ball::setRadius(float r)
{
	float tempr = radius;
	radius = r;
	this->setScale(radius / tempr, radius / tempr);
}


//returns the radius of the ball in pixels
float ball::getRadius()
{
	return radius;
}

//returns the mass of the ball in grams
float ball::getMass()
{
	return mass;
}

//sets the mass of the ball in grams
void ball::setMass(float m)
{
	mass = m;
}

//returns the isinhole boolean
bool ball::getisinhole()
{
	return isinhole;
}

//flips the isinhole boolean
void ball::setisinhole(bool boolean)
{
	isinhole = boolean;
}


//returns the elapsed time since it entered a hole
float ball::elapsedTime()
{
	return holetimer;
}

//resets the hole frame timer
void ball::resetTimer()
{
	holetimer = 0;
}
//adds time to timer
void ball::addtoTimer(float addtotime)
{
	holetimer += addtotime;
}

//returns the color of the string
std::string ball::getBallColor()
{
	return color;
}
//sets the color of the ball
void ball::setBallColor(std::string col)
{
	color = col;
}

//getter function for last ball
ball* ball::getFirstCollision()
{
	return firstcollision;
}

//setter function for lastball
void ball::setFirstCollision(ball *ball1)
{
	firstcollision = ball1;
}
//======================================================================
//Cueball subclass
//=====================================================================
//default constructor
Cueball::Cueball()
{
	anchorpos = getPosition();
	wasshot = false;
	firstcollision = NULL;
}
//updates the anchor position to current position
void Cueball::commitposition()
{
	anchorpos = getPosition();
}
//resets the position of the ball to current position
void Cueball::resetposition()
{
	setPosition(anchorpos);
}

//getter function for anchorpos
sf::Vector2f Cueball::getAnchorPos()
{
	return anchorpos;
}


//getter function for wasshot
bool Cueball::getWasShot()
{
	return wasshot;
}
//setter function for wasshot
void Cueball::setWasShot(bool thisturn)
{
	wasshot = thisturn;
}


//=============================================================================
//BALL Functions
//=============================================================================

//Creates a red ball and returns
ball redball(sf::Texture *texture)
{
	ball ballname;
	ballname.setballTexture(texture);
	ballname.setRadius(10);
	ballname.setColor(sf::Color(255, 0, 0));
	return ballname;
}

ball blueball(sf::Texture *texture)
{
	ball ballname;
	ballname.setballTexture(texture);
	ballname.setRadius(10);
	ballname.setColor(sf::Color(0, 0, 255));
	return ballname;
}

ball whiteball(sf::Texture *texture)
{
	ball ballname;
	ballname.setballTexture(texture);
	ballname.setRadius(10);
	return ballname;
}

ball blackball(sf::Texture *texture)
{
	ball ballname;
	ballname.setballTexture(texture);
	ballname.setRadius(10);

	ballname.setColor(sf::Color(30, 30, 30));
	return ballname;
}

//creates a white cueball and returns a copy of it given a texture
Cueball whitecueball(sf::Texture *texture)
{
	Cueball ballname;
	ballname.setballTexture(texture);
	ballname.setRadius(10);
	return ballname;
}
//=============================================================================
//Stick Class
//=============================================================================
//constructor - takes the texture of the stick and sets default values
stick::stick(sf::Texture *texture)
{
	setTexture(*texture);
	setOrigin(8, 0);
	showstick = true;
	power = 0;
	shootstick = true;
	movestick = true;
}

//Returns whether stick is movable or not
bool stick::getmovestick()
{
	return movestick;
}

//Switches whether stick is movable or not
void stick::setmovestick(bool boo)
{
	movestick = boo;
}

//Returns whether the stick is allowed to shoot when given input
bool stick::getshootstick()
{
	return shootstick;
}

//sets whether the stick is allowed to shoot when given input
void stick::setshootstick(bool boo)
{
	shootstick = boo;
}

//Returns whether stick will be shown or not - mainly for moving cueball without shooting
bool stick::getshowstick()
{
	return showstick;
}

//Switches whether stick will be shown or not
void stick::setshowstick(bool show)
{
	showstick = show;
}

//Sets power
void stick::setpower(double val)
{
	power = val;
}

//Returns the power
double stick::getpower()
{
	return power;
}