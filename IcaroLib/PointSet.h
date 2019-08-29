/*  FUNCTION DECLARATIONS  */

#include <vector>
#include "Point.h"
#include "Line.h"

using namespace std;

#ifndef POINTSET_H
#define POINTSET_H

class PointSet
{
public:
    //DEFAULT CONSTRUCTOR
    PointSet();

    //OVERLOAD CONSTRUCTOR
    PointSet(const vector<Point> &dPoints);
    //OVERLOAD CONSTRUCTOR (from another PointSet)
    PointSet(const PointSet& other);
    //DESTRUCTOR
    ~PointSet();

    //ALLOCATION
    void Set(const vector<Point> &dPoints);
    //ALLOCATION (from another PointSet)
    void Set(const PointSet& other);

	//ADD A NEW POINT AT THE END
	void AddPoint(const Point& newPoint);
	//ADD A NEW SET OF POINTS AT THE END
	void AddPointSet(const PointSet& newPointSet);
	//DELETE POINT
	void DelPoint(const Point& oldPoint);
	//DELETE REPEAT POINT
	void DelRepeatPoint();
	//CONVEX HULL (by QuickHull algorithm)
	PointSet ConvexHull() const;
	//GET LEFT POINTS OF A LINE
	PointSet LeftPoints(const Line& edge) const;

    //VARIABLES
    vector<Point> Points;

private:
	//GET THE MOST LEFT DISTANCE POINT OF A LINE (CCW)
	Point MostDistPoint(const Line& edge) const;
	//GET POINTS OF CONVEX HULL (QuickHull algorithm)
	void QuickHull(const PointSet& C, const Line& edgeBase);
};

#endif // POINTSET_H
