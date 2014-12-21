#include "stdafx.h"
#include "Triangle.h"

using namespace course_Liryk_Anton;

Triangle::Triangle(void)//����������� �� ������������, �������� ��������� ���������� �� ���� �����
{
	vertexes = Array::CreateInstance(Point::typeid,4);

	halfPerimeter = 0.0f;
	sideAB = 0.0f;
	sideBC = 0.0f;
	sideAC = 0.0f;
	square = 0.0f;

	colorType = Color::Black;

	isIsosceles = false;
}

Triangle::Triangle(Point A, Point B, Point C)//����������� � �����������, �������� ����� ������ � ��������� ������� ������, ������� ������� ��������� ��� ���� �����
{
	vertexes = Array::CreateInstance(Point::typeid,4);

	vertexes->SetValue(A,0);
	vertexes->SetValue(B,1);
	vertexes->SetValue(C,2);
	vertexes->SetValue(A,3);

	halfPerimeter = 0.0f;
	sideAB = 0.0f;
	sideBC = 0.0f;
	sideAC = 0.0f;
	square = 0.0f;

	colorType = Color::Black;

	isIsosceles = false;

	prepareTiangle();
}

void Triangle::prepareTiangle()//�������� �� ���� �����
{
	calculateSides();//�������� ������� �����
	calculateHalfPerimeter();//�������� ����������
	calculateSquare();//�������� �����
	checkIsosceles();//�������� �� ������������ ���������
}

float Triangle::countVectorAbs(Point pA, Point pB)//���������� ������ ������� ��� ����������� ���� �������
{
	float sqrX = (pB.X - pA.X)*(pB.X - pA.X);
	float sqrY = (pB.Y - pA.Y)*(pB.Y - pA.Y);

	return System::Math::Sqrt(sqrX + sqrY);
}

void Triangle::calculateSides()
{//������� ����� �� ������ ����� ������
	sideAB = countVectorAbs((Point)vertexes->GetValue(0), (Point)vertexes->GetValue(1));
	sideBC = countVectorAbs((Point)vertexes->GetValue(0), (Point)vertexes->GetValue(2));
	sideAC = countVectorAbs((Point)vertexes->GetValue(1), (Point)vertexes->GetValue(2));
}

void Triangle::calculateHalfPerimeter()
{
	halfPerimeter = (sideAB + sideBC + sideAC)/2;
}

void Triangle::calculateSquare()
{
	square = System::Math::Sqrt(halfPerimeter*(halfPerimeter - sideAB)*(halfPerimeter - sideBC)*(halfPerimeter - sideAC));
}

void Triangle::checkIsosceles()
{
	isIsosceles = sideAB.Equals(sideBC) || sideBC.Equals(sideAC) || sideAC.Equals(sideAB);

	if(isIsosceles) colorType = Color::Aquamarine;//������ ������� �������� ���������� ����������
}

bool Triangle::intersection(Triangle ^trgl)
{
	bool isIntersection = false;

	Array^ triangleVertexes = trgl->getVertexes();

	for(int i = 0; i < 3 && !isIntersection; i++)//���� ��� ����� ���� ������ ������ ����������, ���� �� �������� ��������
	{
		Point vA1 = (Point)vertexes->GetValue(i);
		Point vA2 = (Point)vertexes->GetValue(i + 1);

		for(int k = 0; k < 3 && !isIntersection; k++)//���� ��� ����� ���� ������ ������ ����������, ���� �� �������� ��������
		{
			Point vB1 = (Point)triangleVertexes->GetValue(k);
			Point vB2 = (Point)triangleVertexes->GetValue(k + 1);

			isIntersection = isIntersectionOfTwoLines(vA1, vA2, vB1, vB2);//�������� �� ������������� ������ (�� ������������� �� ������� ����������)
		}
	}

	return isIntersection;
}

float Triangle::countIntersectionSquare(Triangle ^trgl)//���� ����� ������, ��� ���������� ��� ������� ����������
{
	float intersectionSquare = 0.0f;

	Array ^pointsOfIntersection = Array::CreateInstance(Point::typeid, 4);
	int counterFoundPoints = 0;

	Array^ triangleVertexes = trgl->getVertexes();

	bool isIntersection = false;

	for(int i = 0; i < 3 && counterFoundPoints < 4; i++)//�������� ����� ������� ��������
	{
		Point vA1 = (Point)vertexes->GetValue(i);
		Point vA2 = (Point)vertexes->GetValue(i + 1);

		for(int k = 0; k < 3 && counterFoundPoints < 4; k++)
		{
			Point vB1 = (Point)triangleVertexes->GetValue(k);
			Point vB2 = (Point)triangleVertexes->GetValue(k + 1);

			isIntersection = isIntersectionOfTwoLines(vA1, vA2, vB1, vB2);

			if(isIntersection)
			{
				pointsOfIntersection->SetValue(findIntersectionPoint(vA1, vA2, vB1, vB2), counterFoundPoints);

				counterFoundPoints++;
			}
		}
	}

	if(counterFoundPoints == 4)//���� �������� ������ ����� - �������� ����� �������������
	{
		intersectionSquare = findPolygonSquare(pointsOfIntersection);
	}
	else if(counterFoundPoints == 2)//���� ������� �� �����, ���� �����, ��� � ���� � ������ ����������
	{
		Point intersecP1 = (Point)pointsOfIntersection->GetValue(0);//������� ����� ��������
		Point intersecP2 = (Point)pointsOfIntersection->GetValue(1);

		bool otherTriangle = false;//���������, ���� ������ �� � ����� ����� � �������� ������� ����������

		for(int i = 0; i < 3 && !otherTriangle; i++)
		{
			Point vA1 = (Point)vertexes->GetValue(i);
			Point vA2 = (Point)vertexes->GetValue(i + 1);

			if(isPointBelongsToSide(intersecP1, vA1, vA2) && isPointBelongsToSide(intersecP2, vA1, vA2)) //���� ����� ����� �������� �������� ���� ������ ����� ����������, ������� ����� � �������� ������ ����������
			{
				otherTriangle = true;
			}
		}

		if(otherTriangle)
		{
			pointsOfIntersection->SetValue(findThirdVertex(triangleVertexes, intersecP1, intersecP2), 2);//������ �� ������ � ����� ����� �������� ����� �����
		}
		else 
		{
			pointsOfIntersection->SetValue(findThirdVertex(vertexes, intersecP1, intersecP2), 2);
		}

		counterFoundPoints++;//���������� �������� ��������� ����� � 3
	}

	if(counterFoundPoints == 3)//���� �������� 3 ����� ��������, ������� ��������� �� ������� ���� �����
	{
		Triangle ^tempTrgl = gcnew Triangle((Point)pointsOfIntersection->GetValue(0),(Point)pointsOfIntersection->GetValue(1),(Point)pointsOfIntersection->GetValue(2));

		intersectionSquare = tempTrgl->getSquare();
	}

	return intersectionSquare;
}

Point Triangle::findThirdVertex(Array ^trgl, Point p1, Point p2)
{
	Array ^sidesVertexes = Array::CreateInstance(Point::typeid, 4);

	int counter = 0;

	for(int i = 0; i < 3 && counter < 2; i++)//���� � ����� ���� �����, ���� �������� ����� ��������
	{
		Point vA1 = (Point)trgl->GetValue(i);
		Point vA2 = (Point)trgl->GetValue(i + 1);

		if(isPointBelongsToSide(p1, vA1, vA2) || isPointBelongsToSide(p2, vA1, vA2))
		{
			sidesVertexes->SetValue(vA1, counter);
			sidesVertexes->SetValue(vA2, counter + 1);

			counter++;
		}
	}

	for(int i = 0; i < 4; i++)//���� �� ��������� ���� ��� ����� ���� ����� ����� ���������� � ����� ���� �� ������� �������, ��� � � ������� ������� ������ ��������
	{
		Point current = (Point) sidesVertexes->GetValue(i);

		for(int k = 0; k < 4; k++)
		{
			Point other = (Point) sidesVertexes->GetValue(k);

			if(i != k)
			{
				if(current.Equals(other))
				{
					return current;
				}
			}
		}
	}

	return Point();
}

bool Triangle::isPointBelongsToSide(Point P, Point A, Point B)//��������, �� �������� ����� � ������
{
	return countVectorAbs(A, B) == countVectorAbs(A, P) + countVectorAbs(P, B); 
}

Point Triangle::findIntersectionPoint(Point A1, Point A2, Point B1, Point B2)
{	//�� ������������ �������� ���� ����� ��������
	float u1 = (B2.X - B1.X)*(A1.Y - B1.Y) - (B2.Y - B1.Y)*(A1.X - B1.X);
	float u2 = (B2.Y - B1.Y)*(A2.X - A1.X) - (B2.X - B1.X)*(A2.Y - A1.Y);
	float u = u1/u2;

	int x = A1.X + u*(A2.X - A1.X);
	int y = A1.Y + u*(A2.Y - A1.Y);

	return Point(x,y);
}

bool Triangle::isIntersectionOfTwoLines(Point A1, Point A2, Point B1, Point B2)
{//������� �� ������������ �������� �������� �� ������������� ��� ������
	float v1 = (B2.X - B1.X)*(A1.X - B1.Y) - (B2.Y - B1.Y)*(A1.X - B1.X);
	float v2 = (B2.Y - B1.Y)*(A2.X - A1.X) - (B2.X - B1.X)*(A2.Y - A1.Y);
	
	return (v1/v2 >= 0) && ( v1/v2 <= 1);
}

float Triangle::findPolygonSquare(Array ^pointsOfIntersection)
{	//��������� ����� ������������
	float s1 = countVectorAbs((Point)pointsOfIntersection->GetValue(0),(Point)pointsOfIntersection->GetValue(1));
	float s2 = countVectorAbs((Point)pointsOfIntersection->GetValue(1),(Point)pointsOfIntersection->GetValue(2));
	float s3 = countVectorAbs((Point)pointsOfIntersection->GetValue(2),(Point)pointsOfIntersection->GetValue(3));
	float s4 = countVectorAbs((Point)pointsOfIntersection->GetValue(0),(Point)pointsOfIntersection->GetValue(3));

	float p = (s1 + s2 + s3 + s4)/2;

	return Math::Sqrt((p - s1)*(p - s2)*(p - s3)*(p - s4));
}

void Triangle::draw(PaintEventArgs^ e)
{
	e->Graphics->DrawPolygon(gcnew Pen(colorType, 3.0f), (array<Point>^)vertexes);
}

float Triangle::getSquare() { return square; }

Array^ Triangle::getVertexes() { return vertexes; }