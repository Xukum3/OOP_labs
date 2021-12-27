#include <iostream>
#include <cmath>
#include <new>
#include <initializer_list>
#include <algorithm>
#include "Vector.h"
#include "auxFuncs.h"

namespace Static {
	Vector::Vector(unsigned int size, double initNumber) {
		if (size > maxSize) {
			throw std::invalid_argument("Invalid size");
		}
		for (int i = 0; i < size; i++) {
			array[i] = initNumber;
		}
		curSize = size;
	}

	 Vector::Vector(unsigned int size, const double* inArray) {
		if (size > maxSize) {
			throw std::invalid_argument("Invalid size");
		}
		if (inArray == nullptr) {
			throw std::invalid_argument("Invalid array");
		}
		for (int i = 0; i < size; i++) {
			array[i] = inArray[i];
		}
		curSize = size;
	}

	Vector::Vector(const std::initializer_list<int>& list) {
		if (list.size() > maxSize) {
			throw std::invalid_argument("Invalid size");
		}
		int i = 0;
		for (double element : list) {
			array[i++] = element;
		}
		curSize = list.size();
	}

	bool Vector::getItem(unsigned int num, double& val) const {
		if (num > curSize - 1) {
			return false;
		}
		val = array[num];
		return true;
	}

	bool Vector::setItem(unsigned int num, double val) {
		if (num > curSize - 1) {
			return false;
		}
		array[num] = val;
		return true;
	}

	double& Vector::operator[](unsigned int i) {
		if (i >= curSize) throw std::out_of_range("Index out of range");
		return array[i];
	}

	const double& Vector::operator[](unsigned int i) const {
		if (i >= curSize) throw std::out_of_range("Index out of range");
		return array[i];
	}

	bool Vector::getArray(std::ostream& outStream) const {
		outStream << curSize << " ";
		for (int i = 0; i < curSize; i++) {
			outStream << array[i] << " ";
		}
		outStream << std::endl;
		return true;
	}

	bool Vector::setArray(std::istream& inStream) {
		unsigned int tmp = 0;
		if (!getNum(tmp, inStream)) return false;
		if (tmp > maxSize) return false;
		if (tmp == 0) {
			curSize = tmp;
			return true;
		}
		double* tmpArray = new(std::nothrow) double[tmp];
		if (tmpArray == nullptr) return false;
		for (int i = 0; i < tmp; i++) {
			if (!getNum(tmpArray[i], inStream)) {
				delete[] tmpArray;
				return false;
			}
		}
		curSize = tmp;
		for (int i = 0; i < curSize; i++) {
			array[i] = tmpArray[i];
		}
		return true;
	}

	void Vector::add(const Vector& vector, Vector& new_vector) const {
		new_vector.curSize = std::min(curSize, vector.curSize);
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = array[i] + vector.array[i];
		}
	}

	void Vector::sub(const Vector& vector, Vector& new_vector) const {
		new_vector.curSize = std::min(curSize, vector.curSize);
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = array[i] - vector.array[i];
		}
	}

	double Vector::mult(const Vector& vector) const {
		int num = std::min(curSize, vector.curSize);
		double val = 0;
		for (int k = 0; k < num; k++) {
			val += array[k] * vector.array[k];
		}
		return val;
	}

	Vector operator+(const Vector& v1, const Vector& v2) {
		Vector new_vector;
		new_vector.curSize = std::min(v1.curSize, v2.curSize);
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = v1.array[i] + v2.array[i];
		}
		return new_vector;
	}

	Vector operator-(const Vector& v1, const Vector& v2) {
		Vector new_vector;
		new_vector.curSize = std::min(v1.curSize, v2.curSize);
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = v1.array[i] - v2.array[i];
		}
		return new_vector;
	}

	double operator*(const Vector& v1, const Vector& v2) {
		int num = std::min(v1.curSize, v2.curSize);
		double val = 0;
		for (int k = 0; k < num; k++) {
			val += v1.array[k] * v2.array[k];
		}
		return val;
	}

	bool Vector::normOfVector(double& norma) const {
		norma = 0;
		if (curSize == 0) {
			return false;
		}
		for (int i = 0; i < curSize; i++) {
			if (std::abs(array[i]) > norma) {
				norma = std::abs(array[i]);
			}
		}
		return true;
	}

	bool Vector::append(double val) {
		if (curSize == maxSize) {
			return false;
		}
		array[curSize] = val;
		curSize++;
		return true;
	}
}

namespace Dynamic {
	Vector::Vector(double initNumber) {
		array = new double[1];
		array[0] = initNumber;
		curSize = 1;
	}

	Vector::Vector(unsigned int size, double initNumber) {
		if (size == 0) {
			curSize = 0;
			return;
		}
		array = new double[size];
		curSize = size;
		for (int i = 0; i < curSize; i++) array[i] = initNumber;
		//std::copy(begin(), end(), initNumber);
	}
	Vector::Vector(unsigned int size,  const double* inArray) {
		if (size == 0) {
			curSize = 0;
			return;
		}
		array = new double[size];
		curSize = size;
		for (int i = 0; i < curSize; i++) array[i] = inArray[i];
	}

	Vector::Vector(const std::initializer_list<double>& list) {
		if (list.size() == 0) {
			curSize = 0;
			return;
		}
		array = new double[list.size()];
		int i = 0;
		for (double element : list) {
			array[i] = element;
			i++;
		}
		curSize = list.size();
	}

	Vector::Vector(const Vector& vector) {
		if (vector.curSize == 0) {
			curSize = 0;
			return;
		}
		array = new double[vector.curSize];
		curSize = vector.curSize;
		memcpy(array, vector.array, curSize * sizeof(double));
	}

	Vector::Vector(Vector&& vector) noexcept {
		curSize = vector.curSize;
		array = vector.array;
		vector.array = nullptr;
	}

	Vector::~Vector() {
		if (curSize > 0) {
			delete[] array;
		}
	}

	bool Vector::getItem(unsigned int num, double& val) const {
		if (num > curSize - 1) {
			return false;
		}
		val = array[num];
		return true;
	}

	bool Vector::setItem(unsigned int num, double val) {
		if (num > curSize - 1) {
			return false;
		}
		array[num] = val;
		return true;
	}

	bool Vector::getArray(std::ostream& outStream) const {
		outStream << curSize << " ";
		for (int i = 0; i < curSize; i++) {
			outStream << array[i] << " ";
		}
		outStream << std::endl;
		return true;
	}

	bool Vector::setArray(std::istream& inStream) {
		int tmp = 0;
		if (!getNum(tmp, inStream)) return false;
		if (tmp == 0) {
			curSize = tmp;
			return true;
		}
		double* tmpArray = new(std::nothrow) double[tmp];
		if (tmpArray == nullptr) return false;
		for (int i = 0; i < tmp; i++) {
			if (!getNum(tmpArray[i], inStream)) {
				delete[] tmpArray;
				return false;
			}
		}
		delete[] array;
		curSize = tmp;
		array = tmpArray;
		return true;
	}

	double& Vector::operator[](unsigned int i) {
		if (i >= curSize) throw std::out_of_range("Index out of range");
		return array[i];
	}

	const double& Vector::operator[](unsigned int i) const {
		if (i >= curSize) throw std::out_of_range("Index out of range");
		return array[i];
	}

	Vector operator+(const Vector& v1, const Vector& v2) {
		Vector new_vector;
		new_vector.curSize = std::min(v1.curSize, v2.curSize);
		if (new_vector.curSize == 0) return new_vector;
		new_vector.array = new double[new_vector.curSize];
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = v1.array[i] + v2.array[i];
		}
		return new_vector;
	}

	Vector operator-(const Vector& v1, const Vector& v2) {
		Vector new_vector;
		new_vector.curSize = std::min(v1.curSize, v2.curSize);
		if (new_vector.curSize == 0) return new_vector;
		new_vector.array = new double[new_vector.curSize];
		for (int i = 0; i < new_vector.curSize; i++) {
			new_vector.array[i] = v1.array[i] - v2.array[i];
		}
		return new_vector;
	}

	double operator*(const Vector& v1, const Vector& v2) {
		int num = std::min(v1.curSize, v2.curSize);
		double val = 0;
		for (int k = 0; k < num; k++) {
			val += v1.array[k] * v2.array[k];
		}
		return val;
	}

	Vector& Vector::operator=(const Vector& vector) {
		if (&vector == this) return *this;
		double* tmpArray = new double[vector.curSize]; //
		if (curSize > 0) delete[] array;
		if (vector.curSize == 0) {
			curSize = vector.curSize;
			return *this;
		}
		array = tmpArray; //
		memcpy(array, vector.array, curSize * sizeof(double));
		return *this;
	}

	Vector& Vector::operator=(Vector&& vector) noexcept {
		if (&vector == this) return *this;
		if (curSize > 0) delete[] array;
		if (vector.curSize == 0) {
			curSize = vector.curSize;
			return *this;
		}
		curSize = vector.curSize;
		array = vector.array;
		vector.curSize = 0;
		vector.array = nullptr;
		return *this;
	}

	bool operator==(const Vector& v1, const Vector& v2) {
		if (v1.curSize != v2.curSize) {
			return false;
		}
		for (int i = 0; i < v1.curSize; i++) {
			if (v1.array[i] != v2.array[i]) return false;
		}
		return true;
	}

	bool operator!=(const Vector& v1, const Vector& v2) {
		if (v1.curSize != v2.curSize) {
			return true;
		}
		for (int i = 0; i < v1.curSize; i++) {
			if (v1.array[i] != v2.array[i]) return true;
		}
		return false;
	}

	std::istream& operator>>(std::istream& inStream, Vector& vector) {
		unsigned int tmp = 0;
		if (!getNum(tmp, inStream)) {
			inStream.setstate(std::ios::failbit);
			return inStream;
		}
		if (tmp == 0) {
			vector.curSize = tmp;
			return inStream;
		}
		double* tmpArray = new(std::nothrow) double[tmp];
		if (tmpArray == nullptr) { 
			inStream.setstate(std::ios::failbit);
			return inStream;
		}
		for (int i = 0; i < tmp; i++) {
			if (!getNum(tmpArray[i], inStream)) {
				delete[] tmpArray;
				return inStream;
			}
		}
		if(vector.curSize > 0) delete[] vector.array;
		vector.curSize = tmp;
		vector.array = tmpArray;
		return inStream;
	}

	std::ostream& operator<<(std::ostream& outStream, const Vector& vector) {
		outStream << vector.curSize << " ";
		for (int i = 0; i < vector.curSize; i++) {
			outStream << vector.array[i] << " ";
		}
		outStream << std::endl;
		return outStream;
	}

	bool Vector::normOfVector(double& norma) const {
		norma = 0;
		if (curSize == 0) {
			return false;
		}
		for (int i = 0; i < curSize; i++) {
			if (std::abs(array[i]) > norma) {
				norma = std::abs(array[i]);
			}
		}
		return true;
	}

	bool Vector::append(double val) {
		double* tmpArray = new(std::nothrow) double[curSize + 1];
		if (tmpArray == nullptr) return false;
		memcpy(tmpArray, array, curSize * sizeof(double));
		if (curSize > 0) delete[] array;
		array = tmpArray;
		array[curSize] = val;
		curSize++;
	}
}
