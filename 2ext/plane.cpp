#include <iostream>
#include "../used_lib/Library/Vector.h"

using namespace std;
using namespace Dynamic;


struct Point {
	double x;
	double y;
	double z;

	Point() : x(0), y(0), z(0) { }
	Point(double X, double Y, double Z) : x(X), y(Y), z(Z) { }
};


ostream& operator<< (ostream& out, Point& p) {
	out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return out;
}

istream& operator>> (istream& in, Point& p) {
	in >> p.x;
	in >> p.y;
	in >> p.z;
	return in;
}

Vector crossprod(const Vector& v1, const Vector& v2) {
	return Vector{
			v1[1] * v2[2] - v1[2] * v2[1],
			-v1[0] * v2[2] + v1[2] * v2[0],
			v1[0] * v2[1] - v1[1] * v2[0]
	};
}

class Plane{
	Point p1, p2, p3;

	Vector norm() const {
		double vec1[3], vec2[3];
		vec1[0] = p2.x - p1.x;
		vec1[1] = p2.y - p1.y;
		vec1[2] = p2.z - p1.z;		
		
		vec2[0] = p3.x - p1.x;
		vec2[1] = p3.y - p1.y;
		vec2[2] = p3.z - p1.z;

		Vector v1(3, vec1);
		Vector v2(3, vec2);

		return crossprod(v1, v2);
	}
public:
	Plane(const Point& P1, const Point& P2, const Point& P3) : p1(P1), p2(P2), p3(P3) {}

	short int intersect(const Plane& other) const {
		Vector n1 = norm();
		Vector n2 = other.norm();

		Vector n1xn2 = crossprod(n1, n2);

		double a;
		n1xn2.normOfVector(a);
		
		if (a != 0) {
			return 1;
		}

		else {
			a = n1 * Vector{ other.p1.x - p1.x, other.p1.y - p1.y , other.p1.z - p1.z };
			if (a == 0) return -1;
			else return 0;
		}
	}
};

int main() {
	bool out = false;
	Point a1, a2, a3, b1, b2, b3;
	cout << "Enter 3 points of the first plane\n";
	while (1) {
		cin >> a1 >> a2 >> a3;
		if (!cin.good()) {
			out = true;
			break;
		}
		Vector cross = crossprod(Vector{ a2.x - a1.x, a2.y - a1.y, a2.z - a1.z }, Vector{ a3.x - a1.x, a3.y - a1.y, a3.z - a1.z });
		double norm;
		cross.normOfVector(norm);
		if (norm == 0) {
			cout << "Points are on the one line\nRepeat\n";
		}
		else break;
	}

	if (!out) {
		cout << "Enter 3 points of the second plane\n";
		while (1) {
			cin >> b1 >> b2 >> b3;
			if (!cin.good()) {
				out = true;
				break;
			}
			Vector cross = crossprod(Vector{ b2.x - b1.x, b2.y - b1.y, b2.z - b1.z }, Vector{ b3.x - b1.x, b3.y - b1.y, b3.z - b1.z });
			double norm;
			cross.normOfVector(norm);
			if (norm == 0) {
				cout << "Points are on the one line\nRepeat\n";
			}
			else break;
		}
	}

	if (!out) {
		Plane pl1(a1, a2, a3), pl2(b1, b2, b3);

		short int inter = pl1.intersect(pl2);

		if (inter == 0) {
			cout << "Planes are parallel'ny\n";
		}
		else if (inter == 1) {
			cout << "Planes intersect\n";
		}
		else {
			cout << "Planes are equal\n";
		}
	}


}
