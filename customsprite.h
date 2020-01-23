/*Sprites*/
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

//=============================================================================
//BALL Class
//=============================================================================
/*Ball class creates a ball object with some radius r, given some texture*/
//class for pool balls, please set Texture before using
class ball : public sf::Sprite
{
public:
	ball();
	float getMass();
	void setMass(float m);
	void setRadius(float r);
	float getRadius();
	void setballTexture(sf::Texture *texture);
	bool getisinhole();
	void setisinhole(bool boolean);
	float elapsedTime();
	void resetTimer();
	void addtoTimer(float addtotime);
	std::string getBallColor();
	void setBallColor(std::string col);

	//getter function for first collision
	ball* getFirstCollision();
	//setter function for first collision
	void setFirstCollision(ball *ball1);
	//returns the color of last ba
	
	//velocity(x,y) and acc in m/s
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

protected:
	//radius is in pixels, mass in g
	float radius;
	float mass;
	bool isinhole;
	//creates a measure of how long in simulated seconds which starts when a ball enters a holespace
	float holetimer;
	//color of the ball
	std::string color;
	//stores the first ball that it hit as a pointer in last turn  - mainly for use by cueball
	ball *firstcollision;
	
};

class Cueball :public ball
{
public:
	//default constructor
	Cueball();
	//updates the anchor position to current position
	void commitposition();
	//resets the position of the ball to current position
	void resetposition();
	//getter function for anchorpos
	sf::Vector2f getAnchorPos();
	//getter function for wasshot
	bool getWasShot();
	//setter function for wasshot
	void setWasShot(bool thisturn);
private:
	//saved anchor position for use when moving the cueball by hand
	sf::Vector2f anchorpos;
	//boolean that states whether the cueball was shot this turn or not
	bool wasshot;
};

//=============================================================================
//BALL Functions
//=============================================================================

//Creates red ball and returns a copy of it given a texture
ball redball(sf::Texture *texture);

//Creates a blue ball and returns a copy of it given a texture
ball blueball(sf::Texture *texture);

//Creates white ball and returns a copy of it given a texture
ball whiteball(sf::Texture *texture);

//Creates a black ball and returns a copy of it given a texture
ball blackball(sf::Texture *texture);

//creates a white cueball and returns a copy of it given a texture
Cueball whitecueball(sf::Texture *texture);

//=============================================================================
//Stick Class
//=============================================================================
//class for poolstick
class stick : public sf::Sprite
{
public:
	stick(sf::Texture *texture);
	bool getmovestick();
	void setmovestick(bool boo);
	bool getshootstick();
	void setshootstick(bool boo);
	bool getshowstick();
	void setshowstick(bool show);
	void setpower(double val);
	double getpower();
	

private:
	bool showstick;
	bool movestick;
	bool shootstick;
	double power;

};

//=============================================================================
//Hole class
//=============================================================================
//class for a pool hole