#include <iostream>
#include <cmath>
#include "Triangle.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Triangle::Triangle()
{
	pt1.x = pt1.y = 0;
	pt2.x = pt2.y = 0;
	pt3.x = pt3.y = 0;
}

//OVERLOAD CONSTRUCTOR (from three Points)
Triangle::Triangle(const Point& nPt1, const Point& nPt2, const Point& nPt3)
{
	pt1 = nPt1;
	pt2 = nPt2;
	pt3 = nPt3;
}

//OVERLOAD CONSTRUCTOR (from another Triangle)
Triangle::Triangle(const Triangle& other)
{
	pt1 = other.pt1;
	pt2 = other.pt2;
	pt3 = other.pt3;
}

//DESTRUCTOR
Triangle::~Triangle()
{

}

//ALLOCATION (from three Points)
void Triangle::Set(const Point &nPt1, const Point &nPt2, const Point &nPt3)
{
	pt1 = nPt1;
	pt2 = nPt2;
	pt3 = nPt3;
}

//ALLOCATION (from another Triangle)
void Triangle::Set(const Triangle& other)
{
	pt1 = other.pt1;
	pt2 = other.pt2;
	pt3 = other.pt3;
}

//AREA
double Triangle::Area() const
{
	return fabs(AreaSigned());
}

//AREA SIGNED
double Triangle::AreaSigned() const
{
	return AreaSigned(pt1, pt2, pt3);
}
double Triangle::AreaSigned(const Point& nPt1, const Point& nPt2, const Point& nPt3)
{
	double tArea = 0.0;

	tArea = nPt1.crossP(nPt2) + nPt2.crossP(nPt3) + nPt3.crossP(nPt1);
	tArea /= 2.0;

	return tArea;
}

//CONTAINS (by Barycentric coordinate system)
bool Triangle::Contains(const Point& pTest) const
{
	double S = AreaSigned();
	double S1 = AreaSigned(pt2, pt3, pTest);
	double S2 = AreaSigned(pt3, pt1, pTest);
	double S3 = AreaSigned(pt1, pt2, pTest);

	if ((S1 / S) < 0) return false;
	else if ((S2 / S) < 0) return false;
	else if ((S3 / S) < 0) return false;

	return true;
}

//REORDER (switch first point to last point)
void Triangle::Reorder()
{
	Point aux(pt1);
	pt1.Set(pt3);
	pt3.Set(aux);
}

//InCircle TEST
bool Triangle::InCircle(const Point& pTest)
{
	if (AreaSigned() < 0)
		Reorder();

	//Initialize
	Matrix D(4, 4);
	D.e[0][0] = pt1.x;		D.e[0][1] = pt1.y;		D.e[0][2] = pow(pt1.x, 2.0) + pow(pt1.y, 2.0);		D.e[0][3] = 1.0;
	D.e[1][0] = pt2.x;		D.e[1][1] = pt2.y;		D.e[1][2] = pow(pt2.x, 2.0) + pow(pt2.y, 2.0);		D.e[1][3] = 1.0;
	D.e[2][0] = pt3.x;		D.e[2][1] = pt3.y;		D.e[2][2] = pow(pt3.x, 2.0) + pow(pt3.y, 2.0);		D.e[2][3] = 1.0;
	D.e[3][0] = pTest.x;	D.e[3][1] = pTest.y;	D.e[3][2] = pow(pTest.x, 2.0) + pow(pTest.y, 2.0);	D.e[3][3] = 1.0;

	if (D.det() > 0)
		return true;
	else return false;
}

//CENTER OF THE CIRCLE (by 3 points)
Point Triangle::CircleCenter()
{
	/*
	 * https://math.stackexchange.com/questions/213658/get-the-equation-of-a-circle-when-given-3-points
	 */

    //Initialize
    Matrix M11(3, 3);
    M11.e[0][0] = pt1.x;    M11.e[0][1] = pt1.y;    M11.e[0][2] = 1;
    M11.e[1][0] = pt2.x;    M11.e[1][1] = pt2.y;    M11.e[1][2] = 1;
    M11.e[2][0] = pt3.x;    M11.e[2][1] = pt3.y;    M11.e[2][2] = 1;
    Matrix M12(3, 3);
    M12.e[0][0] = pow(pt1.x, 2.0) + pow(pt1.y, 2.0);    M12.e[0][1] = pt1.y;    M12.e[0][2] = 1;
    M12.e[1][0] = pow(pt2.x, 2.0) + pow(pt2.y, 2.0);    M12.e[1][1] = pt2.y;    M12.e[1][2] = 1;
    M12.e[2][0] = pow(pt3.x, 2.0) + pow(pt3.y, 2.0);    M12.e[2][1] = pt3.y;    M12.e[2][2] = 1;
    Matrix M13(3, 3);
	M13.e[0][0] = pow(pt1.x, 2.0) + pow(pt1.y, 2.0);    M13.e[0][1] = pt1.x;    M13.e[0][2] = 1;
	M13.e[1][0] = pow(pt2.x, 2.0) + pow(pt2.y, 2.0);    M13.e[1][1] = pt2.x;    M13.e[1][2] = 1;
	M13.e[2][0] = pow(pt3.x, 2.0) + pow(pt3.y, 2.0);    M13.e[2][1] = pt3.x;    M13.e[2][2] = 1;
    Matrix M14(3, 3);
	M14.e[0][0] = pow(pt1.x, 2.0) + pow(pt1.y, 2.0);    M14.e[0][1] = pt1.x;    M14.e[0][2] = pt1.y;
	M14.e[1][0] = pow(pt2.x, 2.0) + pow(pt2.y, 2.0);    M14.e[1][1] = pt2.x;    M14.e[1][2] = pt2.y;
	M14.e[2][0] = pow(pt3.x, 2.0) + pow(pt3.y, 2.0);    M14.e[2][1] = pt3.x;    M14.e[2][2] = pt3.y;

	//Center of the Circle
	Point pt0;
	pt0.x = 0.5 * (M12.det() / M11.det());
	pt0.y = -0.5 * (M13.det() / M11.det());

	return pt0;
}
