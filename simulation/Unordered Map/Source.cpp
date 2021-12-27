#include <iostream>
#include "Unordered_Map.h"

using namespace std;

Unordered_Map<int, int> test(Unordered_Map<int, int> map) {
	map.erase(-1);
	return map;
}


int main() {
	Unordered_Map<int, int> map;
	cout << map.contains(3);
	try {
		map.at(3);
	}
	catch (std::exception& ex) {
		cout << ex.what();
	}

	map[-1] = 10;

	for (int i = 0; i < 20; ++i) {
		map.insert(i, i);
	}

	Unordered_Map<int, int> map2 = map;

	/*for (int i = 0; i < 10; ++i) {
		map.erase(i);
	}*/
	//map.clear();
	//map.show();
	//cout << "before copy";
	//map.debug_show();
	//map = test(map);
	//map.debug_show();
	if (map2 == map) {
		std::cout << "\n\n1";
	}
}