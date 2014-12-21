#pragma once
#include "stdafx.h"

using namespace std;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace course_Liryk_Anton 
{
	ref class Triangle
	{
		Array ^vertexes;
		float halfPerimeter;
		float sideAB, sideBC, sideAC;
		float square;
		bool isIsosceles;

		float countVectorAbs(Point pA, Point pB);
		
		bool isIntersectionOfTwoLines(Point A1, Point A2, Point B1, Point B2);

		void calculateSides();
		void calculateHalfPerimeter();
		void calculateSquare();
		void checkIsosceles();
		void prepareTiangle();
		
		bool isPointBelongsToSide(Point P, Point A, Point B);
		Point findIntersectionPoint(Point A1, Point A2, Point B1, Point B2);
		Point findThirdVertex(Array ^trgl, Point p1, Point p2);
		float findPolygonSquare(Array ^pointsOfIntersection);

	public:		
		Color colorType;

		Triangle(void);
		Triangle(Point A, Point B, Point C);

		bool intersection(Triangle ^trgl);
		float countIntersectionSquare(Triangle ^trgl);
		float getSquare();
		Array^ getVertexes();
		void draw(PaintEventArgs^ e);
	};
}