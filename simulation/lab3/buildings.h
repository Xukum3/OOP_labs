#pragma once
#include <vector>
#include <iostream>
#include "classes.h"
#include "Units.h"
#include "defines.h"
#include <unordered_set>
#include "../Unordered Map/Unordered_Map.h"
class Colony;
class Field;

class Spawner : public Visual_Object {
private:
	struct creating_unit {
		int id;
		int time_remain;
	};

	int hp;
	int colony;
	std::vector<int> ready;
	std::list<creating_unit> creating;

	static std::unordered_map<int, required_resources> resources;

private:
	Point find_point_for_unit(Level& lvl);
	bool add_new_unit(Level& lvl);

public:
	Spawner(std::string sprite_name, int x, int y, int colony, int hp = 10000) : Visual_Object(sprite_name, x, y), colony(colony), hp(hp) { }
	void make_new(int type, Colony& owner);

	void get_damage(int damage) { hp -= damage; }
	int get_hp() const { return hp; }

	void count_tick(Level& lvl);
	static void set_new_req_resources();
	int get_required_food(int type) { return resources.at(type).food; }

	void update() override {
		Visual_Object::update();
		Point pos = get_cur_pos();
		al_draw_bitmap(Defines::col_nums[colony - 1], pos.x * 10, pos.y * 10, 0);
	}

};


class Resource_Point : public Visual_Object {
protected:
	int hp = 0;
	int colony = 0;
	int resource_amount;
	Unordered_Map<int, Unit*> defenders;
public:
	Resource_Point(std::string sprite_name, int x, int y, int resouces) : resource_amount(resouces), Visual_Object(sprite_name, x, y) {}

	virtual void update_owner(int new_owner, Field& field, std::vector<Colony>& colonies);
	void add_defender(Unit* u);
	void delete_defender(Unit* u);

	void get_damage(int damage) { hp -= damage; }
	int get_resource() const { return resource_amount; }
	int get_hp() const { return hp; }
	int get_colony_owner() { return colony; }
	int get_num_of_defenders() { return defenders.size(); }
	virtual void update_resource_level(int new_level, std::vector<Colony>& colonies);

	virtual std::string get_type() = 0;

	void update() override {
		Visual_Object::update();
		if (colony > 0) {
			Point pos = get_cur_pos();
			al_draw_bitmap(Defines::col_nums[colony - 1], pos.x * 10, pos.y * 10, 0);
		}
	}
};

class Salt_Cave : public Resource_Point {
public:
	Salt_Cave(int x, int y, std::string sprite_name, int resource_amount) : Resource_Point(sprite_name, x, y, resource_amount) {}
	void update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) override;
	std::string get_type() override { return "Salt Cave"; }
	void update_resource_level(int new_level, std::vector<Colony>& colonies) override;
	~Salt_Cave(){}
};

class Acid_Lake : public Resource_Point {
public:
	Acid_Lake(int x, int y, std::string sprite_name, int resource_amount) : Resource_Point(sprite_name, x, y, resource_amount) {}
	void update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) override;
	std::string get_type() override { return "Acid Lake"; }
	void update_resource_level(int new_level, std::vector<Colony>& colonies) override;
	~Acid_Lake() {}
};

class Food_Point : public Resource_Point {
public:
	Food_Point(int x, int y, std::string sprite_name, int resource_amount) : Resource_Point(sprite_name, x, y, resource_amount) {}
	void update_owner(int new_owner, Field& field, std::vector<Colony>& colonies) override;
	std::string get_type() override { return "Food Point"; }
	void update_resource_level(int new_level, std::vector<Colony>& colonies) override;
	~Food_Point() {}
};



