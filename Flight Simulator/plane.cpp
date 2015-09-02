#include "plane.h"
#include <math.h>
#include <iostream>
using namespace std;

/* plane()
*	Description: constructs a plane object and initializes the heading, position, up, and wing vectors
*/
plane::plane()
{
	//planes initial position
	position.x = 0;
	position.y = 0;
	position.z = .1f;

	//planes inisitial heading vector
	heading.x = -.5f;
	heading.y = -.5f;
	heading.z = -.1f;
	//normalize the heading
	float length = sqrt(pow(heading.x, 2) + pow(heading.y, 2) + pow(heading.z, 2));
	heading.x = heading.x / length;
	heading.y = heading.y / length;
	heading.z = heading.z / length;

	//set initial up vector to be along z
	up.x = 0;
	up.y = 0;
	up.z = 1;
	//normalize the vector
	length = sqrt(pow(up.x, 2) + pow(up.y, 2) + pow(up.z, 2));
	up.x = up.x / length;
	up.y = up.y / length;
	up.z = up.z / length;

	//compute the initial wing vector: wing = heading x up / ||heading x up||
	wing = heading.crossproduct(up);
	length = sqrt(pow(wing.x, 2) + pow(wing.y, 2) + pow(wing.z, 2));
	wing.x = wing.x / length;
	wing.y = wing.y / length;
	wing.z = wing.z / length;

	//set the speed
	speed = -.001f;
}

//default destructor
plane::~plane(){}

/* update()
*	description: updates the position according to the speed and current heading
*/
void plane::update()
{
	position.x += speed * heading.x;
	position.y += speed * heading.y;
	position.z += speed * heading.z;
}

/* Roll(float angle)
*	description: computes the new wing vector and up after rolling 'angle' degrees
*/
void plane::Roll(float angle)
{
	if (angle == 0)
		return;
	//new wing vector = (old_wing * cos(angle)) + (old_up * sin(angle)) 
	wing.x = (wing.x * cos(angle)) + (up.x * sin(angle));
	wing.y = (wing.y * cos(angle)) + (up.y * sin(angle));
	wing.z = (wing.z * cos(angle)) + (up.z * sin(angle));
	//new up vector =  wing x heading
	up = wing.crossproduct(heading);
}

/* Pitch(float angle)
*	description: computes the new wing vector and up after rolling 'angle' degrees
*/
void plane::Pitch(float angle)
{
	if (angle == 0)
		return;
	//new heading vector = (old_heading * cos(angle)) + (old_up * sin(angle))
	heading.x = (heading.x * cos(angle)) + (up.x * sin(angle));
	heading.y = (heading.y * cos(angle)) + (up.y * sin(angle));
	heading.z = (heading.z * cos(angle)) + (up.z * sin(angle));
	//new up vector = wing x heading
	up = wing.crossproduct(heading);
}