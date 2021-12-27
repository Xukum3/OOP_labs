#include "application.h"

void Renderer::gen_field(std::vector<std::vector<back_el>>& back) {
	for (size_t i = 0; i < back.size(); i++) {
		for (size_t j = 0; j < back[i].size(); j++) {
			int r = rand() % 10;
			if (r >= 9) {
				back[i][j].name = 1;
				back[i][j].bitmap = al_load_bitmap("lava.png");
			}
			else {
				back[i][j].name = 0;
				back[i][j].bitmap = al_load_bitmap("sand.png");
			}
		}
	}
}

void Renderer::gen_sprites(std::vector<std::vector<int>>& sprite, std::vector<std::vector<back_el>>& back, Level& lvl) {
	int r_x, r_y;

	for (size_t sz = 0; sz < Defines::n_salt(); sz++) {
		r_x = rand() % sprite.size();
		r_y = rand() % sprite[0].size();
		if (back[r_x][r_y].name == 0 && sprite[r_x][r_y] == -1) {
			int id = (sz + 1) * 100 + salt;
			sprite[r_x][r_y] = id * Defines::offset();
			//Resource_Point* s = new Salt_Cave(r_x, r_y, "pixil-frame-salt.png", 100);
			lvl.t.add_point(id, new Salt_Cave(r_x, r_y, "pixil-frame-salt.png", 100));
		}
		else sz -= 1;
	}
	for (size_t sz = 0; sz < Defines::n_acid(); sz++) {
		r_x = rand() % sprite.size();
		r_y = rand() % sprite[0].size();
		if (back[r_x][r_y].name == 0 && sprite[r_x][r_y] == -1) {
			int id = (sz + 1) * 100 + acid;
			sprite[r_x][r_y] = id * Defines::offset();
			//Resource_Point* a = new Acid_Lake(r_x, r_y, "pixil-frame-acid.png", 100);
			lvl.t.add_point(id, new Acid_Lake(r_x, r_y, "pixil-frame-acid.png", 100));
		}
		else sz -= 1;
	}
	for (size_t sz = 0; sz < Defines::n_meal(); sz++) {
		r_x = rand() % sprite.size();
		r_y = rand() % sprite[0].size();
		if (back[r_x][r_y].name == 0 && sprite[r_x][r_y] == -1) {
			int id = (sz + 1) * 100 + meal;
			sprite[r_x][r_y] = id * Defines::offset();
			//Resource_Point* f = new Food_Point(r_x, r_y, "pixil-frame-meal.png", 100);
			lvl.t.add_point(id, new Food_Point(r_x, r_y, "pixil-frame-meal.png", 100));
		}
		else sz -= 1;
	}
}

void Renderer::draw_field(std::vector<std::vector<back_el>>& back) {
	for (size_t i = 0; i < back.size(); i++) {
		for (size_t j = 0; j < back[i].size(); j++) {
			al_draw_bitmap(back[i][j].bitmap, i * 10, j * 10, 0);
			//al_draw_scaled_bitmap(back[i][j].bitmap, i * 10, j * 10, 10, 10, i * 10, j * 10, 10, 10, 0);
		}
	}
}

void Renderer::draw_sprites(Point_Table& r_p) {

	for (auto it = r_p.begin(); it != r_p.end(); ++it) {
		it->second->update();
	}
}

void Renderer::redraw_blood_points(Field& field) {
	for (auto it : points_to_redraw) {
		field.draw_back_at_position(it.x, it.y);
	}
	points_to_redraw.clear();
}

void Renderer::gen_level(Level& lvl) {
	Defines::blood = al_load_bitmap("blood2.png");
	Defines::col_nums[0] = al_load_bitmap("1.png");
	Defines::col_nums[1] = al_load_bitmap("2.png");
	Defines::col_nums[2] = al_load_bitmap("3.png");
	Defines::col_nums[3] = al_load_bitmap("4.png");
	Defines::col_nums[4] = al_load_bitmap("5.png");
	Defines::col_nums[5] = al_load_bitmap("6.png");
	Defines::col_nums[6] = al_load_bitmap("7.png");
	Defines::col_nums[7] = al_load_bitmap("8.png");
	Defines::col_nums[8] = al_load_bitmap("9.png");
	Field::FRONT& front = lvl.get_front();
	Field::BACK& back = lvl.get_back();
	ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
	al_draw_bitmap(background, 0, 0, 0);
	gen_field(back);
	gen_sprites(front, back, lvl);
	gen_colonies(front, back, lvl);

	points_to_redraw.reserve(50);
}

void Renderer::gen_colonies(std::vector<std::vector<int>>& front, std::vector<std::vector<back_el>>& back, Level& lvl)
{
	int r_x, r_y;

	for (size_t sz = 0; sz < Defines::n_col(); sz++) {
		r_y = rand() % lvl.field.get_height();
		r_x = rand() % lvl.field.get_width();
		if (back[r_x][r_y].name == 0 && front[r_x][r_y] == -1) {
			front[r_x][r_y] = spawner * Defines::offset() + sz + 1;
			Spawner* s = new Spawner("pixil-frame-spawner.png", r_x, r_y, sz + 1);
			lvl.colonies.push_back(Colony(sz + 1, s));
		}
		else sz -= 1;
	}

	for (int i = 0; i < Defines::n_col() * 2; i++) {
		r_y = rand() % lvl.get_height();
		r_x = rand() % lvl.get_width();
		if (back[r_x][r_y].name == 0 && front[r_x][r_y] == -1) {
			int id = lvl.colonies[i % Defines::n_col()].generate_id(invader);
			front[r_x][r_y] = id;

			Unit* u = nullptr;
			u = new Invader("invader.png", r_x, r_y, i % Defines::n_col() + 1, id);

			u->set_next_position(u->get_cur_pos());
			assert(u->get_colony() == (i % Defines::n_col()) + 1);
			lvl.colonies[i % Defines::n_col()].add_unit(id, u);
		}
		else i -= 1;
	}
}

void Renderer::draw_colonies(Level& lvl)
{
	for (size_t i = 0; i < lvl.colonies.size(); ++i) {
		if (lvl.colonies[i].is_alive()) {
			if (lvl.colonies[i].hp() < 0) {
				lvl.colonies[i].delete_all_units(lvl.field);
			}
			else
				lvl.colonies[i].draw(lvl, points_to_redraw);
		}
	}
}