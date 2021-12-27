#pragma once
#include <vector>
#include <allegro5/allegro.h>
#include <iostream>

class Defines {
private:
	static unsigned int _n_acid, _n_salt, _n_meal, _n_col;
	static int _log_col;
	static int _offset;
public:
	static ALLEGRO_BITMAP* blood;
	static ALLEGRO_BITMAP* col_nums[9];
	static unsigned int& n_acid() { return _n_acid; }
	static unsigned int& n_salt() { return _n_salt; }
	static unsigned int& n_meal() { return _n_meal; }
	static unsigned int& n_col() { return _n_col; }
	static int& log_col() { return _log_col; }
	static int& offset() { return _offset; }

	~Defines() {
		for (int i = 0; i < 9; ++i) {
			al_destroy_bitmap(col_nums[i]);
		}
		al_destroy_bitmap(blood);
	}
};


enum types {
	soldier = 10,
	defender = 11,
	invader = 12,
	hunter = 13,
	spawner = 50,
	acid = 51,
	meal = 52,
	salt = 53
};

static int _Get_Type(int ID){
	return (ID % (100 * Defines::offset())) / Defines::offset();
}

template <typename T>
class my_ptr {
public:
	T* b_ptr;

	my_ptr(T* d_ptr = nullptr) {
		b_ptr = d_ptr;
	}

	~my_ptr(){
		if(b_ptr) delete b_ptr;
	}

	my_ptr(my_ptr&& other): b_ptr(other.b_ptr) {
		other.b_ptr = nullptr;
	}

	my_ptr(const my_ptr& other) = delete;

	T& operator* (){
		return *b_ptr;
	}
	T* operator-> () {
		return b_ptr;
	}
};

struct Point {
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) { }
	Point operator+ (const Point& other) {
		return Point(x + other.x, y + other.y);
	}
	Point& operator= (const Point& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	bool operator==(Point other) {
		return (x == other.x && y == other.y);
	}
	
	bool operator!=(Point other) {
		return !(*this == other);
	}
};

struct back_el {
	char name;
	ALLEGRO_BITMAP* bitmap;
};

struct required_resources {
	int salt;
	int acid;
	int food;
	size_t time_to_create;

	std::istream& get_stats(std::istream& in = std::cin) {
		int n = -1;
		while (n < 0) {
			std::cout << "\tSalt(>= 0): ";
			in >> n;
		}
		salt = n;

		n = -1;
		while (n < 0) {
			std::cout << "\n\tAcid(>= 0): ";
			in >> n;
		}
		acid = n;

		n = -1;
		while (n < 0) {
			std::cout << "\n\tFood(>= 0): ";
			in >> n;
		}
		food = n;

		n = -1;
		while (n < 5) {
			std::cout << "\n\tTime(>= 5): ";
			in >> n;
		}
		time_to_create = n;

		return in;
	}
};

std::ostream& operator<<(std::ostream& out, const required_resources& r);
std::ostream& operator<<(std::ostream& out, const Point& p);


template<typename T>
bool own_cin(T& a, std::istream & in = std::cin) {
	while (1) {
		in >> a;
		if (!in.good()) {
			if (in.eof()) return false; //^Z
			in.clear();
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		return true;
	}
}