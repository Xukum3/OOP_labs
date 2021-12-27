#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <random>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "defines.h"


class Visual_Object {
private:
	ALLEGRO_BITMAP* bitmap;
	Point current;
	Point next;
public:
	virtual void update(){
		al_draw_bitmap(bitmap, current.x * 10, current.y * 10, 0);
	}

	void update_position() {
		current = next;
	}

	void set_next_position(int x, int y) {
		next.x = x;
		next.y = y;
	}
	void set_next_position(const Point& p) {
		next.x = p.x;
		next.y = p.y;
	}

	Visual_Object(std::string photo_source, size_t x = 0, size_t y = 0) : current(x, y) {
		bitmap = al_load_bitmap(photo_source.c_str());
	}

	void set_bitmap(const std::string& source) {
		al_destroy_bitmap(bitmap);
		bitmap = al_load_bitmap(source.c_str());
	}

	Point get_cur_pos() const {
		return current;
	}

	virtual ~Visual_Object() {
		al_destroy_bitmap(bitmap);
	}
};



