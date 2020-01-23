#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

#include "customsprite.h"

//==========================================================================================
//Constants
//==========================================================================================
//Frames per second of physics in Hz
extern int freq;
//conversion factor for pixel to meter ratio - 3.0cm = 10 pixel
extern float holet;
//timestep dt for each movement in s
extern float dt;
//minimum velocity that is approximately 0
extern double epsilon;

//==========================================================================================
//Functions
//==========================================================================================
//takes the dot product of 2 Vector2f
float dot(sf::Vector2f v1, sf::Vector2f v2);

//Finds the distance between 2 Vector2f
float distance(sf::Vector2f v1, sf::Vector2f v2);

//Calculates the magnitude of the vector
float magnitude(sf::Vector2f v);

//Calculates the projection of vector a onto vector b
sf::Vector2f project(sf::Vector2f a, sf::Vector2f b);

//2d line function
sf::Vector2f line(sf::Vector2f v, sf::Vector2f b);

//returns a,b,c equation of a line in form ax+by+c = 0 when given a starting position and direction vector
std::vector<float> lineparameters(sf::Vector2f &initialpos, sf::Vector2f &direction);

//uses gaussian elimination with pivoting to solve a set of linear equation in matrix form - assumes mxm matrix
std::vector<float> gaussianelimination(std::vector<std::vector<float>> &matrix, std::vector<float> &RHS);

//returns the intersection with boundary points of a line given an initial position and direction vector u
sf::Vector2f aimindicatorposition(sf::Vector2f &u, sf::Vector2f &initpos, std::vector<ball*> &allballs);

//Given two lines in form [a,b,c], returns the intersection point of the two lines
sf::Vector2f intersection(std::vector<float> &line1, std::vector<float> &line2);

//checks if velocity is less than epsilon, if it is then turns it to 0
bool checkvelocity(ball *balln);

//checks if a ball is in a position in hole
void checkinholes(ball *balln);

//Boundary conditions-reflect if a ball goes past the boundary
void boundary(ball *balln);

//given a velocity and previous acc, calculates the acceleration of the ball due to friction
sf::Vector2f ballfriction(sf::Vector2f v,sf::Vector2f a);

//calculates the total acceleration of the ball
sf::Vector2f acceleration(ball *ball1);

//sets the acceleration of all the balls
void setacceleration(std::vector<ball*> balls);

//Checks for collisions over each the balls, and if there is a collision, calculates the result
void collision(ball *balln, std::vector <ball*> balls);

//given their velocity and acceleration, moves all the ball some meters and checks for collision
void moveball(std::vector <ball*> balls);

