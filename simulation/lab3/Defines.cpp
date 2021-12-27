#include "defines.h"

unsigned int Defines::_n_acid = 0;
unsigned int Defines::_n_salt = 0;
unsigned int Defines::_n_meal = 0;
unsigned int Defines::_n_col = 0;
int Defines::_log_col = 0;
int Defines::_offset = 0;
ALLEGRO_BITMAP* Defines::blood = nullptr;

ALLEGRO_BITMAP* Defines::col_nums[9];


std::ostream& operator<<(std::ostream& out, const required_resources& r) {
	out << "\tsalt: " << r.salt << std::endl;
	out << "\tacid: " << r.acid << std::endl;
	out << "\tfood: " << r.food << std::endl;
	out << "\ttime: " << r.time_to_create << std::endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
	out << "(" << p.x << "; " << p.y << ")";
	return out;
}