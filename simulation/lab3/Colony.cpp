#include "Colony.h"
#include "application.h"
#include "Units.h"

int Colony::generate_id(types type) { 
	return (++next_unit_id * 100 + type) * Defines::offset() + number;
}

void Colony::add_unit(int id, Unit* u) {
	units.insert(std::make_pair(id, u));
}

void Colony::delete_unit(int id) {
	delete units.at(id);
	units.erase(id);
}

void Colony::update_units(Level& lvl) {
	for (auto it = units.begin(); it != units.end(); ++it) {
		Point p = (*it).second->make_move(lvl);
		if (p.x == 0 && p.y == 0) {
			int xxxx = 1;
		}
		(*it).second->set_next_position(p.x, p.y);
	}
	if (is_alive()) {
		s->count_tick(lvl);
	}
}

void Colony::set_unit_state(unsigned int id, Unit::states state) {
	Unit* u = units.at(id);
	u->set_state(state);
}

void Colony::set_all_units_state(Unit::states state) {
	for (auto it = units.begin(); it != units.end(); ++it) {
		it->second->set_state(state);
	}
}

void Colony::show_info() {
	std::cout << "Number: " << number << std::endl;
	std::cout << "\tAlive:" << alive << std::endl;
	if (alive) {
		std::cout << "\tSalt Income: " << salt_income << std::endl;
		std::cout << "\tAcid Income: " << acid_income << std::endl;
		std::cout << "\tAcid Level: " << _acid_level << std::endl;
		std::cout << "\tSalt Level: " << _salt_level << std::endl;
		std::cout << "\tFood Level: " << _food_level << std::endl;
		std::cout << "\tNumber of units: " << units.size() << std::endl;
		std::cout << "\tSpawner coordinates: " << s->get_cur_pos() << std::endl;
	}
	std::cout << std::endl;
}

void Colony::show_all_units() {
	if (units.size() == 0) {
		std::cout << "No units in this colony\n";
	}
	for (auto it = units.begin(); it != units.end(); ++it) {
		it->second->show_info();
	}
}



Unit* Colony::get_unit(int id) {
	Unit* u = units.at(id);
	return u;
}

void Colony::update_resources() {
	if (_salt_level < 100000)
		_salt_level += salt_income;
	if (_acid_level < 100000)
		_acid_level += acid_income;
}

void Colony::draw(Level& lvl, std::vector<Point>& points_to_redraw) {
	auto& front = lvl.get_front();
	s->update();
	int to_delete = -1;
	Point pos_to_delete;

	for (auto it = units.begin(); it != units.end(); ++it) {
		if (units.contains(to_delete)) {
			units.erase(to_delete);
			front[pos_to_delete.x][pos_to_delete.y] = -1;
			lvl.draw_blood(pos_to_delete);
			points_to_redraw.push_back(pos_to_delete);
		}

		if ((*it).second->get_hp() <= 0) {
			to_delete = it->first;
			int type = _Get_Type(it->first);
			_food_level += s->get_required_food(type);
			pos_to_delete = (*it).second->get_cur_pos();
			continue;
		}
		else{
			if (under_attack)
				it->second->colony_temp_state(Unit::protect_spawner);
			else
				it->second->set_prev_state();
			it->second->update();
			//it->second->is_new_position = false;
		}
	}
	if (units.contains(to_delete)) {
		units.erase(to_delete);
		front[pos_to_delete.x][pos_to_delete.y] = -1;
		lvl.draw_blood(pos_to_delete);
		points_to_redraw.push_back(pos_to_delete);
	}
	under_attack = false;
}

void Colony::delete_all_units(Field& field) {
	alive = false;
	auto& front = field.get_front();
	for (auto it = units.begin(); it != units.end(); ++it) {
		it->second->update_position();
		Point p = it->second->get_cur_pos();
		front[p.x][p.y] = -1;
		field.draw_back_at_position(p.x, p.y);
		delete it->second;
	}
	Point sp_pos = s->get_cur_pos();
	if (front[sp_pos.x][sp_pos.y] == spawner * Defines::offset() + number) {
		front[sp_pos.x][sp_pos.y] = -1;
		field.draw_back_at_position(sp_pos.x, sp_pos.y);
	}

	units.clear();
}
