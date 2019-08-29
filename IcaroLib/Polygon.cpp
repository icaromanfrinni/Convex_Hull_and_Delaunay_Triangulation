/* FUNCTION DEFINITIONS */

#include <iostream>
#include <cmath>
#include "Polygon.h"
#include "PointSet.h"
#include "constants.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Polygon::Polygon()
{
    //Initialize
    Vertices.clear();
	Edges.clear();
}

//OVERLOAD CONSTRUCTOR
Polygon::Polygon(const vector<Point> &dVertices)
{
	//armazena vértices
	Vertices = dVertices;
	//cria arestas na sequência dos vértices
	for (int i = 0; i < dVertices.size(); i++)
	{
		int next = i + 1;
		if (next == dVertices.size())
			next = 0;
		Line edge(dVertices[i], dVertices[next]);
		Edges.push_back(edge);
	}
}

//OVERLOAD CONSTRUCTOR
Polygon::Polygon(const obj &dObj)
{
	//armazena vértices
	for (int i = 0; i < dObj.Face.size(); i++)
		Vertices.push_back(dObj.Vertices[i]);

	//cria arestas na sequência dos vértices
	for (int i = 0; i < Vertices.size(); i++)
	{
		int next = i + 1;
		if (next == Vertices.size())
			next = 0;
		Line edge(Vertices[i], Vertices[next]);
		Edges.push_back(edge);
	}
}

//OVERLOAD CONSTRUCTOR (from another Polygon)
Polygon::Polygon(const Polygon& other)
{
	Vertices = other.Vertices;
	Edges = other.Edges;
}

//DESTRUCTOR
Polygon::~Polygon()
{
}

//ALLOCATION
void Polygon::Set(const vector<Point> &dVertices)
{
	//armazena vértices
	Vertices = dVertices;
	//cria arestas na sequência dos vértices
	Edges.clear();
	for (int i = 0; i < dVertices.size(); i++)
	{
		int next = i + 1;
		if (next == dVertices.size())
			next = 0;
		Line edge(dVertices[i], dVertices[next]);
		Edges.push_back(edge);
	}
}

//ALLOCATION (from another Polygon)
void Polygon::Set(const Polygon& other)
{
	Vertices = other.Vertices;
	Edges = other.Edges;
}

//AREA
double Polygon::Area() const
{
	return fabs(AreaSigned());
}

//AREA SIGNED
double Polygon::AreaSigned() const
{
	return AreaSigned(Vertices);
}
double Polygon::AreaSigned(const vector<Point> &dVertices)
{
	double S = 0.0;

	for (int i = 0; i < dVertices.size() - 1; i++)
		S += dVertices[i].crossP(dVertices[i + 1]);
	S += dVertices[dVertices.size() - 1].crossP(dVertices[0]);

	S /= 2.0;
	return S;
}

//REVERSE ORDER
void Polygon::ReverseOrder()
{
	int n = Vertices.size();
	for (int i = 0; i < n; i++)
	{
		Point aux(Vertices[i]);
		Vertices[i] = Vertices[n - 1];
		Vertices[n - 1] = aux;
		n--;
	}
	//cria arestas na sequência dos vértices
	Edges.clear();
	for (int i = 0; i < Vertices.size(); i++)
	{
		int next = i + 1;
		if (next == Vertices.size())
			next = 0;
		Line edge(Vertices[i], Vertices[next]);
		Edges.push_back(edge);
	}
}

//REORDER BY ORIENTED ANGLE (Quicksort)
void Polygon::oAngleReorder(vector<Point> &dVertices, int began, int end)
{
    vector<double> oAngle;
    for (int i = 0; i < dVertices.size(); i++)
        oAngle.push_back(dVertices[i].oAngle());

    double auxAngle;
    Point auxPoint;
    int i = began;
    int j = end - 1;
    double pivo = oAngle[(began + end) / 2];

    while (i <= j)
    {
        while (oAngle[i] < pivo)
            i++;
        while (oAngle[j] > pivo)
            j--;
        if (i <= j)
        {
            auxAngle = oAngle[i];
            oAngle[i] = oAngle[j];
            oAngle[j] = auxAngle;

            auxPoint = dVertices[i];
            dVertices[i] = dVertices[j];
            dVertices[j] = auxPoint;

            i++;
            j--;
        }
    }
    if (j > began)
        oAngleReorder(dVertices, began, j + 1);
    if (i < end)
        oAngleReorder(dVertices, i, end);
}

//CONTAINS (by The Signed Angle method)
bool Polygon::Contains(const Point& pTest) const
{
	Point pA, pB;
	double angle = 0.0;
	double graus = 180 / PI;
	for (int i = 0; i < Vertices.size(); i++)
	{
		pA.Set(Vertices[i].Subtr(pTest));
		if (i == Vertices.size() - 1)
			pB.Set(Vertices[0].Subtr(pTest));
		else pB.Set(Vertices[i + 1].Subtr(pTest));

		if (pA.CCW(pB) == true)
			angle += pA.Angle(pB) * graus;
		else angle -= pA.Angle(pB) * graus;
	}
	if (angle < 180)
		return false;
	else return true;
}

//CONVEX HULL (by QuickHull)
Polygon Polygon::ConvexHull() const
{
	//Inicializar o Fecho Convexo com todos os pontos do polígono
	PointSet vFC(Vertices);
	//Extrair fecho convexo
	vFC.Set(vFC.ConvexHull());
	//Inicializar polígono com vértices do fecho
	Polygon FC(vFC.Points);

	return FC;
}

//NEXT TO OTHER
bool Polygon::NextTo(const Polygon& other) const
{
	for (int i = 0; i < Edges.size(); i++)
		for (int j = 0; j < other.Edges.size(); j++)
			if (Edges[i] == other.Edges[j])
				return true;
	return false;
}

//MERGE TWO CLOSE POLYGONS
Polygon Polygon::Merge(const Polygon& other) const
{
	vector<Point> Union;

	for (int i = 0; i < Edges.size(); i++)
		for (int j = 0; j < other.Edges.size(); j++)
		{
			if (Edges[i] == other.Edges[j])
			{
				for (int k = 0; k < Vertices.size() - 1; k++)
				{
					i++;
					if (i == Vertices.size())
						i = 0;
					Union.push_back(Vertices[i]);
				}
				for (int k = 0; k < other.Vertices.size() - 1; k++)
				{
					j++;
					if (j == other.Vertices.size())
						j = 0;
					Union.push_back(other.Vertices[j]);
				}
				Polygon U(Union);
				return U;
			}
		}
}