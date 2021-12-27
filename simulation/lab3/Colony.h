#pragma once
#include <stdexcept>
#include <unordered_map>
#include "buildings.h"
#include "classes.h"
#include "Units.h"
#include "application.h"
#include "../Unordered Map/Unordered_Map.h"

class Level;

class Colony {
private:
	bool under_attack = false;
	int next_unit_id = 0;
	bool alive = true;
	size_t number;
	unsigned int salt_income;
	unsigned int acid_income;
	unsigned int _food_level;
	unsigned int _salt_level;
	unsigned int _acid_level;
	unsigned int cons_level; 
	Unordered_Map<unsigned int, Unit*> units;
	Spawner* s;

public:
	Colony(size_t number, Spawner* sp, unsigned int s_i = 0, unsigned int a_i = 0, unsigned int f_l = 300, unsigned int s_l = 300, unsigned int a_l = 300, unsigned int c_l = 0) :
		number(number), salt_income(s_i), acid_income(a_i), _food_level(f_l), _salt_level(s_l), _acid_level(a_l), cons_level(c_l)
	{
		if (sp == nullptr) throw std::logic_error("No spawner!");
		s = sp;
	}

	Colony(Colony&& other) noexcept : s(other.s), number(other.number),
	salt_income(other.salt_income), acid_income(other.acid_income),
	_salt_level(other._salt_level), _acid_level(other._acid_level), _food_level(other._food_level),
	cons_level(other.cons_level), units(other.units)
	{
		other.s = nullptr;
		for (auto it = units.begin(); it != units.end(); ++it) {
			it->second = nullptr;
		}
	}

	~Colony() {
		delete s;
		for (auto it = units.begin(); it != units.end(); ++it) {
			delete it->second;
		}
	}

	//-----------------------------------------------------------------------------------
	int generate_id(types type);
	void add_unit(int id, Unit* u);
	void delete_unit(int id);
	void delete_all_units(Field& field);

	bool contains_unit(int id) { return units.contains(id); }
	Unit* get_unit(int id);
	void update_units(Level& lvl);
	void set_unit_state(unsigned int id, Unit::states state); 
	void set_all_units_state(Unit::states state); 

	void show_info();
	void show_all_units();

	//-----------------------------------------------------------------------------------
	void get_damage_spawner(int damage) { s->get_damage(damage); under_attack = true;  }
	Point get_spawner_pos(){ return s->get_cur_pos(); }

	void update_salt(int value) { salt_income += value; }
	void update_acid(int value) { acid_income += value; }
	void update_food(int value) { 
		if (value < 0 && abs(value) >= _food_level) _food_level = 0;
		else _food_level += value; 
	}

	void update_resources();

	//-------------------------------------------------------------------------------------
	int hp() { return s->get_hp(); }
	bool is_alive() { return alive; }
	unsigned int& salt_level() { return _salt_level; }
	unsigned int& acid_level() { return _acid_level; }
	unsigned int& food_level() { return _food_level; }

	void draw(Level& lvl, std::vector<Point>& points_to_redraw);
};