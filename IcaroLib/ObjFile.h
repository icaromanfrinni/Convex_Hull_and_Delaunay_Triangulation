#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include "PointSet.h"
#include "QuadEdge.h"

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct objLine
{
	int vFrom;
	int vTo;
};

struct objFace
{
	vector<int> vInd;
};

struct obj
{
	string Name;
	vector<Point> Vertices;
	vector<objLine> Lines;
	vector<int> Face;
	vector<Point> Voronoi;
};

//------------------------------------------------------------------------------
// CLASSES
//------------------------------------------------------------------------------

class ObjFile
{
public:
	//DEFAULT CONSTRUCTOR
	ObjFile();
	//DEFAULT CONSTRUCTOR (from QuadEdge)
	ObjFile(const QuadEdge& qEdge);

	//DESTRUCTOR
	~ObjFile();

	//CLEAR A OBJ VARIABLE TYPE
	void ClearCurrentObj();
	//READ .OBJ FILE
	bool ReadObjFile();
	//WRITE .OBJ FILE
	void WriteObjFile();

	//READ POINTS .OBJ FILE
	//bool ReadObjFile(PointSet& pS);

	//WRITE ALL POINTS .OBJ FILE
	//void WriteAllPointsObjFile(const PointSet& pS);
	//WRITE CONVEX HULL .OBJ FILE
	//void WriteConvexHullObjFile(const PointSet& pS);

	//VARIABLES
	vector<obj> vObj; //vector of objets from OBJ File

private:
	
	//VARIABLES
	bool object;
	obj cObj; //current object from OBJ File
	string fName;
};

#endif // OBJFILE_H