/*
 *  Copyright 2019 Maxine Michalski <maxine@furfind.net>
 *
 *  This file is part of Amazed.
 *
 *  Amazed is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Amazed is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Amazed.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <csignal>
#include <cmath>

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include "config.hpp"
#include "board/curses.hpp"
#include "environment/maze.hpp"

using namespace std;

Board *board = nullptr;
Environment *env = nullptr;
bool run = true;
mutex mtx;
int energy = 300;
int time_drain = 1, step_drain = 1;
unsigned int seconds = 0, steps = 0;
unsigned short score = 0, pos = 0;

// These are all game rule definitions and necessary to properly run Amazed
#define MAX_ENERGY 1000
#define MIN_ENERGY 0
#define ENERGY_UP 300

#define DRAIN_INTERVAL 15
#define DRAIN_STEPS 100

/** @brief Helper cleanup function
 *
 * This function takes care of memory deallocation and exiting on signals.
 *
 * @param[in] int sig - Signal reveived (0 if no signal was received)
 */
void cleanup(int sig = 0) {
	if (board != nullptr) {
		delete board;
		board = nullptr;
	}
	if (env != nullptr) {
		delete env;
		env = nullptr;
	}
	if (sig) {
		exit(1);
	}
}

/** @brief Helper function to print 'help' information and credits */
void print_help() {
	cout << "Usage:" << endl
		<< "  " << PROGNAME << " [-d] [-k] [-p] [-w]" << endl
		<< "     -d	Randomized Depth-First search (corridor bias)" << endl
	   	<< "     -k	Randomized Kruskal's algorithm (dead end bias)" << endl
	   	<< "     -p	Randomized Prim's algorithm (dead end bias)" << endl
		<< endl
		<< "To play game, move the cursor with arrow keys." << endl
		<< "To quit game, press 'q'" << endl
		<< endl
		<< "This game is made possible by my Patreons:" << endl
		<< "arc" << endl
		<< "Pupper! ^-^ (Ulvra)" << endl
		<< "Jenny Koda" << endl
		<< endl
		<< "Please support me on my Patreon here: "
		<< "https://www.patreon.com/maxine_red" << endl
		<< "All Patreon supporters will be mentioned in this help!" << endl;
}

/** @brief Test if game over state is reached
 *
 * Game over states are reached by the general rule that energy levels reach 0
 *
 */
void test_game_over() {
	if (energy <= 0) { run = false; }
}

/** @brief Helper function to keep energy in a certain range
 *
 * To keep energy from becoming negative or getting too high, it is clipped
 * here.
 *
 */
void clip_energy() {
	if (energy > MAX_ENERGY) {
		energy = MAX_ENERGY;
	}
	else if (energy < MIN_ENERGY) {
		energy = MIN_ENERGY;
	}
	test_game_over();
}

/** @brief Timer update helper function
 *
 * This function's only purpose is to periodically increase the internal timer.
 * That timer influences energy drain.
 */
void timer_update() {
	// small hack to make UI more responsive, while also allowing somewhat time
	// accuracy
	unsigned int t = 0;
	while (run) {
		t++;
		if (t % 1000 == 0) {
			mtx.lock();
		   	seconds++;
			energy -= time_drain;
			clip_energy();
			mtx.unlock();
			if (seconds % DRAIN_INTERVAL == 0) {
				time_drain++;
			}
	   	}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

/** @brief Updater function for board values */
void update_values() {
	mtx.lock();
	board->energy = ceil(static_cast<float>(energy) / (MAX_ENERGY/20));
	board->time_drain = time_drain;
	board->step_drain = step_drain;
	board->seconds = seconds;
	board->steps = steps;
	board->score = score;
	board->px = (pos>>8)&0x00ff;
	board->py = pos&0x00ff;
	board->rx = (env->reward_position()>>8)&0x00ff;
	board->ry = env->reward_position()&0x00ff;
	mtx.unlock();
}

/** @brief Function for the independent running UI update thread */
void ui_update() {
	while (run) {
		update_values();
		board->update();
		std::this_thread::sleep_for(std::chrono::duration<double,
			   	ratio<1, 60>>(1));
	}
	update_values();
	board->update();
	board->game_over();
}

/** @brief Game main loop
 *
 * This function is where all game operations should be handled inside of.
 */
void game_loop() {
	char input;
	unsigned char action = 0;
	// Game main loop start
	std::thread ui_thread(ui_update);
	std::thread timer_thread(timer_update);
	while (run) {
		// main event loop
		input = board->get_input();
		switch (input) {
			case 'q': run = false; break;
			case 'u': action = 0x01; break;
			case 'r': action = 0x02; break;
			case 'd': action = 0x04; break;
			case 'l': action = 0x08; break;
			default: action = 0; break;
		}
		mtx.lock();
		if (env->valid_actions() & action) {
			pos = env->act(action);
			energy -= step_drain;
			steps++;
			if (steps % DRAIN_STEPS == 0) {
				step_drain++;
			}
			if (pos == env->reward_position()) {
				pos = env->reset(true);
				score++;
				energy += ENERGY_UP;
			}
			clip_energy();
		}
		mtx.unlock();
	}
	while (input != 'q') {
		input = board->get_input();
	}
	// Game main loop end and cleanup
	ui_thread.join();
	timer_thread.join();
}

/** @brief Main menu function
 *
 * Handles main menu, starting and other functionality.
 */
void main_menu() {
	board->menu({"Start Game", "Pick Maze", "Credits"});
	game_loop();
}

int main(int argc, char *argv[]) {
	int c;
	char maze = 'k';
	// register signals
	signal(SIGSEGV, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	// end of signal registration
	// check for command line parmeters
	while ((c = getopt(argc, argv, "hdkp")) != -1) {
		if (c == 'h') {
				print_help();
				exit(0);
		}
		else if (c != '?') {
			maze = c;
		}
		else {
		   	cerr << "Please refer to the help at ./" <<
				PROGNAME << " -h" << endl;
		   	exit(1);
		}
	}
	// end of command line parameter parsing
	if (env == nullptr) {
		env = new Maze(38, 9, maze);
		pos = env->state();
	}
	if (board == nullptr) {
		board = new CursesBoard();
	}
	if (!board->capable()) {
		cerr << board->error_message << endl;
		cleanup();
		exit(1);
	}
	board->setup(env->width(), env->height(), env->nodes());
	main_menu();
	cleanup();
	return 0;
}
