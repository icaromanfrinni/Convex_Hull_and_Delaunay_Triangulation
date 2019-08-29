/*  FUNCTION DECLARATIONS  */

#include <vector>
#include "Point.h"
#include "PointSet.h"
#include "Line.h"
#include "Triangle.h"
#include "ObjFile.h"

using namespace std;

#ifndef QUADEDGE_H
#define QUADEDGE_H

/*struct Vertex
{
	int id;
	Point pos;
};*/

struct Edge
{
	//int id;
	int vFrom;
	int vTo;
	bool alive;
};

struct Face
{
	vector<int> v;
};


class QuadEdge
{
public:
	//DEFAULT CONSTRUCTOR
	QuadEdge();
	//OVERLOAD CONSTRUCTOR (from PointSet)
	QuadEdge(const PointSet& dPoints);
	//OVERLOAD CONSTRUCTOR (from ObjFile)
	//QuadEdge(const obj& dObj);
	//DESTRUCTOR
	~QuadEdge();

	//ALLOCATION

	//INDICE (from Point)
	int idPoint(const Point& p);
	//BRUTE-FORCE TRIANGULATION
	void BruteForce();
	//DELAUNAY TRIANGULATION (by Iteration algorithm O(n²) )
	void Delaunay();
	//DELAUNAY TRIANGULATION (by Divide-and-Conquer algorithm O(n logn) )
	void Delaunay2() const;

	//VARIABLES
	vector<Point> Vertices;
	vector<Edge> Edges;
	vector<Face> Faces;
	vector<int> ConvexHull; //Face do fecho convexo (colocar como primeiro lugar no vector de faces)
    vector<Point> Voronoi;

private:
	PointSet C;
};

#endif // QUADEDGE_H
