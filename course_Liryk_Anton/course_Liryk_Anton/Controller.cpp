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
	std::ifstream ifs(path); //створення файлового потоку вводу

    std::vector<int*> coords;//створення структури вектору, у яку додаватимуться координати

	std::getline(ifs,(string)"");

	int rectanglesCount = 0;

    if(ifs.is_open())//якщо файловий опток відкрито
    {
        while (! ifs.eof() )//доки не досягнуто кінця файлу
        {
            std::string line = "";//оголошення та ініціалізація стрічки для зчитування з файлу
			std::getline(ifs,line);//отримання однієї стрічки з файлу
			
			int* coordsFromLine = new int[2];//оголошення та ініціалізація масиву для збереження зчитаних значень координати
			findCoordsInLine(line, coordsFromLine);//знайти координати в стрічці

			if(coordsFromLine[0] >= 0 && coordsFromLine[1] >= 0)//якщо координату знайдено
			{
				coords.push_back(coordsFromLine);//додати координату в вектор
			}	
        }
		int coordsNumber = coords.size();

		Array ^points = Array::CreateInstance(Point::typeid,coordsNumber);//ініціалізація масиву з даними типу Point

		for (int i = 0; i < coordsNumber; i++)
        {
            points->SetValue(Point(coords[i][0],coords[i][1]),i);//додати у масив точок нову створену точку з двох послідовних значень осей ординати та абсциси
        }

		createTriangles(points);//створити трикутники
    }

    ifs.close();//закриття файлового потоку
}

void Controller::findCoordsInLine(std::string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //знайти позицію першого символа після першого входження символа "="
	size_t posY = line.find_last_of("=") + 1;//знайти позицію першого символа після останнього входження символа "="

	coords[0] = -1;//ініціалізувати значеннями масив точок для ідентифікації відсутності знайдених координат
	coords[1] = -1;

	if(posX > 0 && posY > 0)//якщо символи знайдено
	{
		size_t countX = line.find_first_of(",") - posX;//знайти індекс першого входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа
		size_t countY = line.length() - posY;//знайти індекс останнього входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа		

		if(countX > 0 && countY > 0)//якщо символи знайдено
		{
			int x = stoi(line.substr(posX,countX));//перетворити в число потрібну частину стрічки з координатами
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//заповнити масив значеннями
			coords[1] = y;
		}
	}
}

void Controller::createTriangles(Array ^points)//створити трикутники
{
	int length = points->Length;//отримати кількість точок (вершин трикутників)

	triangles = Array::CreateInstance(Triangle::typeid,length/3);//ініціалізувати масив трикутників розміром в 3 рази менше кількості точок

	for(int i = 0; i < length; i += 3)
	{
		triangles->SetValue(gcnew Triangle((Point)points->GetValue(i), (Point)points->GetValue(i + 1), (Point)points->GetValue(i + 2)), i/3);//створити та додати в масив трикутники з трьох послідовних вершин
	}
}

void Controller::drawTriangles(PaintEventArgs^ e)//намалювати усі трикутники з масиву трикутників
{
	for(int i = 0; i < triangles->Length; i++)
	{
		((Triangle^)triangles->GetValue(i))->draw(e);
	}
}

void Controller::findTheBiggestIntersection()//шукає трикутники, які перетинаються та мають найбільшу сумарну площу
{
	Triangle ^trgl1 = (Triangle^)triangles->GetValue(0);//оголосити і ініціалізувати початкові вказівники на трикутники
	Triangle ^trgl2 = (Triangle^)triangles->GetValue(1);

	float totalSquare = 0.0f;//оголосити і ініціалізувати початкове значення суми площ двох трикутників

	int length = triangles->Length;//отримати кількість трикутників

	for(int i = 0; i < length; i++)//цикл проходження по всім трикутникам
	{
		Triangle ^currentTriangle1 = (Triangle^)triangles->GetValue(i);//отримати перший трикутник для порівняння

		for(int j = 0; j < length; j++)//цикл проходження по всім трикутникам
		{
			Triangle ^currentTriangle2 = (Triangle^)triangles->GetValue(j);//отримати другий трикутник для порівняння

			if(currentTriangle1 != currentTriangle2)//якщо вказівники вказують не на один і той же трикутник
			{
				if(currentTriangle1->intersection(currentTriangle2))//перевірити чи перетинаються трикутники
				{
					float currentTotalSquare = countTotalSquare(currentTriangle1, currentTriangle2, currentTriangle1->countIntersectionSquare(currentTriangle2));//обчислити сумарну площу двох трикутників

					if(currentTotalSquare > totalSquare)//якщо сумарна площа більша поточної, оновити трикутники та значення площі
					{
						totalSquare = currentTotalSquare;

						trgl1 = currentTriangle1;
						trgl2 = currentTriangle2;
					}
				}
			}
		}
	}

	if(totalSquare != 0.0f)//якщо знайдено перетин трикуників з найбільшою сумарною площею
	{
		trgl1->colorType = Color::DarkOrange;//виділити їх окремим кольором
		trgl2->colorType = Color::DarkOrange;
	}
}

float Controller::countTotalSquare(Triangle ^trgl1, Triangle ^trgl2, float squareIntersection)//обчислює сумарну площу трикутників як суму їх площ мінус площа пересічення
{
	return trgl1->getSquare() + trgl2->getSquare() - squareIntersection;
}