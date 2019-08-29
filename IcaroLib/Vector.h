#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"

class Vector
{
public:
	//DEFAULT CONSTRUCTOR
	Vector();

	//OVERLOAD CONSTRUCTOR
	Vector(double di, double dj);
	//OVERLOAD CONSTRUCTOR (from another vector)
	Vector(const Vector& other);
	//OVERLOAD CONSTRUCTOR (from two points)
	Vector(const Point& pointFrom, const Point& pointTo);
	//DESTRUCTOR
	~Vector();

	//ALLOCATION
	void Set(double di, double dj);
	//ALLOCATION (from another vector)
	void Set(const Vector& other);
	//ALLOCATION (from two points)
	void Set(const Point& pointFrom, const Point& pointTo);

	//VARIABLES
	double i;
	double j;
};

#endif // VECTOR_H
