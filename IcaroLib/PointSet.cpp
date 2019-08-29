/* FUNCTION DEFINITIONS */

#include <iostream>
#include <cmath>
#include "PointSet.h"

using namespace std;

//DEFAULT CONSTRUCTOR
PointSet::PointSet()
{
    //Initialize
    Points.clear();
}

//OVERLOAD CONSTRUCTOR
PointSet::PointSet(const vector<Point> &dPoints)
{
    Points = dPoints;
}

//OVERLOAD CONSTRUCTOR (from another PointSet)
PointSet::PointSet(const PointSet& other)
{
    Points = other.Points;
}

//DESTRUCTOR
PointSet::~PointSet()
{
}

//ALLOCATION
void PointSet::Set(const vector<Point> &dPoints)
{
    Points = dPoints;
}

//ALLOCATION (from another PointSet)
void PointSet::Set(const PointSet& other)
{
    Points = other.Points;
}

//ADD A NEW POINT AT THE END
void PointSet::AddPoint(const Point& newPoint)
{
	Points.push_back(newPoint);
}

//ADD A NEW SET OF POINTS AT THE END
void PointSet::AddPointSet(const PointSet& newPointSet)
{
	for (int i = 0; i < newPointSet.Points.size(); i++)
		Points.push_back(newPointSet.Points[i]);
}

//DELETE POINT
void PointSet::DelPoint(const Point& oldPoint)
{
	for (int i = 0; i < Points.size(); i++)
		if (Points[i] == oldPoint)
			Points.erase(Points.begin() + i);
}

//DELETE REPEAT POINT
void PointSet::DelRepeatPoint()
{
	for (int i = 0; i < Points.size(); i++)
		for (int j = i + 1; j < Points.size(); j++)
			if (Points[j] == Points[i])
				DelPoint(Points[j]);
}

//CONVEX HULL (by QuickHull algorithm)
PointSet PointSet::ConvexHull() const
{
	PointSet CH;
	
	//Procura os pontos extremos (esq/inf e dir/sup)
	Point ptLeftMost = Points[0];
	Point ptRightMost = Points[0];
	int iPtLeftMost = 0;
	int iPtRightMost = 0;
	for (int i = 1; i < Points.size(); i++)
	{
		//Procura o mais da esquerda inferior
		if (Points[i].x < ptLeftMost.x)
		{
			ptLeftMost = Points[i];
			iPtLeftMost = i;
		}
		else if (Points[i].x == ptLeftMost.x) {
			if (Points[i].y < ptLeftMost.y)
			{
				ptLeftMost = Points[i];
				iPtLeftMost = i;
			}
		}
	
		//Procura o mais da direita superior
		if (Points[i].x > ptRightMost.x)
		{
			ptRightMost = Points[i];
			iPtRightMost = i;
		}
		else if (Points[i].x == ptRightMost.x) {
			if (Points[i].y > ptRightMost.y)
			{
				ptRightMost = Points[i];
				iPtRightMost = i;
			}
		}
	}

	Line abEdge(ptLeftMost, ptRightMost);
	PointSet C1(LeftPoints(abEdge));
	Line baEdge(ptRightMost, ptLeftMost);
	PointSet C2(LeftPoints(baEdge));

	CH.QuickHull(C1, abEdge);
	CH.QuickHull(C2, baEdge);

	return CH;
}

//GET LEFT POINTS OF A LINE
PointSet PointSet::LeftPoints(const Line& edge) const
{
	PointSet C;

	for (int i = 0; i < Points.size(); i++)
		if (edge.LeftSidePoint(Points[i]) == true)
			C.AddPoint(Points[i]);

	return C;
}

//GET THE MOST LEFT DISTANCE POINT OF A LINE (CCW)
Point PointSet::MostDistPoint(const Line& edge) const
{
	Point pMostDist;
	double dMax = 0;
	for (int i = 0; i < Points.size(); i++)
		if (edge.Dist(Points[i]) > dMax)
		{
			dMax = edge.Dist(Points[i]);
			pMostDist.Set(Points[i]);
		}

	return pMostDist;
}

//QuickHull algorithm
void PointSet::QuickHull(const PointSet& C, const Line& edgeBase)
{
	if (C.Points.size() == 0)
	{
		AddPoint(edgeBase.point);
		return;
	}

	Point p(C.MostDistPoint(edgeBase));

	Line pbEdge(p, edgeBase.GetPointTo());
	PointSet C1(C.LeftPoints(pbEdge));
	Line apEdge(edgeBase.point, p);
	PointSet C2(C.LeftPoints(apEdge));

	QuickHull(C1, pbEdge);
	QuickHull(C2, apEdge);
}

