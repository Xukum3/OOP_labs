#define _USE_MATH_DEFINES
#include "Agnesi.h"
#include <math.h>

std::ostream& operator<< (std::ostream& out, const Point& point) {
	out << "(" << point.x << ", " << point.y << ")";
	return out;
}

Agnesi::Agnesi(double diam) {
	if (diam == 0) throw std::exception("Invalid parameter");
	A = diam;
}

void Agnesi::SetParam(double diam) {
	if (diam == 0) throw std::exception("Invalid parameter");
	A = diam;
}

std::pair<Point, Point> Agnesi::FindInfl()const {
	Point p1(-abs(A) / sqrt(3), 3 * A / 4);
	Point p2(abs(A) / sqrt(3), 3 * A / 4);
	return std::make_pair(p1, p2);
}

char* Agnesi::Eq()const {
	size_t size = sizeof(" / (x^2 + )");
	char num[20];
	sprintf_s(num, 20, "%.3f", pow(A, 3));
	size += strlen(num);
	sprintf_s(num, 20, "%.3f", A * A);
	size += strlen(num);
	char* Equation = new char[size];
	sprintf_s(Equation, size, "%.3f / (x^2 + %.3f)\0", pow(A, 3), A * A);
	return Equation;
}

double Agnesi::FindVolume()const { return pow(M_PI, 2) * abs(pow(A, 3)) / 2; }
double Agnesi::FindSquare()const { return M_PI * A * A; }
double Agnesi::FindY(double x)const { return pow(A, 3) / (A * A + x * x); }