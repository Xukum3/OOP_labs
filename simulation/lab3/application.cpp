#include "application.h"
using std::cout;
using std::cin;
using std::endl;


void Level::count_tick() {
	for (auto c_it = colonies.begin(); c_it != colonies.end(); ++c_it) {
		if(c_it->is_alive())
			(*c_it).update_units(*this);
	}
	for (auto c_it = colonies.begin(); c_it != colonies.end(); ++c_it) {
		if (c_it->is_alive())
			c_it->update_resources();
	}
}


void Field::update_size(size_t w, size_t h) {
	if (w < width || h < height) throw std::invalid_argument("Incorrect new size");
	width = w;
	height = h;
}



void Application::ready_to_simulation() {
	srand(time(0));

	al_init();
	al_install_keyboard();
	al_init_image_addon();

	display = al_create_display(1000, 1000);
	al_clear_to_color(al_map_rgba(1, 1, 1, 1));

	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));

	rend.gen_level(lvl);
	rend.draw_field(lvl.get_back());

	rend.draw_colonies(lvl);
	rend.draw_sprites(lvl.t);
	al_flip_display();
}

void Application::simulate() {
	while (running) {
		run_simulation();
		if (!running)
			break;
		else
			pause_sim();
	}
}

void Application::end_simulation() {
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	cout << "\n\n\nGood bye)";
}

void Application::run_simulation() {
	cout << "Press \"p\" to pause simulation\n";

	timer = al_create_timer(1.0 / fps);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	while (running) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			running = false;
		}

		if (event.type == ALLEGRO_EVENT_KEY_UP) {
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				running = false;

			case ALLEGRO_KEY_P:
				pause = true;
				return;
			}
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {

			lvl.count_tick();
			rend.draw_sprites(lvl.t);
			rend.draw_colonies(lvl);

			al_flip_display();
			rend.redraw_blood_points(lvl.field);
		}
	}
}

void Application::pause_sim() {
	double new_fps;
	bool pause = true;
	al_flush_event_queue(queue);
	al_unregister_event_source(queue, al_get_timer_event_source(timer));
	
	while (pause) {
		int action = -1, ans = -1;
		cout << "What you want to do:\n";
		cout << "\tPress 1 to change fps\n";
		cout << "\tPress 2 to change required resources for units creating\n";
		cout << "\tPress 3 get/set colonies characteristics\n";
		cout << "\tPress 4 to get/set resorce points characteristics\n";
		cout << "\tPress 0 to return to simulation\n";
		own_cin(action);
		switch (action) {
		case 0:
			cout << "Returns to simlation...\n\n";
			pause = false;
			break;
		case 1:
			new_fps = 0;
			cout << "Enter new fps (from 0.2 to 30): ";
			while (new_fps < 0.2 || new_fps > 30) {
				own_cin(new_fps);
			}
			change_fps(new_fps);
			break;

		case 2:
			Spawner::set_new_req_resources();
			break;

		case 3:
			colonies_action();
			break;

		case 4:
			res_points_action();
			break;

		default:
			cout << "You entered something wrong";
		}
	}
}

void Application::res_points_action() {
	bool in_foo = true;
	while (in_foo) {
		int ans = -1;
		cout << "\nWhat do you want?\n";
		cout << "\tPress 1 to show info about points\n";
		cout << "\tPress 2 to change resource level for point\n";
		cout << "\tPress 3 to change resource level for all points\n";
		cout << "\tPress 0 to return\n";
		own_cin(ans);
		switch (ans) {
		case 1:
			lvl.t.show_points_info();
			cout << endl;
			break;

		case 2:
			lvl.t.update_resource_level(lvl);
			break;

		case 3:
			lvl.t.update_all_resource_level(lvl);
			break;

		case 0:
			in_foo = false;
			break;

		default:
			cout << "Repeat please\n";
			break;
		}
	}
}

//------------------------------------------------------------------------------------------------------
void Point_Table::show_points_info() {
	for (auto it = points.begin(); it != points.end(); ++it) {
		std::cout << "ID: " << it->first << std::endl;
		std::cout << "\tType: " << it->second->get_type() << std::endl;
		std::cout << "\tPosition: " << it->second->get_cur_pos() << std::endl;
		std::cout << "\tOwner: colony #" << it->second->get_colony_owner() << std::endl;
		std::cout << "\tHp: " << it->second->get_hp() << std::endl;
		std::cout << "\tNumber of defenders: " << it->second->get_num_of_defenders() << std::endl;
		std::cout << "\tResource level: " << it->second->get_resource() << endl << endl;
	}
}

void Point_Table::update_resource_level(Level& lvl) {
	std::cout << "Enter id of point: ";
	int find_id;
	own_cin(find_id);
	if (points.contains(find_id)) {
		Resource_Point* r = points.at(find_id);
		int new_level = -1;
		std::cout << "Enter new level of resources (> 0): ";
		while (new_level < 1) {
			own_cin(new_level);
		}

		r->update_resource_level(new_level, lvl.colonies);
		cout << "Sucessfull\n";
	}
	else {
		std::cout << "No point with this id(\n";
	}
}

void Point_Table::update_all_resource_level(Level& lvl) {
	int new_level = -1;
	std::cout << "Enter new level of resources (> 0): ";
	while (new_level < 1) {
		own_cin(new_level);
	}
	for (auto it = points.begin(); it != points.end(); ++it) {
		it->second->update_resource_level(new_level, lvl.colonies);
	}
}
//--------------------------------------------------------------------------------------------------------

void Application::colonies_action() {
	bool in_foo = true;
	while (in_foo) {
		int ans = -1;
		cout << "\nWhat do you want?\n";
		cout << "\tPress 1 to show info about colonies\n";
		cout << "\tPress 2 to show all units of colony\n";
		cout << "\tPress 3 to change units states\n";
		cout << "\tPress 4 to set attractive coefficients for own state\n";
		cout << "\tPress 0 to return\n";
		own_cin(ans);
		switch (ans) {
		case 1:
			for (auto it = lvl.colonies.begin(); it != lvl.colonies.end(); ++it) {
				it->show_info();
			}
			cout << endl;
			break;

		case 2:
			show_all_units();
			break;

		case 3:
			change_units_states();
			break;

		case 4:
			change_attractive();
			break;

		case 0:
			in_foo = false;
			break;
		}
	}
}


void Application::show_all_units() {
	int number = -1;
	cout << "Enter number of colony (from 1 to " << lvl.colonies.size() << ")";

	while (number < 1 || number > lvl.colonies.size()) {
		own_cin(number);
	}
	lvl.colonies[number - 1].show_all_units();
}


void get_state(Unit::states& state) {
	int n_state = -1;
	std::cout << "Enter\n\t0: Base state\n\t1: Protect points state\n\t2: Protect spawner state\n\t3: Own state\n";
	while (n_state < 0 || n_state > 3) { own_cin(n_state); }
	switch (n_state) {
	case 0:
		state = Unit::base;
		break;
	case 1:
		state = Unit::protect_point;
		break;
	case 2:
		state = Unit::protect_spawner;
		break;
	case 3:
		state = Unit::user;
		break;
	}
}


void Application::change_units_states() {
	bool in_foo = true;
	while (in_foo) {
		int ans = -1;
		Unit::states state;
		int colony = -1;
		cout << "\nWhat do you want?\n";
		cout << "\tPress 1 to change state of all units\n";
		cout << "\tPress 2 to change state of all units in one colony\n";
		cout << "\tPress 0 to return\n";
		own_cin(ans);
		switch (ans) {
		case 1:
			get_state(state);
			for (auto it = lvl.colonies.begin(); it != lvl.colonies.end(); ++it) {
				it->set_all_units_state(state);
			}
			cout << endl;
			break;

		case 2:
			get_state(state);
			cout << "Enter number of colony (from 1 to " << lvl.colonies.size() << ")";
			while (colony < 1 || colony > lvl.colonies.size()) { own_cin(colony); }

			lvl.colonies[colony - 1].set_all_units_state(state);
			cout << endl;
			break;

		case 0:
			in_foo = false;
			break;

		default:
			cout << "Repeat please\n";
			break;
		}
	}
}


void Application::change_attractive() {
	cout << "Soldier:\n";
	cout << "\tAttrativeness of enemies:\n";
	Soldier::set_user_ai_map_enemies();
	cout << "\tAttractiveness of freinds:\n";
	Soldier::set_user_ai_map_own();
	cout << endl;

	cout << "Defender:\n";
	cout << "Attrativeness of enemies:\n";
	Defender::set_user_ai_map_enemies();
	cout << "Attractiveness of freinds:\n";
	Defender::set_user_ai_map_own();
	cout << endl;

	cout << "Invader:\n";
	cout << "Attrativeness of enemies:\n";
	Invader::set_user_ai_map_enemies();
	cout << "Attractiveness of freinds:\n";
	Invader::set_user_ai_map_own();
	cout << endl;
}
