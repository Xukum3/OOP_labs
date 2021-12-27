#pragma once

#include <iostream>

template <typename T>
bool getNum(T& a, std::istream& inStream) {
	inStream >> a;
	if (!inStream.good())
		return false;
	return true;
}