#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../Unordered Map/Unordered_Map.h"
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include "classes.h"
#include "defines.h"
#include "buildings.h"
#include "application.h"
#include "Colony.h"
#include <conio.h>


class Point_Table {
private:
	Unordered_Map<int, Resource_Point*> points;
public:
	Resource_Point* get_point(int id) { return points.at(id); }
	void add_point(int id, Resource_Point* point) { points.insert(id, point); }

	Unordered_Map<int, Resource_Point*>::const_iterator begin() const noexcept{ return points.begin(); }
	Unordered_Map<int, Resource_Point*>::const_iterator end() const noexcept{ return points.end(); }

	void show_points_info();
	void update_resource_level(Level& lvl);
	void update_all_resource_level(Level& lvl);

	~Point_Table() {
		for (auto it = points.begin(); it != points.end(); ++it) {
			delete it->second;
		}
	}
};

class Field {
private:
	size_t width;
	size_t height;
	std::vector<std::vector<back_el>> back;
	std::vector<std::vector<int>>front;

public:
	Field(size_t x, size_t y) : width(x), height(y) {
		back.resize(width, std::vector<back_el>(height));
		front.resize(width, std::vector<int>(height, -1));
	}

	using BACK = std::vector<std::vector<back_el>>;
	using FRONT = std::vector<std::vector<int>>;

	std::vector<std::vector<back_el>>& get_back() { return back; }
	std::vector<std::vector<int>>& get_front() { return front; }

	void set_front_cell(int x, int y, int id) { front[x][y] = id; }
	int get_front_cell(int x, int y) { return front[x][y]; }
	int get_back_cell(int x, int y) { return back[x][y].name; }

	size_t get_width() { return width; }
	size_t get_height() { return height; }

	void update_size(size_t  w, size_t h);

	void draw_back_at_position(size_t x, size_t y) {
		al_draw_bitmap(back[x][y].bitmap, x * 10, y * 10, 0);
	}
};

//----------------------------------------------------------------------------------------------------------------
class Level {
public:
	Field field;
	Point_Table t;
	std::vector<Colony> colonies;

	std::vector<std::vector<back_el>>& get_back() { return field.get_back(); }
	std::vector<std::vector<int>>& get_front() { return field.get_front(); }

	size_t get_height() { return field.get_height(); }
	size_t get_width() { return field.get_width(); }

	Level(size_t w, size_t h) : field(w, h) { }

	void draw_blood(Point t) {
		al_draw_bitmap(Defines::blood, t.x * 10, t.y * 10, 0);
	}

	void count_tick();
};

//********************************************************************************
class Renderer { //взаимодействие с полем
private:
	void gen_field(std::vector<std::vector<back_el>>& back);
	void gen_sprites(std::vector<std::vector<int>>& sprite, std::vector<std::vector<back_el>>& back, Level& lvl);
	void gen_colonies(std::vector<std::vector<int>>& front, std::vector<std::vector<back_el>>& back, Level& lvl);

public:
	std::vector<Point> points_to_redraw;

	void gen_level(Level& lvl);

	void draw_field(std::vector<std::vector<back_el>>& back);
	void draw_colonies(Level& lvl);
	void draw_sprites(Point_Table& r_p);
	void redraw_blood_points(Field& field);
};


//----------------------------------------------------------------------------------------------------------------

class Application {
private:
	Renderer rend;
	Level lvl;
	double fps;
	bool running = true; 
	bool pause = false;

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT event;

private:
	void run_simulation();
	void pause_sim();
	void change_fps(double new_fps) { fps = new_fps; }
	void res_points_action();

	void colonies_action();
	void show_all_units();
	void change_units_states();
	void change_attractive();

public:
	void ready_to_simulation();
	void simulate();
	void end_simulation();

	Application(size_t width, size_t height, double fps) : lvl(width, height), fps(fps) { }
};