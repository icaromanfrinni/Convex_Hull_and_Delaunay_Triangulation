#ifndef ICAROVIEW_H
#define ICAROVIEW_H

#include "PointSet.h"
#include "ObjFile.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Pt2D
{
	GLfloat x, y;
};

struct Line2D
{
	Pt2D pFrom, pTo;
};

struct Face2D
{
	vector<Pt2D> v;
};

class icaroView
{
public:
	//DEFAULT CONSTRUCTOR
	icaroView();
	//OVERLOAD CONSTRUCTOR
	icaroView(const ObjFile& inObjFile);
	//DESTRUCTOR
	~icaroView();

	//ALLOCATION (edges)
	void SetEdges(const ObjFile& inObjFile);

	//INITIALIZE OPENGL WINDOW
	void DrawOpenGL();

private:
	//Objeto da classe GLFWwindow
	GLFWwindow* window;

	//VARIABLES
	vector<Pt2D> vPt2D; //vetor de pontos
	vector<Line2D> vLine2D; //vetor de arestas
	vector<Face2D> vFace2D; //vetor de faces
	vector<Pt2D> vPtVoronoi2D; //vetor de pontos de Voronoi

	//RESHAPE
	void Reshape();
	//DRAW POINTS
	void DrawPoints();
	//DRAW SPLINE
	void DrawSpline();
	//DRAW VORONOI POINTS
	void DrawVoronoiPoints();
	//DRAW EDGES
	void DrawEdges();
	//DRAW FACES
	void DrawFaces();
};

#endif // ICAROVIEW_H