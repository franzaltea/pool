#include "physics.h"

//==========================================================================================
//Constants
//==========================================================================================
//conversion factor for frames to seconds
float sec = dt;
//conversion factor for pixel to meter ratio - 3.0cm = 10 pixel
float meter = 0.003f;
//conversion factor for gram to kilogram
float kg = 0.001f;
//coefficient of rolling friction - dimensionless
float mu = 0.009f;
//time required for ball to fall in hole in seconds
float holet = 0.04f;
//calculates the acceleration due to gravity in m/s^2
float g = 9.8f;
//minimum velocity that is approximately 0
double epsilon = dt*mu*g*1.1;

//==========================================================================================
//Functions
//==========================================================================================
//takes the dot product of 2 Vector2f
float dot(sf::Vector2f v1, sf::Vector2f v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

//Finds the distance between 2 Vector2f
float distance(sf::Vector2f v1, sf::Vector2f v2)
{
	return magnitude(v1 - v2);;
}

//Calculates the magnitude of the vector
float magnitude(sf::Vector2f v)
{
	return sqrt(dot(v, v));
}

//Calculates the projection of vector a onto vector b
sf::Vector2f project(sf::Vector2f a, sf::Vector2f b)
{
	return (dot(a, b) / dot(b, b))*b;
}

//returns a,b,c equation of a line in form ax+by+c = 0 when given a starting position and direction vector
std::vector<float> lineparameters(sf::Vector2f &initialpos, sf::Vector2f &direction)
{
	float a = direction.y;
	float b = -direction.x;
	float c = -b*initialpos.y - a*(initialpos.x);
	std::vector<float> v = { a, b, c };
	return v;
}

//uses gaussian elimination with pivoting to solve a set of linear equation in matrix form - assumes mxm matrix*x = RHS
std::vector<float> gaussianelimination(std::vector<std::vector<float>> &matrix, std::vector<float> &RHS)
{
	int N = matrix.size();
	for (int m = 0; m < N; m++)
	{
		//partial pivoting - checks if it's the last row and if not do partial pivoting
		if (m != N)
		{
			//checks which value in column m is farthest from zero and switches with current row
			float max = abs(matrix[m][m]);
			int row = m;

			for (int n = m+1; n < N; n++)
			{
				if (abs(matrix[n][m]) > max)
				{
					max = abs(matrix[n][m]);
					row = n;
				}
			}
			//switches the rows
			std::vector<float> temprow = matrix[m];
			float tempv = RHS[m];
			matrix[m] = matrix[row];
			matrix[row] = temprow;
			RHS[m] = RHS[row];
			RHS[row] = tempv;
		}

		//divides the row by the diagonal element to make the diagonal element 1
		float div = matrix[m][m];
		for (int n = 0; n < N; n++)
		{
			matrix[m][n] /= div;
		}
		RHS[m] /= div;

		//Subtract the current row from lower rows
		for (int k = m + 1; k < N; k++)
		{
			float mult = matrix[k][m];
			for (int n = 0; n < N; n++)
			{
				matrix[k][n] -= matrix[m][n] * mult;
			}
			RHS[k] -= RHS[m] * mult;
		}
	}
	/*Backsubstitution - takes the value of the bottom variable and substitute them to the top equations
	and solve the rest of the problems*/
	//initializes a vector to store values in
	std::vector<float> x(N,0);
	for (int m = N - 1; m >= 0; m--)
	{
		x[m] = RHS[m];
		//substitute into matrix and solve for x
		for (int i = m + 1; i < N; i++)
		{
			x[m] -= matrix[m][i] * x[i];
		}
	}
	return x;
}

//Given two lines in form [a,b,c], returns the intersection point of the two lines
sf::Vector2f intersection(std::vector<float> &line1, std::vector<float> &line2)
{
	//initializes a 2x2 matrix
	const int dim = 2;
	std::vector<std::vector<float>> A(dim);
	for (int i = 0; i < dim; i++)
	{
		A[i].resize(dim);
	}
	for (int m = 0; m < dim; m++)
	{
		A[0][m] = line1[m];
		A[1][m] = line2[m];
	}

	//calculates the RHS vector
	std::vector<float> v(dim);
	v[0] = -line1[2];
	v[1] = -line2[2];

	//calculates the position where they intersect
	std::vector<float> intersect = gaussianelimination(A, v);
	sf::Vector2f inter(intersect[0], intersect[1]);
	return inter;


}

//returns the intersection with boundary points of a line given an initial position of the cueball and direction vector u
sf::Vector2f aimindicatorposition(sf::Vector2f &u, sf::Vector2f &initpos, std::vector<ball*> &allballs)
{
	//calculate the direction unit vector for each of the four corners
	sf::Vector2f topleft = sf::Vector2f(20, 20) - initpos;
	sf::Vector2f topright = sf::Vector2f(780, 20) - initpos;
	sf::Vector2f bottomright = sf::Vector2f(780, 380) - initpos;
	sf::Vector2f bottomleft = sf::Vector2f(20, 380) - initpos;
	//calculates the angle at which they are pointing
	float atopleft = atan2(topleft.y, topleft.x);
	float atopright = atan2(topright.y, topright.x);
	float abottomright = atan2(bottomright.y, bottomright.x);
	float abottomleft = atan2(bottomleft.y, bottomleft.x);

	//calculates against which boundary the direction is pointing
	float uangle = atan2(u.y, u.x);
	std::vector<float> par = lineparameters(initpos, u);
	std::vector<float> bound(3);

	//calculates nnormal vector of direction vector and radius of cueball
	sf::Vector2f normal(u.y,-u.x);
	float rad1 = allballs[0]->getRadius();

	//calculates the closest possible collision with other balls
	float mindist = 1000;
	sf::Vector2f ballprojectf;
	float normdistf;
	float rad2;
	for (unsigned int i = 1; i < allballs.size(); i++)
	{
		if (!allballs.at(i)->getisinhole())
		{
			sf::Vector2f ballpos = allballs.at(i)->getPosition();
			rad2 = allballs[i]->getRadius();
			std::vector<float> normalline = lineparameters(ballpos, normal);

			//calculates the intersection
			sf::Vector2f ballproject = intersection(par, normalline);
			float normdist = distance(ballproject, ballpos);
			//finds the closest possible collision with other balls
			if (normdist <= (rad1 + rad2))
			{
				float balldist = distance(ballproject, initpos);
				float direction = dot(ballproject - initpos, u);	//whether ball is positive or negative direction
				if ((balldist < mindist) && (direction>0))
				{
					mindist = balldist;
					ballprojectf = ballproject;
					normdistf = normdist;
				}
			}
		}
	}

	//if there was collision with other balls return new position, else calculate boundary collision
	if (mindist < 1000)
	{
		sf::Vector2f aimpos = ballprojectf - u*(float)sqrt(pow((rad1+rad2),2) - pow(normdistf,2));
		return aimpos;
	}

	//check with right boundary
	if ((atopright <= uangle) && (uangle < abottomright))
	{
		bound = { 1, 0, -780 };
	}
	//check with bottom
	else if ((abottomright <= uangle) && (uangle < abottomleft))
	{
		bound = { 0, 1, -380 };
	}
	//check with left
	else if ((abottomleft <= uangle) || (uangle<atopleft))
	{
		bound = { 1, 0, -20 };
	}
	//check with top
	else if ((atopleft <= uangle) && (uangle < atopright))
	{
		bound = { 0, 1, -20 };
	}
	

	//calculates the position where they intersect
	sf::Vector2f aimpos = intersection(par, bound);
	return aimpos;
}

//checks if velocity is less than epsilon, if it is then turns it to 0
bool checkvelocity(ball *balln)
{
	if (magnitude(balln->velocity) < epsilon)
	{
		balln->velocity = sf::Vector2f(0, 0);
		return false;
	}
	else
	{
		return true;
	}
}


//checks if a ball is in a position in hole
void checkinholes(ball *balln)
{
	//defines needed variables
	sf::Vector2f pos = balln->getPosition();

	//checks if in top left hole
	if (magnitude(pos - sf::Vector2f(20, 20)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	//top middle hole
	else if (magnitude(pos - sf::Vector2f(400, 20)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	//top right hole
	else if (magnitude(pos - sf::Vector2f(780, 20)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	//bottom left hole
	else if (magnitude(pos - sf::Vector2f(20, 380)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	//bottom middle hole
	else if (magnitude(pos - sf::Vector2f(400, 380)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	//bottom right hole
	else if (magnitude(pos - sf::Vector2f(780, 380)) <= 15)
	{
		balln->addtoTimer(dt);
		if (balln->elapsedTime() >= holet)
		{
			balln->setisinhole(true);
			balln->velocity = sf::Vector2f(0, 0);
		}
	}
	else
	{
		balln->resetTimer();
	}
	
}
//Boundary conditions-reflect if a ball goes past the boundary
void boundary(ball *balln)
{
	//define needed variables
	sf::Vector2f pos = balln->getPosition();
	float rad = balln->getRadius();
	sf::Vector2f v = balln->velocity;

	//right boundary condition
	if ((pos.x + rad >= 780) && (pos.y >35) && (pos.y<365))
	{

		//normal unit vector of boundary
		sf::Vector2f normal(-1, 0);
		sf::Vector2f v1 = project(v, normal);
		//calculates the reflection of the ball
		if (v1.x*normal.x < 0)
		{
			sf::Vector2f reflect;
			reflect = project(balln->velocity, normal);
			balln->velocity -= 2.0f * reflect;
		}
	}
	//left boundary condition
	else if ((pos.x - rad < 20) && (pos.y >35) && (pos.y<365))
	{
		//normal unit vector of boundary
		sf::Vector2f normal(1, 0);
		sf::Vector2f v1 = project(v, normal);
		//calculates the reflection of the ball
		if (v1.x*normal.x < 0)
		{
			sf::Vector2f reflect;
			reflect = project(balln->velocity, normal);
			balln->velocity -= 2.0f * reflect;
		}
	}
	//top boundary condition
	else if ((pos.y - rad < 20) && (((pos.x>35) && (pos.x<385)) || ((pos.x>415) && (pos.x<765))))
	{
		//normal unit vector of boundary
		sf::Vector2f normal(0, 1);
		sf::Vector2f reflect;
		sf::Vector2f v1 = project(v, normal);
		//calculates the reflection of the ball
		if (v1.y*normal.y < 0)
		{
			sf::Vector2f reflect;
			reflect = project(balln->velocity, normal);
			balln->velocity -= 2.0f * reflect;
		}
	}
	//bottom boundary condition
	else if ((pos.y + rad > 380) && (((pos.x>35) && (pos.x<385)) || ((pos.x>415) && (pos.x<765))))
	{
		//normal unit vector of boundary
		sf::Vector2f normal(0, -1);
		sf::Vector2f v1 = project(v, normal);
		//calculates the reflection of the ball
		if (v1.y*normal.y < 0)
		{
			sf::Vector2f reflect;
			reflect = project(balln->velocity, normal);
			balln->velocity -= 2.0f * reflect;
		}
	}

	//top left hole bc
	else if (magnitude(pos - sf::Vector2f(20, 20)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(20, 20) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((magnitude(balledge - sf::Vector2f (20, 20)) > 15) && ((balledge.x < 20) || (balledge.y < 20)))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x <0)||(v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
	//top middle
	else if (magnitude(pos - sf::Vector2f(400, 20)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(400, 20) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((distance(balledge, sf::Vector2f(400, 20)) > 15) && (balledge.y < 20))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x<0) || (v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
	//top right hole
	else if (magnitude(pos - sf::Vector2f(780, 20)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(780, 20) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((magnitude(balledge - sf::Vector2f(780, 20)) >= 15) && ((balledge.x >= 780) || (balledge.y <= 20)))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x <0) || (v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
	//bottom left hole
	else if (magnitude(pos - sf::Vector2f(20, 380)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(20, 380) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((magnitude(balledge - sf::Vector2f(20, 380)) > 15) && ((balledge.x < 20) || (balledge.y > 380)))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x) || (v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
	//bottom middle hole
	else if (magnitude(pos - sf::Vector2f(400, 380)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(400, 380) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((magnitude(balledge - sf::Vector2f(400, 380)) > 15) && (balledge.y > 380))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x <0) || (v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
	//bottom right hole
	else if (magnitude(pos - sf::Vector2f(780, 380)) < 15)
	{
		//calculates the normal unit vector of possible surface
		sf::Vector2f normal;
		normal = sf::Vector2f(780, 380) - pos;
		normal = normal / magnitude(normal);
		//finds the edge of ball closest to wall
		sf::Vector2f balledge;
		balledge = pos + normal*rad;
		//checks for collision with wall
		if ((magnitude(balledge - sf::Vector2f(780, 380)) > 15) && ((balledge.x > 780) || (balledge.y > 380)))
		{
			sf::Vector2f v1 = project(v, normal);
			//calculates the reflection of the ball
			if ((v1.x*normal.x <0) || (v1.y*normal.y < 0))
			{
				sf::Vector2f reflect;
				reflect = project(balln->velocity, normal);
				balln->velocity -= 2.0f * reflect;
			}
		}
	}
}

//given a velocity, calculates the acceleration of the ball due to friction
sf::Vector2f ballfriction(sf::Vector2f v,sf::Vector2f a)
{
	if (magnitude(v-dt*a) < epsilon)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		return -(v/magnitude(v))*(mu*g);
	}
}

//calculates the total acceleration of the ball
sf::Vector2f acceleration(ball *ball1)
{
	sf::Vector2f friction = ballfriction(ball1->velocity, ball1->acceleration);
	return friction;
}


/*second order ODE to solve-Newton's equation
where a is current acceleration used to estimate zero velocity*/
sf::Vector2f f1(sf::Vector2f x, sf::Vector2f v,sf::Vector2f a)
{
	return ballfriction(v, a);
}

//first order ODE to solve - Newton's equation
sf::Vector2f f2(sf::Vector2f x, sf::Vector2f v, sf::Vector2f a)
{
	return v;
}

//sets the acceleration of all the balls
void setacceleration(std::vector<ball*> balls)
{
	for (unsigned int i = 0; i < balls.size(); i++)
	{
		balls.at(i)->acceleration = acceleration(balls.at(i));
	}
}

//Checks for collisions over each the balls, and if there is a collision, calculates the result
void collision(ball *balln, std::vector <ball*> balls)
{
	sf::Vector2f ballpos1 = balln->getPosition();
	float radius1 = balln->getRadius();

	//checks for collision for each ball
	for (unsigned int i = 0; i < balls.size(); i++)
	{
		//check if ball is in hole or not if so, then does not initiate collision
		if (!balls.at(i)->getisinhole())
		{
			sf::Vector2f ballpos2 = balls.at(i)->getPosition();
			float radius2 = balls.at(i)->getRadius();

			//calculates the normal and surface unit vector of ball2
			sf::Vector2f normal = ballpos1 - ballpos2;
			normal /= magnitude(normal);
			sf::Vector2f surface(-normal.y, normal.x);

			//defines needed variables
			float m1 = balln->getMass();
			sf::Vector2f v1 = project(balln->velocity, normal);
			float m2 = balls.at(i)->getMass();
			sf::Vector2f v2 = project(balls.at(i)->velocity, normal);

			//if the balls are touching and heading towards each other, initiate collision
			if ((distance(ballpos1, ballpos2) < (radius1 + radius2)) && (distance(ballpos1, ballpos2) > 0) && ((v1.x*normal.x<0)||(v1.y*normal.y<0)))
			{
				//ellastic collision
				sf::Vector2f v1f = ((m1 - m2) / (m1 + m2))*(v1 - v2) + v2;
				sf::Vector2f v2f = (m1 / m2)*v1 + v2 - (m1 / m2)*v1f;
				balln->velocity = project(balln->velocity, surface) + v1f;
				balls.at(i)->velocity = project(balls.at(i)->velocity, surface) + v2f;

				//if cueball sets the first ball that it collides with as lastball
				if ((balln == balls.at(0)) && (balln->getFirstCollision() == NULL))
				{
					balln->setFirstCollision(balls.at(i));
				}
			}
		}
	}
}



//given a velocity and acceleration, moves all the ball some meters
void moveball(std::vector <ball*> balls)
{
	//moves each ball using Euler method
	for (unsigned int j = 0; j < balls.size(); j++)
	{
		//checks if velocity of ball is greater than epsilon and if so move the ball
		if ((checkvelocity(balls.at(j))) && (!balls.at(j)->getisinhole()))
		{
			//defines needed variables
			sf::Vector2f a0 = balls.at(j)->acceleration;
			sf::Vector2f x0 = balls.at(j)->getPosition()*meter;
			sf::Vector2f v0= balls.at(j)->velocity;
			sf::Vector2f v1;
			sf::Vector2f x1;
			sf::Vector2f k1x, k2x, k3x, k4x;
			sf::Vector2f k1v, k2v, k3v, k4v;

			//uses 4th order runge kutta to solve for new position of the ball
			k1v = dt*f1(x0, v0, a0);
			k1x = dt*f2(x0, v0, a0);
			k2v = dt*f1(x0 + 0.5f*k1x, v0 + 0.5f*k1v, a0);
			k2x = dt*f2(x0 + 0.5f*k1x, v0 + 0.5f*k1v, a0);
			k3v = dt*f1(x0 + 0.5f*k2x, v0 + 0.5f*k2v, a0);
			k3x = dt*f2(x0 + 0.5f*k2x, v0 + 0.5f*k2v, a0);
			k4v = dt*f1(x0 + k3x, v0 + k3v, a0);
			k4x = dt*f2(x0 + k3x, v0 + k3v, a0);

			v1 = v0 + (1.0f / 6.0f)*(k1v + 2.0f*k2v + 2.0f*k3v + k4v);
			x1 = x0 + (1.0f / 6.0f)*(k1x + 2.0f*k2x + 2.0f*k3x + k4x);

			balls.at(j)->velocity = v1;
			balls.at(j)->setPosition(x1 / meter);

			//checks for collision
			collision(balls.at(j), balls);

			//checks if inside a hole
			checkinholes(balls.at(j));

			//checks boundary condition
			boundary(balls.at(j));
		}
	}
}
