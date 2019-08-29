#include "Vector.h"
#include "Point.h"

//DEFAULT CONSTRUCTOR
Vector::Vector()
{
	i = 0.0;
	j = 0.0;
}

//OVERLOAD CONSTRUCTOR
Vector::Vector(double di, double dj)
{
	i = di;
	j = dj;
}

//OVERLOAD CONSTRUCTOR (from another vector)
Vector::Vector(const Vector& other)
{
	i = other.i;
	j = other.j;
}

//OVERLOAD CONSTRUCTOR (from two points)
Vector::Vector(const Point& pointFrom, const Point& pointTo)
{
	i = pointTo.x - pointFrom.x;
	j = pointTo.y - pointFrom.y;
}

//DESTRUCTOR
Vector::~Vector()
{

}

//ALLOCATION
void Vector::Set(double di, double dj)
{
	i = di;
	j = dj;
}

//ALLOCATION (from another vector)
void Vector::Set(const Vector& other)
{
	i = other.i;
	j = other.j;
}

//ALLOCATION (from two points)
void Vector::Set(const Point& pointFrom, const Point& pointTo)
{
	i = pointTo.x - pointFrom.x;
	j = pointTo.y - pointFrom.y;
}
