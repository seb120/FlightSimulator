#pragma once

#include "Vector.h"
using namespace std;

class plane
{
public:
	/* plane()
	*	Description: constructs a plane object and initializes the heading, position, up, and wing vectors
	*/
	plane();

	/* default destructor
	*/
	~plane();

	//movement functions

	/* update()
	*	description: updates the position according to the speed and current heading
	*/
	void update();

	/* Roll(float angle)
	*	description: computes the new wing vector and up after rolling 'angle' degrees
	*/
	void Roll(float angle);

	/* Pitch(float angle)
	*	description: computes the new wing vector and up after rolling 'angle' degrees
	*/
	void Pitch(float angle);

	//plane vectors
	Vector position;
	Vector heading;
	Vector up;
	Vector wing;

	//speed
	float speed;
};
