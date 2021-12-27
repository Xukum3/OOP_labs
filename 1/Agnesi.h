#ifndef AGNESI_H
#define AGNESi_H

#include <iostream>

struct Point {
	double x;
	double y;

	Point(double X = 0, double Y = 0) : x(X), y(Y) { }
	friend std::ostream& operator<< (std::ostream& out, const Point& point);
};

class Agnesi {
	double A;
public:
	Agnesi(double diam);
	void SetParam(double diam);
	double GetParam()const { return A; }

	double FindY(double x)const;
	double FindSquare()const;
	double FindVolume()const;
	std::pair<Point, Point> FindInfl()const;
	char* Eq()const;
};

#endif //AGNESI_H