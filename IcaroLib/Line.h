#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include "Vector.h"

class Line
{
public:
	//DEFAULT CONSTRUCTOR
	Line();

	//OVERLOAD CONSTRUCTOR (from another line)
	Line(const Line& other);
	//OVERLOAD CONSTRUCTOR (from point and vector)
	Line(const Point& pointFrom, const Vector& vectorTo);
	//OVERLOAD CONSTRUCTOR (from two points)
	Line(const Point& pointFrom, const Point& pointTo);
	//DESTRUCTOR
	~Line();

	//ALLOCATION (from another line)
	void Set(const Line& other);
	//ALLOCATION (from point and vector)
	void Set(const Point& pointFrom, const Vector& vectorTo);
	//ALLOCATION (from two points)
	void Set(const Point& pointFrom, const Point& pointTo);

	//EQUALITY TEST
	bool operator==(const Line& other) const;
	//RETURNS THE SECOND POINT
	Point GetPointTo() const;
	//LINE SEGMENT INTERSECTION
	bool LineLine(const Line& other) const;
	//DISTANCE FROM POINT
	double Dist(const Point& p) const;
	//LEFT SIDE POINT
	bool LeftSidePoint(const Point& p) const;

	//VARIABLES
	/*
	 * A linha pode ser formada por um VETOR
	 * partindo de um PONTO
	 * OU
	 * Entre dois PONTOS definidos
	 */
	Point point; //ponto de origem da linha
	Vector vector; //vetor na direção e magnitude da linha
};

#endif // LINE_H
