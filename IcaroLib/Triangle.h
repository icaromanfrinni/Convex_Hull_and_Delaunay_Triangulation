#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"
#include "Vector.h"
#include "Matrix.h"

class Triangle
{
public:
	//DEFAULT CONSTRUCTOR
	Triangle();

	//OVERLOAD CONSTRUCTOR (from three Points)
	Triangle(const Point& nPt1, const Point& nPt2, const Point& nPt3);
	//OVERLOAD CONSTRUCTOR (from another Triangle)
	Triangle(const Triangle& other);
	//DESTRUCTOR
	~Triangle();

	//ALLOCATION (from three Points)
	void Set(const Point& nPt1, const Point& nPt2, const Point& nPt3);
	//ALLOCATION (from another Triangle)
	void Set(const Triangle& other);

	//AREA
	double Area() const;
	//AREA SIGNED
	double AreaSigned() const;
	static double AreaSigned(const Point& nPt1, const Point& nPt2, const Point& nPt3);
	//CONTAINS (by Barycentric coordinate system)
	bool Contains(const Point& pTest) const;
	//REORDER
	void Reorder();
	//InCircle TEST
	bool InCircle(const Point& pTest);
    //CENTER OF THE CIRCLE (by 3 points)
    Point CircleCenter();

	//VARIABLES
	Point pt1, pt2, pt3;

};

#endif // TRIANGLE_H
