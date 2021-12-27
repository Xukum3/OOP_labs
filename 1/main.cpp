/*Вариант 4. Верзьера Аньези
Разработать класс, определяющий кривую – верзьера Аньези.

Верьзьера Аньези определяется следующим образом : на оси OX выбирается точка A, на отрезке OA, как на диаметре строится окружность.На оси ОY выбирается точка B, через нее проводится полухорда BC,
параллельная OX.Данная хорда продолжается до точки M, определяемой соотношением BM : BC = OA : OB.Геометрическое место точек M есть верзьера Аньези.
1) Определить состояние класса.
2) Разработать необходимые конструкторы и методы получения и изменения параметров, определяющих кривую.
3) Вернуть координату y в декартовой системе координат, принадлежащую верзьере в зависимости от x.
4) Вернуть координаты точек перегиба верзьеры.
5) Вернуть площадь полосы между верзьерой и ее асимптотой.
6) Вернуть объем тела, полученной вращением полосы из пункта 6 вокруг асимптоты.
7) Вернуть текстовое представление уравнения верзьеры в декартовой системе координат в виде строки(char[] или wchar_t[]).
Разработать диалоговую программу для тестирования класса.*/

#include "Agnesi.h"
#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

short int Get_num(double& a);

int main() {
	Agnesi Test(sqrt(3));
	bool out = 0;
	while (!out) {
		cout << endl << "Agnesi info:" << endl;
		cout << "Parameter: " << Test.GetParam() << endl;

		char* Eq = Test.Eq();
		cout << "Equation: " << Eq << endl;
		delete[] Eq;

		cout << "Area: " << Test.FindSquare() << endl;
		cout << "Volume: " << Test.FindVolume() << endl;

		auto p = Test.FindInfl();
		cout << "Inflections Points: " << p.first << " " << p.second << endl;

		double x;
		cout << "Enter x coordinate: "; 
		short int res = Get_num(x);
		if (!res) break;
		cout << "Y = " << Test.FindY(x) << endl;

		while (1) {
			double param;
			cout << "Enter new parameter or ^Z to exit: (!= 0): ";
			short int a = Get_num(param);
			if (!a) {
				out = 1;
				break;
			}
			try {
				Test.SetParam(param);
				break;
			}
			catch(std::exception &ex){
				cout << ex.what() << endl;
			}
		}
	}
	cout << "Good bye";
	return 0;
}


short int Get_num(double& a) {
	while (1) {
		cin >> a;
		if (!cin.good()) {
			if (cin.eof()) return 0; //^Z
			cout << "You entered not an int number! Please repeat: ";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		return 1;
	}
}