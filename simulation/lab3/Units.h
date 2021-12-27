#pragma once
//#include "defines.h"
#include "classes.h"

class Resource_Point;
class Field;
class Colony;
class Level;


class Unit : public Visual_Object {
public:
	enum states {
		base = 0,
		protect_point = 1,
		protect_spawner = 2,
		user = 3
	};

protected:
	int hp;
	int maxhp;
	int colony;
	int damage;
	states prev_state;
	states state;
	double agility;
	unsigned int id;
	bool is_dead = false;
	bool is_new_position = false;
	std::pair<Point, double> move_to[9];

	enum directions {
		left = 0,
		right = 1,
		up = 2,
		down = 3,
		h = 4,
		ur = 5,
		rd = 6,
		dl = 7,
		lu = 8,
	};

public:
	Unit(std::string& sprite_name, int x, int y, int colony, int id, int hp = 100, int maxhp = 100, int damage = 100, double agility = 0.5, states state = states::base) :
		hp(hp), maxhp(maxhp), damage(damage), agility(agility), Visual_Object(sprite_name, x, y), colony(colony), id(id), state(state), prev_state(state) { }


	virtual Point make_move(Level& lvl) = 0;
	virtual void set_state(states s) { state = s; prev_state = s; }
	void colony_temp_state(states s) { state = s; }
	void set_prev_state() { state = prev_state; }
	bool get_damage(int damage);
	int get_colony() { return colony; }
	size_t get_id() { return id; }
	int get_hp() { return hp; }
	virtual void show_info();
	virtual std::string get_type() = 0;

	void update() override {
		is_new_position = false;
		update_position();
		Visual_Object::update();
		Point pos = get_cur_pos();
		al_draw_bitmap(Defines::col_nums[colony - 1], pos.x * 10, pos.y * 10, 0);
	}

protected:
	virtual double AI_polynome(size_t x) { return 10 - x; }
	virtual void lookout(Level& lvl, std::unordered_map<int, double>& attractive, std::unordered_map<int, double>& own_attractive);
	virtual void lookout_point(Point p, Level& lvl, std::unordered_map<int, double>& attractive, std::unordered_map<int, double>& own_attractive);
	virtual void move_if_no_hp(Level& lvl);

	static void update_ai_attractive(std::unordered_map<int, double>& attractive_map);
	//virtual void set_user_ai_map_enemies() = 0;
	//virtual void set_user_ai_map_own() = 0;
};




class Soldier : public Unit {
	static std::unordered_map<int, double> attractive;
	static std::unordered_map<int, double> own_attractive;
	
	static std::unordered_map<int, double> protect_points_attractive;
	static std::unordered_map<int, double> protect_points_own_attractive;

	static std::unordered_map<int, double> user_attractive;
	static std::unordered_map<int, double> user_own_attractive;

public:
	Soldier(std::string sprite_name, int x, int y, int colony, int id, states state = states::base, int hp = 100, int maxhp = 100, int damage = 10, double agility = 0.1) :
		Unit(sprite_name, x, y, colony, id, hp, maxhp, damage, agility, state) {}

	Point make_move(Level& lvl) override;
	std::string get_type() override { return "Soldier"; }
	static void set_user_ai_map_enemies() { update_ai_attractive(user_attractive); }
	static void set_user_ai_map_own() { update_ai_attractive(user_own_attractive); }
};



class Defender : public Unit {
	static std::unordered_map<int, double> attractive;
	static std::unordered_map<int, double> own_attractive;
	
	static std::unordered_map<int, double> protect_points_attractive;
	static std::unordered_map<int, double> protect_points_own_attractive;

	static std::unordered_map<int, double> user_attractive;
	static std::unordered_map<int, double> user_own_attractive;

	int point_damage = 12;
	int inside_point_damage = 3;
	bool inside_point = false;

public:
	Defender(std::string sprite_name, int x, int y, int colony, int id, states state = states::base, int hp = 100, int maxhp = 100, int damage = 10, double agility = 0.3) :
		Unit(sprite_name, x, y, colony, id, hp, maxhp, damage, agility, state) {}

	Point make_move(Level& lvl) override;
	void show_info() override;
	std::string get_type() override { return "Defender"; }
	static void set_user_ai_map_enemies() { update_ai_attractive(user_attractive); }
	static void set_user_ai_map_own() { update_ai_attractive(user_own_attractive); }

	void update() override;
};


class Invader : public Unit {
	static std::unordered_map<int, double> attractive;
	static std::unordered_map<int, double> own_attractive;

	static std::unordered_map<int, double> protect_points_attractive;
	static std::unordered_map<int, double> protect_points_own_attractive;

	static std::unordered_map<int, double> user_attractive;
	static std::unordered_map<int, double> user_own_attractive;

	int point_damage = 20;
public:
	Invader(std::string sprite_name, int x, int y, int colony, int id, states state = states::base, int hp = 100, int maxhp = 100, int damage = 8, double agility = 0.2) :
		Unit(sprite_name, x, y, colony, id, hp, maxhp, damage, agility, state) {}

	Point make_move(Level& lvl) override;
	void show_info() override;
	std::string get_type() override { return "Invader"; }
	static void set_user_ai_map_enemies() { update_ai_attractive(user_attractive); }
	static void set_user_ai_map_own() { update_ai_attractive(user_own_attractive); }
};
