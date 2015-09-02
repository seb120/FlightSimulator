#pragma once

class Vector
{
public:
	/* Vector()
	*	description: creates a vector with x,y,z = 0 components
	*/
	Vector();

	/* Vector(float ex, float ey, float ez)
	*	description: creates a vector with the specified x,y,z components
	*/
	Vector(float ex, float ey, float ez);

	/*default destructor
	*/
	~Vector();

	/*operator+(const Vector& B)
	* description: adds the components of two vectors
	* returns: vector of added components
	*/
	Vector operator +(const Vector& B);

	/*crossproduct(Vector& B)
	* description:takes the cross product of two vectors
	* returns: resulting cross product
	*/
	Vector crossproduct(Vector B);

	//components
	float x;
	float y;
	float z;

};
