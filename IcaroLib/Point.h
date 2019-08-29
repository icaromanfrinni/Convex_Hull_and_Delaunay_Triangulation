/*  FUNCTION DECLARATIONS   */

#ifndef POINT_H
#define POINT_H

class Point
{
public:
	//DEFAULT CONSTRUCTOR
	Point();

	//OVERLOAD CONSTRUCTOR
	Point(double dx, double dy);
	//OVERLOAD CONSTRUCTOR (from another Point)
	Point(const Point& other);
	//DESTRUCTOR
	~Point();

	//ALLOCATION
	void Set(double dx, double dy);
	//ALLOCATION (from another Point)
	void Set(const Point& other);

	//EQUALITY TEST
	bool operator==(const Point& other) const;
	//VECTOR ADDITION
	Point Sum(const Point& other) const;
	//VECTOR SUBTRACTION
	Point Subtr(const Point& other) const;
	//SCALAR MULTIPLICATION
	Point Mult(double dFactor) const;
	//DOT PRODUCT
	double dotP(const Point& other) const;
	//NORMA (length)
	double Norma() const;
	//DISTANCE BETWEEN TWO POINTS
	double Dist(const Point& other) const;
	//ANGLE BETWEEN TWO VECTORS
	double Angle(const Point& other) const;
	//ORIENTED ANGLE BETWEEN VECTORS
	double oAngle() const;
	//CROSS PRODUCT
	double crossP(const Point& other) const;
	//COUNTER-CLOCKWISE
	bool CCW(const Point& other) const;

	//VARIABLES
	double x;
	double y;
};

#endif // POINT_H
