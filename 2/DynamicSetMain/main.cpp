#include <iostream>
#include "../SetDynamic/DynSet.h"
#include <limits>

using namespace std;

template <typename T>
short int Get_num(T& a);

int main() {
	bool out = false;
	short int answer;
	unsigned int size;
	Set *result;
	Set a{ -1, 1, 0 };
	Set b(9);
	cout << "default set 1:" << a << endl;
	cout << "default set 2:" << b;

	while (!out) {
		cout << "\n\nWhat do you want to do?: ";
		cout << "\n\t0 show sets";
		cout << "\n\t1 update set1\n\t2 update set2";
		cout << "\n\t3 insert in set1\n\t4 insert in set2";
		cout << "\n\t5 find value in set 1\n\t6 find value in set2";
		cout << "\n\t7 Union set1 and set2\n\t8 Intersect set1 and set2";
		cout << "\n\t9 Find set1-set2\n\t10 Find set2 - set1";
		cout << "\n\t-1 to exit\n";

		if(Get_num(answer) == 0)
			break;
		switch (answer) {
		case 0:
			cout << "set1: " << a << endl << "set2: " << b;
			break;
		case 1:
			cout << "Enter new set1 size: ";
			Get_num(size);
			a.set(size);
			cout << "Current set1: " << a;
			if (cin.eof()) out = 1;
			break;

		case 2:
			cout << "Enter new set2 size: ";
			Get_num(size);
			b.set(size);
			cout << "Current set2: " << b;
			if (cin.eof()) out = 1;
			break;

		case 3:
			cout << "Enter new set1 element: ";
			Get_num(answer);
			if (!a.insert(answer))
				cout << "Element " << answer << " exists in set1";
			break;

		case 4:
			cout << "Enter new set2 element: ";
			Get_num(answer);
			if (!b.insert(answer))
				cout << "Element " << answer << " exists in set2";
			break;

		case 5:
			cout << "Enter element to find in set1: ";
			Get_num(answer);
			if (a.isexist(answer)) {
				cout << "Element " << answer << " exists in set1";
			}
			else cout << "Element " << answer << " don't exist in set1";
			break;

		case 6:
			cout << "Enter element to find in set2: ";
			Get_num(answer);
			if (b.isexist(answer)) {
				cout << "Element " << answer << " exists in set2";
			}
			else cout << "Element " << answer << " don't exist in set2";
			break;

		case 7:
			result = new Set(a | b);
			cout << "set1 | set2 = " << *result;
			delete result;
			break;

		case 8:
			result = new Set(a & b);
			cout << "set1 & set2 = " << *result;
			delete result;
			break;

		case 9:
			result = new Set(a - b);
			cout << "set1 - set2 = " << *result;
			delete result;
			break;

		case 10:
			result = new Set(b - a);
			cout << "set2 - set1 = " << *result;
			delete result;
			break;

		case -1:
			out = true;
			break;

		default:
			cout << "There is no such case";
		}
	}
	return 0;
}

template <typename T>
short int Get_num(T & a) {
	while (1) {
		cin >> a;
		if (!cin.good()) {
			if (cin.eof()) return 0; //^Z
			cout << "Please repeat: ";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		return 1;
	}
}