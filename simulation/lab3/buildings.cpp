#include "buildings.h"
#include "application.h"

void Resource_Point::update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) {
	auto& front = field.get_front();
	Point pos = get_cur_pos();
	front[pos.x][pos.y] -= colony; //delete old colony owner
	front[pos.x][pos.y] += new_owner; //set new colony owner

	if (colony > 0 && colonies[colony - 1].is_alive()) {
		for (auto it = defenders.begin(); it != defenders.end(); ++it) {
			colonies[colony - 1].delete_unit(it->second->get_id());
		}
	}

	defenders.clear();
}


void Salt_Cave::update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) {
	Resource_Point::update_owner(new_owner, field, colonies);

	if (colony > 0)
		colonies[colony - 1].update_salt(-resource_amount);

	colonies[new_owner- 1].update_salt(resource_amount);

	colony = new_owner;
	hp = 25;
}


void Acid_Lake::update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) {
	Resource_Point::update_owner(new_owner, field, colonies);

	if (colony > 0)
		colonies[colony - 1].update_acid(-resource_amount);
	colonies[new_owner- 1].update_acid(resource_amount);

	colony = new_owner;
	hp = 25;
}

void Food_Point::update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) {
	Resource_Point::update_owner(new_owner, field, colonies);

	if (colony > 0) {
		if (resource_amount <= colonies[colony - 1].food_level())
			colonies[colony - 1].update_food(-resource_amount);
		else
			colonies[colony - 1].food_level() = 0;
	}

	colonies[new_owner- 1].update_food(resource_amount);

	colony = new_owner;
	hp = 25;
}


void Resource_Point::add_defender(Unit* u) {
	int col_own = u->get_id() % Defines::offset();
	if (col_own != colony) throw std::logic_error("Colonies aren't same");
	defenders.insert(u->get_id(), u);
	hp += u->get_hp();
}


void Resource_Point::delete_defender(Unit* u) {
	defenders.erase(u->get_id());
	hp -= u->get_hp();
	if (hp < 0) hp = 0;
}

void Resource_Point::update_resource_level(int new_level, std::vector<Colony>& colonies) {
	resource_amount = new_level;
}

void Salt_Cave::update_resource_level(int new_level, std::vector<Colony>& colonies) {
	if (new_level <= 0) return;
	if(colony > 0)
		colonies[colony - 1].update_salt(new_level - resource_amount);
	Resource_Point::update_resource_level(new_level, colonies);
}

void Acid_Lake::update_resource_level(int new_level, std::vector<Colony>& colonies) {
	if (new_level <= 0) return;
	if (colony > 0)
		colonies[colony - 1].update_acid(new_level - resource_amount);
	Resource_Point::update_resource_level(new_level, colonies);
}

void Food_Point::update_resource_level(int new_level, std::vector<Colony>& colonies) {
	if (new_level <= 0) return;
	if (colony > 0)
		colonies[colony - 1].update_food(new_level - resource_amount);
	Resource_Point::update_resource_level(new_level, colonies);
}

Point Spawner::find_point_for_unit(Level& lvl) {
	auto& back = lvl.get_back();
	auto& front = lvl.get_front();

	Point sp_position = get_cur_pos();

	for (int i = 0; i < 30; ++i) {
		int r_x = rand() % 5 - 2;
		int r_y = rand() % 5 - 2;
		Point poss(sp_position.x + r_x, sp_position.y + r_y);
		if (0 <= poss.x && poss.x < lvl.get_width() && 0 <= poss.y && poss.y < lvl.get_height()) {
			if (back[poss.x][poss.y].name == 0 && front[poss.x][poss.y] == -1) {
				return poss;
			}
		}
	}

	for (int c_y = std::max(sp_position.y - 3, 0); c_y <= std::min(sp_position.y + 3, int(lvl.get_height()) - 1); ++c_y) {
		for (int c_x = std::max(sp_position.x - 3, 0); c_x <= std::min(sp_position.x + 3, int(lvl.get_width()) - 1); ++c_x) {
			if (back[c_x][c_y].name == 0 && front[c_x][c_y] == -1) {
				return Point(c_x, c_y);
			}
		}
	}

	return Point(-1, -1);
}



void Spawner::make_new(int type, Colony& owner) {
	required_resources res = resources.at(type);
	if (owner.salt_level() >= res.salt &&
		owner.acid_level() >= res.acid &&
		owner.food_level() >= res.food) 
	{
		creating_unit c;
		c.id = type;
		c.time_remain = res.time_to_create;
		creating.push_back(c);
		owner.acid_level() -= res.acid;
		owner.salt_level() -= res.salt;
		owner.food_level() -= res.food;
	}
}

void Spawner::count_tick(Level& lvl) {
	bool is_delete = false;
	std::list<creating_unit>::iterator to_delete;
	for (auto it = creating.begin(); it != creating.end(); ++it) {
		if (is_delete) {
			creating.erase(to_delete);
			is_delete = false;
		}

		it->time_remain -= 1;
		if (it->time_remain <= 0) {
			ready.push_back(it->id);
			to_delete = it;
			is_delete = true;
		}
	}

	if (is_delete) {
		creating.erase(to_delete);
		is_delete = false;
	}

	add_new_unit(lvl);

	int type = 10 + rand() % 3;
	make_new(type, lvl.colonies[colony - 1]);
}

void Spawner::set_new_req_resources() {
	std::cout << "Current values:\n";
	std::cout << "Soldier:\n" << resources.at(soldier);
	std::cout << "Defender:\n" << resources.at(defender);
	std::cout << "Invader:\n" << resources.at(invader) << std::endl;

	std::cout << "New values:\n";
	std::cout << "Soldier:\n"; resources.at(soldier).get_stats();
	std::cout << "Defender:\n"; resources.at(defender).get_stats();
	std::cout << "Invader:\n"; resources.at(invader).get_stats();
}

bool Spawner::add_new_unit(Level& lvl) {
	if (ready.size() == 0) return false;

	auto& back = lvl.get_back();
	auto& front = lvl.get_front();

	int index = rand() % ready.size();
	if (ready.size() == 1) index = 0;
	int type = ready[index];
	ready.erase(ready.begin() + index);

	Point position = find_point_for_unit(lvl);
	if (position == Point(-1, -1)) {
		return false;
	}

	int id;
	Unit* u = nullptr;

	switch (type)
	{
	case soldier:
		id = lvl.colonies[colony - 1].generate_id(soldier);
		u = new Soldier("soldier.png", position.x, position.y, colony, id);
		break;
	
	case defender:
		id = lvl.colonies[colony - 1].generate_id(defender);
		u = new Defender("defender.png", position.x, position.y, colony, id);
		break;
	
	case invader:
		id = lvl.colonies[colony - 1].generate_id(invader);
		u = new Invader("invader.png", position.x, position.y, colony, id);
		break;
	}

	front[position.x][position.y] = id;
	u->set_next_position(u->get_cur_pos());
	lvl.colonies[colony - 1].add_unit(id, u);

	return true;
}

std::unordered_map<int, required_resources> Spawner::resources = {
		{soldier, {30, 30, 60, 10}},
		{defender, {40, 40, 80, 15}},
		{invader, {50, 50, 100, 20}}
};