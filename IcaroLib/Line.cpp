#include <iostream>
#include <cmath>
#include "constants.h"
#include "Line.h"
#include "Point.h"
#include "Vector.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Line::Line()
{
	point.x = 0.0;
	point.y = 0.0;
	vector.i = 0.0;
	vector.j = 0.0;
}

//OVERLOAD CONSTRUCTOR (from another line)
Line::Line(const Line& other)
{
	point = other.point;
	vector = other.vector;
}

//OVERLOAD CONSTRUCTOR (from point and vector)
Line::Line(const Point& pointFrom, const Vector& vectorTo)
{
	point = pointFrom;
	vector = vectorTo;
}

//OVERLOAD CONSTRUCTOR (from two points)
Line::Line(const Point& pointFrom, const Point& pointTo)
{
	point = pointFrom;
	vector.Set(pointFrom, pointTo);
}

//DESTRUCTOR
Line::~Line()
{

}

//ALLOCATION (from another line)
void Line::Set(const Line& other)
{
	point = other.point;
	vector = other.vector;
}

//ALLOCATION (from point and vector)
void Line::Set(const Point& pointFrom, const Vector& vectorTo)
{
	point = pointFrom;
	vector = vectorTo;
}

//ALLOCATION (from two points)
void Line::Set(const Point& pointFrom, const Point& pointTo)
{
	point = pointFrom;
	vector.Set(pointFrom, pointTo);
}

//EQUALITY TEST
bool Line::operator==(const Line& other) const
{
	if (point == other.point)
	{
		if (GetPointTo() == other.GetPointTo())
			return true;
		else return false;
	}
	else {
		if (point == other.GetPointTo())
		{
			if (GetPointTo() == other.point)
				return true;
			else return false;
		}
		else return false;
	}
}

//RETURNS THE SECOND POINT
Point Line::GetPointTo() const
{
	return Point(point.x + vector.i, point.y + vector.j);
}

//LINE SEGMENT INTERSECTION
//*** OBS: verificar se tem pontos coincidentes
bool Line::LineLine(const Line &other) const
{
	Point a = point;
	Point b = GetPointTo();
	Point c = other.point;
	Point d = other.GetPointTo();

	Point vectorAB = b.Subtr(a);
	Point vectorAC = c.Subtr(a);
	Point vectorAD = d.Subtr(a);
	double prod1 = vectorAB.crossP(vectorAC) * vectorAB.crossP(vectorAD); //(ABxAC)*(ABxAD)

	Point vectorCD = d.Subtr(c);
	Point vectorCA = a.Subtr(c);
	Point vectorCB = b.Subtr(c);
	double prod2 = vectorCD.crossP(vectorCA) * vectorCD.crossP(vectorCB); //(CDxCA)*(CDxCB)

	if (prod1 < 0 && prod2 < 0)
		return true;
	else return false;
}

//DISTANCE FROM POINT
double Line::Dist(const Point& p) const
{	
	Point pA(vector.i, vector.j);
	Point pB = p.Subtr(point);
	double alpha = pA.Angle(pB);
	double d = pB.Norma()*sin(alpha);

	return d;
}

//LEFT SIDE POINT
bool Line::LeftSidePoint(const Point& p) const
{
	Point vLine(GetPointTo().Subtr(point));
	Point vP(p.Subtr(point));

	if (vLine.crossP(vP) > 0)
		return true;
	else return false;
}