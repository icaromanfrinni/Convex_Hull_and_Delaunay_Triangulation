/*  FUNCTION DECLARATIONS  */

#include <vector>
#include "Point.h"
#include "Line.h"
#include "ObjFile.h"

using namespace std;

#ifndef POLYGON_H
#define POLYGIN_H

class Polygon
{
public:
    //DEFAULT CONSTRUCTOR
    Polygon();

    //OVERLOAD CONSTRUCTOR (from Vertices)
    Polygon(const vector<Point> &dVertices);
	//OVERLOAD CONSTRUCTOR (from ObjFile)
	Polygon(const obj &dObj);
    //OVERLOAD CONSTRUCTOR (from another Polygon)
    Polygon(const Polygon& other);
    //DESTRUCTOR
    ~Polygon();

    //ALLOCATION
    void Set(const vector<Point> &dVertices);
    //ALLOCATION (from another Polygon)
    void Set(const Polygon& other);

    //AREA
    double Area() const;
    //AREA SIGNED
    double AreaSigned() const;
    static double AreaSigned(const vector<Point> &dVertices);
    //REVERSE ORDER
	void ReverseOrder();
    //REORDER (by Oriented Angle)
    void oAngleReorder(vector<Point> &dVertices, int began, int end);
	//CONTAINS (by The Signed Angle method)
	bool Contains(const Point& pTest) const;
	//CONVEX HULL (by QuickHull)
	Polygon ConvexHull() const;
	//NEXT TO OTHER
	bool NextTo(const Polygon& other) const;
	//MERGE TWO CLOSE POLYGONS
	Polygon Merge(const Polygon& other) const;

    //VARIABLES
    vector<Point> Vertices;
	vector<Line> Edges;
};

//void LeftPointSet(vector<Point> &dPointSet, Point& pLeft, Point& pRight);
//void QuickHull(vector<Point> &dPointSet, Point& pLeft, Point& pRight);

#endif // POLYGON_H
