#include "Units.h"
#include "application.h"
#include <algorithm>


void Unit::lookout_point(Point p, Level& lvl, std::unordered_map<int, double>& attractive, std::unordered_map<int, double>& own_attractive) {
	Field& field = lvl.field;
	Field::BACK& back = field.get_back();
	Field::FRONT& front = field.get_front();
	Point pos = Visual_Object::get_cur_pos();

	int c_x = p.x, c_y = p.y;

	if (front[c_x][c_y] == -1) return;

	int colony_owner = front[c_x][c_y] % Defines::offset();
	int type = _Get_Type(front[c_x][c_y]);


	int value;
	if (colony_owner == colony) value = own_attractive.at(type);
	else value = attractive.at(type);

	int rel_x = c_x - pos.x, rel_y = c_y - pos.y;

	//coeff = coeff(distance);
	double coeff = value * AI_polynome(std::max(abs(rel_x), abs(rel_y)));

	if (state == protect_spawner && p == lvl.colonies[colony - 1].get_spawner_pos()) coeff = 5000;

	//normal directions check
	if (rel_x < 0) {
		if (coeff > 0) move_to[left].second += coeff;
		else move_to[right].second += abs(coeff);
	}
	else if (rel_x > 0) {
		if (coeff > 0) move_to[right].second += coeff;
		else move_to[left].second += abs(coeff);
	}

	if (rel_y < 0) {
		if (coeff > 0) move_to[up].second += coeff;
		else move_to[down].second += abs(coeff);
	}
	else if (rel_y > 0) {
		if (coeff > 0) move_to[down].second += coeff;
		else move_to[up].second += abs(coeff);
	}

	//diagonal directions
	if (rel_x < 0) {
		if (rel_y >= 0) {
			if (coeff > 0) move_to[dl].second += coeff;
			else move_to[ur].second += abs(coeff);
		}
		if (rel_y <= 0) {
			if (coeff > 0) move_to[lu].second += coeff;
			else move_to[rd].second += abs(coeff);
		}
	}
	else if (rel_x > 0) {
		if (rel_y >= 0) {
			if (coeff > 0) move_to[rd].second += coeff;
			else move_to[lu].second += abs(coeff);
		}
		if (rel_y <= 0) {
			if (coeff > 0) move_to[ur].second += coeff;
			else move_to[dl].second += abs(coeff);
		}
	}

	else //rel_x = 0
	{
		if (rel_y > 0) {
			if (coeff > 0) {
				move_to[rd].second += coeff;
				move_to[dl].second += coeff;
			}
			else {
				move_to[lu].second += abs(coeff);
				move_to[ur].second += abs(coeff);
			}
		}
		else if (rel_y < 0) {
			if (coeff > 0) {
				move_to[lu].second += coeff;
				move_to[ur].second += coeff;
			}
			else {
				move_to[rd].second += abs(coeff);
				move_to[dl].second += abs(coeff);
			}
		}

		else {
			if (type == salt || type == acid || type == meal)
				move_to[h].second += abs(coeff);
		}
	}
}

void Unit::lookout(Level& lvl, std::unordered_map<int, double>& attractive, std::unordered_map<int, double>& own_attractive) {
	Field& field = lvl.field;
	Point pos = Visual_Object::get_cur_pos();

	move_to[left].first = Point(-1, 0);
	move_to[right].first = Point(1, 0);
	move_to[up].first = Point(0, -1);
	move_to[down].first = Point(0, 1);
	move_to[h].first = Point(0, 0);
	move_to[ur].first = Point(1, -1);
	move_to[rd].first = Point(1, 1);
	move_to[dl].first = Point(-1, 1);
	move_to[lu].first = Point(-1, -1);

	for (int i = 0; i < 9; ++i) {
		move_to[i].second = 0;
	}
	if (hp > 0) {
		for (int c_y = std::max(pos.y - 20, 0); c_y <= std::min(pos.y + 20, int(field.get_height()) - 1); ++c_y) {
			for (int c_x = std::max(pos.x - 20, 0); c_x <= std::min(pos.x + 20, int(field.get_width()) - 1); ++c_x) {
				lookout_point(Point(c_x, c_y), lvl, attractive, own_attractive);
			}
		}
		if (state == protect_spawner) {
			Point sp_pos = lvl.colonies[colony - 1].get_spawner_pos();
			lookout_point(sp_pos, lvl, attractive, own_attractive);
		}

		std::sort(move_to, move_to + 9,
			[](auto& left, auto& right) { return left.second > right.second; });
	}
}


void Unit::move_if_no_hp(Level& lvl) {
	Point pos = get_cur_pos();
	Field& field = lvl.field;
	Field::FRONT& front = field.get_front();
	std::vector<Colony>& colonies = lvl.colonies;

	for (int c_y = std::max(pos.y - 1, 0); c_y <= std::min(pos.y + 1, int(field.get_height()) - 1); ++c_y) {
		for (int c_x = std::max(pos.x - 1, 0); c_x <= std::min(pos.x + 1, int(field.get_width()) - 1); ++c_x) {
			if (front[c_x][c_y] == -1) continue;

			int poss_id = front[c_x][c_y];
			int col_own = poss_id % Defines::offset();
			if (col_own == colony) continue;

			int type = _Get_Type(front[c_x][c_y]);

			if (type == soldier || type == hunter || type == defender || type == invader) {
				Unit* u = colonies[col_own - 1].get_unit(poss_id);
				bool succesull = u->get_damage(damage * 1.5);
				if (!succesull) continue;
				return;
			}
		}
	}
}



Point Soldier::make_move(Level& lvl) {
	Field& field = lvl.field;
	std::vector<Colony>& colonies = lvl.colonies;
	Point pos = Visual_Object::get_cur_pos();
	Field::BACK& back = field.get_back();
	Field::FRONT& front = field.get_front();
	Point new_pos;


	if (hp <= 0) {
		move_if_no_hp(lvl);
		return pos;
	}

	switch (state) {
	case Unit::base:
		lookout(lvl, attractive, own_attractive);
		break;

	case Unit::protect_point:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;

	case Unit::protect_spawner:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;
	
	case Unit::user:
		lookout(lvl, user_attractive, user_own_attractive);
		break;
	}

	bool equal_choose = (move_to[0].second == 0);


	//--------------------------------------------------non-random variant-----------------------------------------------
	if (!equal_choose) {
		Point poss;
		for (int i = 0; i < 9; ++i) {
			poss = move_to[i].first + pos;
			if (poss.x < 0 || poss.x >= field.get_width() || poss.y < 0 || poss.y >= field.get_height()) continue;
			int poss_id = front[poss.x][poss.y];
			if (back[poss.x][poss.y].name == 0) {
				if ((poss_id == -1 || poss_id == id) && hp > 0) {
					is_new_position = true;
					new_pos = poss;
					front[pos.x][pos.y] = -1;
					front[new_pos.x][new_pos.y] = id;
					if (poss_id != id) {
						field.draw_back_at_position(pos.x, pos.y);
					}
					break;
				}
				size_t col_own = poss_id % Defines::offset();
				if (col_own == colony) continue;

				int type = _Get_Type(poss_id);
				if (type == soldier || type == hunter || type == defender || type == invader) {
					Unit* u = colonies[col_own - 1].get_unit(poss_id);
					bool succesull = u->get_damage(damage);
					if (!succesull) continue;
					new_pos = pos;
					break;
				}
			}
		}
		return new_pos;
	}

	//-------------------------------------------------random variant-----------------------------------------------
	else {
		int n_of_tries = 0;
		while (true) {
			n_of_tries += 1;
			if (n_of_tries > 100) {
				new_pos = pos;
				break;
			}

			int i = rand() % 3 - 1;
			int j = rand() % 3 - 1;
			if (pos.x + i >= 0 && pos.y + j >= 0 && pos.x + i < field.get_width() && pos.y + j < field.get_height() && back[pos.x + i][pos.y + j].name == 0) {
				int en_id = front[pos.x + i][pos.y + j];
				if (en_id == -1 || en_id == id) {
					is_new_position = true;
					new_pos.x = pos.x + i;
					new_pos.y = pos.y + j;
					front[pos.x][pos.y] = -1;
					front[new_pos.x][new_pos.y] = id;

					if (en_id != id)
						field.draw_back_at_position(pos.x, pos.y);
					break;
				}
				else {
					int type = _Get_Type(en_id);
					if (type == soldier || type == hunter || type == defender || type == invader) {
						int col_own = en_id % Defines::offset() - 1;
						if (col_own + 1 == colony) continue;
						Unit* u = colonies[col_own].get_unit(en_id);
						bool succesull = u->get_damage(damage);
						if (!succesull) continue;

						new_pos = pos;
						break;
					}
				}
			}
		}
		return new_pos;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------


Point Invader::make_move(Level& lvl) {
	Field& field = lvl.field;
	std::vector<Colony>& colonies = lvl.colonies;
	Point pos = Visual_Object::get_cur_pos();
	Field::BACK& back = field.get_back();
	Field::FRONT& front = field.get_front();
	Point new_pos;

	if (hp <= 0) {
		move_if_no_hp(lvl);
		return pos;
	}

	switch (state) {
	case Unit::base:
		lookout(lvl, attractive, own_attractive);
		break;

	case Unit::protect_point:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;

	case Unit::protect_spawner:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;

	case Unit::user:
		lookout(lvl, user_attractive, user_own_attractive);
		break;
	}

	bool equal_choose = (move_to[0].second == 0);

	//--------------------------------------------------non-random variant-----------------------------------------------
	if (!equal_choose) {
		Point poss;
		for (int i = 0; i < 9; ++i) {
			poss = move_to[i].first + pos;
			if (poss.x < 0 || poss.x >= field.get_width() || poss.y < 0 || poss.y >= field.get_height()) continue;
			int poss_id = front[poss.x][poss.y];
			if (back[poss.x][poss.y].name == 0) {
				if ((poss_id == -1 || poss_id == id) && hp > 0) {
					is_new_position = true;
					new_pos = poss;
					front[pos.x][pos.y] = -1;
					front[new_pos.x][new_pos.y] = id;

					if (poss_id != id) {
						field.draw_back_at_position(pos.x, pos.y);
					}
					break;
				}

				size_t col_own = poss_id % Defines::offset();
				if (col_own == colony) continue;

				int type = _Get_Type(poss_id);
				if (type == soldier || type == hunter || type == defender || type == invader) {
					Unit* u = colonies[col_own - 1].get_unit(poss_id);
					bool succesull = u->get_damage(damage);
					if (!succesull) continue;
					new_pos = pos;
					break;
				}
				else if (type == salt || type == acid || type == meal) {
					Resource_Point* r = lvl.t.get_point(poss_id / Defines::offset());
					r->get_damage(point_damage);
					if (r->get_hp() <= 0) {
						r->update_owner(colony, field, colonies);
					}

					new_pos = pos;
					break;
				}

				else if (type == spawner) {
					colonies[col_own - 1].get_damage_spawner(point_damage);
					new_pos = pos;
					break;
				}
			}
		}
		return new_pos;
	}

	//-------------------------------------------------random variant-----------------------------------------------
	else {
		int n_of_tries = 0;
		while (true) {
			n_of_tries += 1;
			if (n_of_tries > 100) {
				new_pos = pos;
				break;
			}

			int i = rand() % 3 - 1;
			int j = rand() % 3 - 1;
			if (pos.x + i >= 0 && pos.y + j >= 0 && pos.x + i < field.get_width() && pos.y + j < field.get_height() && back[pos.x + i][pos.y + j].name == 0) {
				int en_id = front[pos.x + i][pos.y + j];
				if (en_id == -1 || en_id == id) {
					is_new_position = true;
					new_pos.x = pos.x + i;
					new_pos.y = pos.y + j;
					front[pos.x][pos.y] = -1;
					front[new_pos.x][new_pos.y] = id;

					if (en_id != id)
						field.draw_back_at_position(pos.x, pos.y);
					break;
				}
				else {
					int type = _Get_Type(en_id);
					if (type == soldier || type == hunter || type == defender || type == invader) {
						int col_own = en_id % Defines::offset() - 1;
						if (col_own + 1 == colony) continue;
						Unit* u = colonies[col_own].get_unit(en_id);
						bool succesull = u->get_damage(damage);
						if (!succesull) continue;

						new_pos = pos;
						break;
					}
				}
			}
		}
		return new_pos;
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------


Point Defender::make_move(Level& lvl) { //attack points
	Field& field = lvl.field;
	std::vector<Colony>& colonies = lvl.colonies;
	Point pos = Visual_Object::get_cur_pos();
	Field::BACK& back = field.get_back();
	Field::FRONT& front = field.get_front();
	Point new_pos;

	if (hp <= 0 && !inside_point) {
		move_if_no_hp(lvl);
		return pos;
	}

	switch (state) {
	case Unit::base:
		lookout(lvl, attractive, own_attractive);
		break;

	case Unit::protect_point:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;

	case Unit::protect_spawner:
		lookout(lvl, protect_points_attractive, protect_points_own_attractive);
		break;

	case Unit::user:
		lookout(lvl, user_attractive, user_own_attractive);
		break;
	}

	bool equal_choose = (move_to[0].second == 0);

	//--------------------------------------------------non-random variant-----------------------------------------------
	if (!equal_choose) {
		Point poss;
		for (int i = 0; i < 9; ++i) {
			poss = move_to[i].first + pos;
			if (poss.x < 0 || poss.x >= field.get_width() || poss.y < 0 || poss.y >= field.get_height()) continue;
			int poss_id = front[poss.x][poss.y];
			if (back[poss.x][poss.y].name == 0) {

				if ((poss_id == -1 || poss == pos) && hp > 0) {					//go to empty point or stay in place

					new_pos = poss;

					if (inside_point && poss != pos) {
						//delete_unit fron defenders of point
						Resource_Point* r = lvl.t.get_point(front[pos.x][pos.y] / Defines::offset());							//выселение
						r->delete_defender(this);
					}

					if (!inside_point) {							//dont rewrite if was inside
						front[pos.x][pos.y] = -1;
						field.draw_back_at_position(pos.x, pos.y);
					}

					if (poss != pos || !inside_point)
						front[new_pos.x][new_pos.y] = id;

					if (poss != pos) {
						is_new_position = true;
						inside_point = false;
					}

					break;
				}

				size_t col_own = poss_id % Defines::offset();

				int type = _Get_Type(poss_id);

				//go to own colony												//заселение
				if (!inside_point && col_own == colony) {
					if (type == salt || type == acid || type == meal){
						new_pos = poss;
						if (!inside_point) {
							front[pos.x][pos.y] = -1;
							field.draw_back_at_position(pos.x, pos.y);
						}

						inside_point = true;

						Resource_Point* r = lvl.t.get_point(poss_id / Defines::offset());
						r->add_defender(this);
						break;
					}
				}

				//attack smth but nnot from own colony
				else if(col_own != colony){
					if (type == soldier || type == hunter || type == defender || type == invader) {
						Unit* u = colonies[col_own - 1].get_unit(poss_id);
						bool succesfull;
						if(inside_point)
							succesfull = u->get_damage(inside_point_damage);
						else
							succesfull = u->get_damage(damage);

						if (!succesfull) continue;
						new_pos = pos;
						break;
					}
					if (!inside_point) {
						if (type == salt || type == acid || type == meal) {
							Resource_Point* r = lvl.t.get_point(poss_id / Defines::offset());
							r->get_damage(point_damage);
							if (r->get_hp() <= 0) {
								r->update_owner(colony, field, colonies);
							}

							new_pos = pos;
							break;
						}

						else if (type == spawner) {
							colonies[col_own - 1].get_damage_spawner(point_damage);
							new_pos = pos;
							break;
						}
					}
				}
			}
		}
		return new_pos;
	}

	//-------------------------------------------------random variant-----------------------------------------------
	else {
		int n_of_tries = 0;
		while (true) {
			n_of_tries += 1;
			if (n_of_tries > 100) {
				new_pos = pos;
				break;
			}

			int i = rand() % 3 - 1;
			int j = rand() % 3 - 1;
			if (pos.x + i >= 0 && pos.y + j >= 0 && pos.x + i < field.get_width() && pos.y + j < field.get_height() && back[pos.x + i][pos.y + j].name == 0) {
				Point poss(pos.x + i, pos.y + j);
				int poss_id = front[pos.x + i][pos.y + j];
				int en_id = front[pos.x + i][pos.y + j];
				if ((poss_id == -1 || poss == pos) && hp > 0) {					//go to empty point or stay in place

					new_pos = poss;

					if (inside_point && poss != pos) {
						//delete_unit fron defenders of point
						Resource_Point* r = lvl.t.get_point(front[pos.x][pos.y] / Defines::offset());							//выселение
						r->delete_defender(this);
					}

					if (!inside_point) {							//dont rewrite if was inside
						front[pos.x][pos.y] = -1;
						field.draw_back_at_position(pos.x, pos.y);
					}

					if (poss != pos || !inside_point)
						front[new_pos.x][new_pos.y] = id;

					if (poss != pos) {
						is_new_position = true;
						inside_point = false;
					}

					break;
				}


				else {
					int type = _Get_Type(en_id);
					if (type == soldier || type == hunter || type == defender || type == invader) {
						int col_own = en_id % Defines::offset() - 1;
						if (col_own + 1 == colony) continue;
						Unit* u = colonies[col_own].get_unit(en_id);
						bool succesull = u->get_damage(damage);
						if (!succesull) continue;

						new_pos = pos;
						break;
					}
				}
			}
		}
		return new_pos;
	}
}


void Defender::update(){
	if(!inside_point)
		Unit::update();
	else {
		is_new_position = false;
		update_position();
	}
}


bool Unit::get_damage(int damage) {
	if (is_new_position) {
		return false;
	}
	damage *= (1 - agility);
	hp -= damage;
	if (hp <= 0) {
		is_dead = true;
	}
	return true;
}

void Unit::show_info() {
	std::cout << "\nID: " << id << std::endl;
	std::cout << "\tType: " << get_type() << std::endl;
	std::cout << "\tColony owner: " << colony << std::endl;
	std::cout << "\tHp: " << hp << std::endl;
	std::cout << "\tMaxHp:" << maxhp << std::endl;
	std::cout << "\tDamage to other units: " << damage << std::endl;
	std::cout << "\tAgility: " << agility << std::endl;
	std::cout << "\tState: ";
	switch (state) {
	case Unit::base:
		std::cout << "Base state";
		break;
	case Unit::protect_point:
		std::cout << "Protect points state";
		break;	
	case Unit::protect_spawner:
		std::cout << "Protect spawner state";
		break;
	case Unit::user:
		std::cout << "User state";
		break;
	}
	std::cout << std::endl;
}

void Defender::show_info() {
	Unit::show_info();
	std::cout << "Point Damage: " << point_damage << std::endl;
	std::cout << "Inside Point Damage: " << inside_point_damage << std::endl;
}

void Invader::show_info() {
	Unit::show_info();
	std::cout << "Point Damage: " << point_damage << std::endl;
}


void Unit::update_ai_attractive(std::unordered_map<int, double>& attractive_map) {
	std::cout << "\tSoldier: ";
	own_cin(attractive_map.at(soldier));

	std::cout << "\tDefender: ";
	own_cin(attractive_map.at(defender));

	std::cout << "\tInvader: ";
	own_cin(attractive_map.at(invader));

	std::cout << "\tSpawner: ";
	own_cin(attractive_map.at(spawner));

	std::cout << "\tAcid Lakes: ";
	own_cin(attractive_map.at(acid));

	std::cout << "\tSalt Caves: ";
	own_cin(attractive_map.at(salt));

	std::cout << "\tFood Points: ";
	own_cin(attractive_map.at(meal));
}