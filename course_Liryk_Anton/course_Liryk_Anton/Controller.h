#pragma once

#include "stdafx.h"
#include "Triangle.h"

using namespace std;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace course_Liryk_Anton 
{
	ref class Controller
	{
		Array ^triangles;

		void createTriangles(Array ^points);		

		void readCoordsFromFile(string path);
		void findCoordsInLine(std::string line, int* coords);
		void findTheBiggestIntersection();
		float countTotalSquare(Triangle ^trgl1, Triangle ^trgl2, float squareIntersection);

		void prepareController();
	public:
		Controller(void);

		void drawTriangles(PaintEventArgs^ e);
	};
}

