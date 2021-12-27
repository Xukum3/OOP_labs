#include "application.h"

/*
To-Do list

1) GRAFICAL PART:
	1) Choose grafic framework allegro|sfml					done: allegro
	2) Renderer and sprite									done
	3) Make move-method of the Unit							done
	4) Generate the field									done
	5) Draw all												done
...
2) GAME PART:
	0) colony												done
	1) decide what to do with states						

	2) make lookout method    --> get map					done
							  --> see what near				done
							  --> choose priority cells		done
	3) make_decision										done 

	4) get resource_point									done
	5) update resources of the colony						done

	6) spawner												done

	------------------------------delete or unique_ptr or my_ptr------------------------------

	7) renderer, application								done
	8) connect all in simulation							done
	9) make pause
		1) change spawner ch-tics								done
		2) get point info										done
		3) change point info (resource level)					done
		4) get unit info										done	

		5) change unit info
					---> get/set
					---> make own ai map

	10) make units protect spawner when its hp is increasing 

*/

//**********************************************************************************************

int main() {
	size_t width = 200, height = 200;
	double fps = 200;
	int number;

	std::cout << "Please! Don't enter wrong symbols(\n\n";
	std::cout << "Ebter width and height of field (from 20 to 100): \n";

	std::cout << "Width: ";
	while (width > 100 || height < 20) { own_cin(width); }

	std::cout << "Height: ";
	while (height > 100 || height < 20) { own_cin(height); }

	std::cout << "\n" << "Enter fps (from 0.2 to 30): ";
	while (fps < 1 || fps > 30) { own_cin(fps); }

	number = 0;
	std::cout << "Enter number of colonies (from 2 to 9): ";
	while (number < 2 || number > 9) { own_cin(number); }

	Defines::n_col() = number;
	Defines::log_col() = log10(number);
	Defines::offset() = pow(10, Defines::log_col() + 1);


	int recommended = (width * height) / 100 / 5 * 2;
	number = 0;
	std::cout << "Enter number of acid lakes (recommended < " << recommended << "): ";
	while (number == 0) { own_cin(number); }
	Defines::n_acid() = number;

	number = 0;
	std::cout << "Enter number of salt caves (recommended < " << recommended << "): ";
	while (number == 0) { own_cin(number); }
	Defines::n_salt() = number;

	number = 0;
	std::cout << "Enter number of food points (recommended < " << recommended << "): ";
	while (number == 0) { own_cin(number); }
	Defines::n_meal() = number;

	Application game(width, height, fps);

	game.ready_to_simulation();
	game.simulate();
	game.end_simulation();
}
