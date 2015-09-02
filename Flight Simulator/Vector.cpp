#include "Vector.h"

/* Vector()
*	description: creates a vector with x,y,z = 0 components
*/
Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

/* Vector(float ex, float ey, float ez)
*	description: creates a vector with the specified x,y,z components
*/
Vector::Vector(float ex, float ey, float ez)
{
	x = ex;
	y = ey;
	z = ez;
}

/*default destructor
*/
Vector::~Vector(){}

/*operator+(const Vector& B)
* description: adds the components of two vectors
* returns: vector of added components
*/
Vector Vector::operator +(const Vector& B)
{
	Vector Temp;
	Temp.x = this->x + B.x;
	Temp.y = this->y + B.y;
	Temp.z = this->z + B.z;
	return Temp;
}

/*crossproduct(Vector& B)
* description:takes the cross product of two vectors
* returns: resulting cross product
*/
Vector Vector::crossproduct(Vector B)
{
	Vector Temp;
	Temp.x = (y * B.z) - (z * B.y);
	Temp.y = (z * B.x) - (x * B.z);
	Temp.z = (x * B.y) - (y * B.x);
	return Temp;
}
