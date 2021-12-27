#pragma once

#include <initializer_list>

#ifndef VECTOR_H
#define VECTOR_H

namespace Static {
	class Vector
	{
	private:
		static constexpr unsigned int maxSize = 20;
		unsigned int curSize;
		double array[maxSize];
	public:
		Vector() : curSize(0) {};
		Vector(double initNumber) : curSize(1) { array[0] = initNumber; };
		Vector(unsigned int size, double initNumber); //Vector(1, 0);
		Vector(unsigned int size, const double* inArray);
		Vector(const std::initializer_list<int>& list);

		bool getItem(unsigned int num, double& val) const;
		bool setItem(unsigned int num, double val);
		unsigned int getMaxSize() const { return maxSize; }
		unsigned int getCurSize() const { return curSize; }
		double& operator[](unsigned int i);
		const double& operator[](unsigned  int i) const;
		bool getArray(std::ostream& outStream = std::cout) const ;
		bool setArray(std::istream& inStream = std::cin);

		void add(const Vector& vector, Vector& new_vector) const ;
		void sub(const Vector& vector, Vector& new_vector) const ;
		double mult(const Vector& vector) const ;

		friend Vector operator+(const Vector& v1, const Vector& v2);
		friend Vector operator-(const Vector& v1, const Vector& v2);
		friend double operator*(const Vector& v1, const Vector& v2);

		bool normOfVector(double& norma) const ;
		bool append(double val);
	};
}

namespace Dynamic {
	class Vector
	{
	private:
		unsigned int curSize;
		double* array;
	public:
		Vector() : curSize(0) {};
		Vector (double initNumber);
		Vector(unsigned int size, double initNumber);
		Vector(unsigned int size, const double* inArray);
		Vector(const std::initializer_list<double>& list);
		Vector(const Vector& vector);
		Vector(Vector&& vector) noexcept ;
		~Vector();

		bool getItem(unsigned int num, double& val) const ;
		bool setItem(unsigned int num, double val);
		unsigned int getCurSize() const { return curSize; }
		bool getArray(std::ostream& outStream = std::cout) const ;
		bool setArray(std::istream& inStream = std::cin);

		double& operator[](unsigned int i);
		const double& operator[](unsigned int i) const;
		friend Vector operator+(const Vector& v1, const Vector& v2);
		friend Vector operator-(const Vector& v1, const Vector& v2);
		friend double operator*(const Vector& v1, const Vector& v2);
		Vector& operator=(const Vector& vector); 
		Vector& operator=(Vector&& vector) noexcept;
		friend bool operator==(const Vector& v1, const Vector& v2);
		friend bool operator!=(const Vector& v1, const Vector& v2);
		friend std::istream& operator>>(std::istream& inStream, Vector& vector);
		friend std::ostream& operator<<(std::ostream& outStream, const Vector& vector);

		bool normOfVector(double& norma) const;
		bool append(double val);

		double* begin() const { return &array[0]; }
		double* end() const { return &array[curSize]; }
	};

}

#endif // VECTOR_H
