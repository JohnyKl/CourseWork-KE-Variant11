#include "stdafx.h"
#include "Controller.h"
#include "Triangle.h"

using namespace course_Liryk_Anton;

Controller::Controller(void)
{
	prepareController();
}

void Controller::prepareController()
{
	readCoordsFromFile("coords.txt");
	findTheBiggestIntersection();
}

void Controller::readCoordsFromFile(string path)
{
	std::ifstream ifs(path); //��������� ��������� ������ �����

    std::vector<int*> coords;//��������� ��������� �������, � ��� �������������� ����������

	std::getline(ifs,(string)"");

	int rectanglesCount = 0;

    if(ifs.is_open())//���� �������� ����� �������
    {
        while (! ifs.eof() )//���� �� ��������� ���� �����
        {
            std::string line = "";//���������� �� ����������� ������ ��� ���������� � �����
			std::getline(ifs,line);//��������� ���� ������ � �����
			
			int* coordsFromLine = new int[2];//���������� �� ����������� ������ ��� ���������� �������� ������� ����������
			findCoordsInLine(line, coordsFromLine);//������ ���������� � ������

			if(coordsFromLine[0] >= 0 && coordsFromLine[1] >= 0)//���� ���������� ��������
			{
				coords.push_back(coordsFromLine);//������ ���������� � ������
			}	
        }
		int coordsNumber = coords.size();

		Array ^points = Array::CreateInstance(Point::typeid,coordsNumber);//����������� ������ � ������ ���� Point

		for (int i = 0; i < coordsNumber; i++)
        {
            points->SetValue(Point(coords[i][0],coords[i][1]),i);//������ � ����� ����� ���� �������� ����� � ���� ���������� ������� ���� �������� �� �������
        }

		createTriangles(points);//�������� ����������
    }

    ifs.close();//�������� ��������� ������
}

void Controller::findCoordsInLine(std::string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //������ ������� ������� ������� ���� ������� ��������� ������� "="
	size_t posY = line.find_last_of("=") + 1;//������ ������� ������� ������� ���� ���������� ��������� ������� "="

	coords[0] = -1;//������������ ���������� ����� ����� ��� ������������� ��������� ��������� ���������
	coords[1] = -1;

	if(posX > 0 && posY > 0)//���� ������� ��������
	{
		size_t countX = line.find_first_of(",") - posX;//������ ������ ������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����
		size_t countY = line.length() - posY;//������ ������ ���������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����		

		if(countX > 0 && countY > 0)//���� ������� ��������
		{
			int x = stoi(line.substr(posX,countX));//����������� � ����� ������� ������� ������ � ������������
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//��������� ����� ����������
			coords[1] = y;
		}
	}
}

void Controller::createTriangles(Array ^points)//�������� ����������
{
	int length = points->Length;//�������� ������� ����� (������ ����������)

	triangles = Array::CreateInstance(Triangle::typeid,length/3);//������������ ����� ���������� ������� � 3 ���� ����� ������� �����

	for(int i = 0; i < length; i += 3)
	{
		triangles->SetValue(gcnew Triangle((Point)points->GetValue(i), (Point)points->GetValue(i + 1), (Point)points->GetValue(i + 2)), i/3);//�������� �� ������ � ����� ���������� � ����� ���������� ������
	}
}

void Controller::drawTriangles(PaintEventArgs^ e)//���������� �� ���������� � ������ ����������
{
	for(int i = 0; i < triangles->Length; i++)
	{
		((Triangle^)triangles->GetValue(i))->draw(e);
	}
}

void Controller::findTheBiggestIntersection()//���� ����������, �� ������������� �� ����� �������� ������� �����
{
	Triangle ^trgl1 = (Triangle^)triangles->GetValue(0);//��������� � ������������ �������� ��������� �� ����������
	Triangle ^trgl2 = (Triangle^)triangles->GetValue(1);

	float totalSquare = 0.0f;//��������� � ������������ ��������� �������� ���� ���� ���� ����������

	int length = triangles->Length;//�������� ������� ����������

	for(int i = 0; i < length; i++)//���� ����������� �� ��� �����������
	{
		Triangle ^currentTriangle1 = (Triangle^)triangles->GetValue(i);//�������� ������ ��������� ��� ���������

		for(int j = 0; j < length; j++)//���� ����������� �� ��� �����������
		{
			Triangle ^currentTriangle2 = (Triangle^)triangles->GetValue(j);//�������� ������ ��������� ��� ���������

			if(currentTriangle1 != currentTriangle2)//���� ��������� �������� �� �� ���� � ��� �� ���������
			{
				if(currentTriangle1->intersection(currentTriangle2))//��������� �� ������������� ����������
				{
					float currentTotalSquare = countTotalSquare(currentTriangle1, currentTriangle2, currentTriangle1->countIntersectionSquare(currentTriangle2));//��������� ������� ����� ���� ����������

					if(currentTotalSquare > totalSquare)//���� ������� ����� ����� �������, ������� ���������� �� �������� �����
					{
						totalSquare = currentTotalSquare;

						trgl1 = currentTriangle1;
						trgl2 = currentTriangle2;
					}
				}
			}
		}
	}

	if(totalSquare != 0.0f)//���� �������� ������� ��������� � ��������� �������� ������
	{
		trgl1->colorType = Color::DarkOrange;//������� �� ������� ��������
		trgl2->colorType = Color::DarkOrange;
	}
}

float Controller::countTotalSquare(Triangle ^trgl1, Triangle ^trgl2, float squareIntersection)//�������� ������� ����� ���������� �� ���� �� ���� ���� ����� ����������
{
	return trgl1->getSquare() + trgl2->getSquare() - squareIntersection;
}