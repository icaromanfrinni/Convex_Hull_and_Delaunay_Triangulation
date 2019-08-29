/*  FUNCTION DEFINITIONS    */
#include <iostream>
#include <cmath>
#include "Point.h"
#include "constants.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Point::Point()
{
	//Initialize
	x = 0.0;
	y = 0.0;
}

//OVERLOAD CONSTRUCTOR
Point::Point(double dx, double dy)
{
	x = dx;
	y = dy;
}

//OVERLOAD CONSTRUCTOR (from another Point)
Point::Point(const Point& other)
{
	x = other.x;
	y = other.y;
}

//DESTRUCTOR
Point::~Point()
{

}

//ALLOCATION
void Point::Set(double dx, double dy)
{
	x = dx;
	y = dy;
}

//ALLOCATION (from another Point)
void Point::Set(const Point& other)
{
	x = other.x;
	y = other.y;
}

//EQUALITY TEST
bool Point::operator==(const Point& other) const
{
	if (x == other.x && y == other.y)
		return true;
	else return false;
}

//VECTOR ADDITION
Point Point::Sum(const Point& other) const
{
	return Point(x + other.x, y + other.y);
}

//VECTOR SUBTRACTION
Point Point::Subtr(const Point& other) const
{
	return Point(x - other.x, y - other.y);
}

//SCALAR MULTIPLICATION
Point Point::Mult(double dFactor) const
{
	return Point(x * dFactor, y * dFactor);
}

//DOT PRODUCT
double Point::dotP(const Point& other) const
{
	double soma = (x * other.x) + (y * other.y);
	return soma;
}

//NORMA (LENGTH)
double Point::Norma() const
{
	double soma = pow(x, 2.0) + pow(y, 2.0);
	return sqrt(soma);
}

//DISTANCE BETWEEN TWO POINTS
double Point::Dist(const Point& other) const
{
	Point d = Subtr(other);
	return d.Norma();
}

//ANGLE BETWEEN TWO VECTORS
double Point::Angle(const Point& other) const
{
	double cos = 0.0;
	cos = dotP(other) / (Norma() * other.Norma());
	return acos(cos);
}

//ORIENTED ANGLE BETWEEN VECTORS
double Point::oAngle() const
{
	Point u(1.0, 0.0);
	if (y < 0.0)
		return (2 * PI - Angle(u));
	else return Angle(u);
}

//CROSS PRODUCT
double Point::crossP(const Point& other) const
{
	return (x * other.y - y * other.x);
}

//COUNTER-CLOCKWISE
bool Point::CCW(const Point& other) const
{
	double AxB = crossP(other);

	if (AxB < 0) return false;
	else return true;
}
