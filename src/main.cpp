/*
 *  Copyright 2019 Maxine Michalski <maxine@furfind.net>
 *
 *  This file is part of Alex.
 *
 *  Alex is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Alex is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Alex.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <iostream>
#include <csignal>

#include "board/curses.hpp"
#include "environment/maze.hpp"

using namespace std;

Board *board = nullptr;
Environment *env = nullptr;

void print_help() {
	cout << "grid world" << endl
		<< "Usage:" << endl
		<< "  " << PROGNAME << endl
		<< "To play game, move the cursor with arrow keys." << endl
		<< "To quit game, press 'q'" << endl;
	exit(1);
}

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


int main(int argc, char *argv[]) {
	// register signals
	signal(SIGSEGV, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	int c;
	bool run = true;
	char input;
	unsigned short action = 0;
	while ((c = getopt(argc, argv, "h s:")) != -1) {
		if (c == 'h') {
				print_help();
		}
		else if (c == '?') {
		   	cerr << "Please refer to the help at ./" <<
				PROGNAME << " -h" << endl; exit(1);
		}
		// TODO: ensure that other combinations of game mode, UI mode and
		// Synth helper are available
	}
	if (env == nullptr) {
		env = new Maze;
	}
	if (board == nullptr) {
		board = new CursesBoard(env);
	}
	if (!board->capable()) {
		cerr << board->error_message << endl;
		cleanup();
		return 1;
	}
	board->setup();
	// Game main loop start
	while (run) {
		board->update();
		input = board->get_input();
		switch (input) {
			case 'q': run = false; break;
			//case 'o': env->reset(); break; // start game again
			case 'u': action = 0x0001; break;
			case 'r': action = 0x0002; break;
			case 'd': action = 0x0004; break;
			case 'l': action = 0x0008; break;
			case 'a': action = 0x8000; break;
			default: action = 0; break;
		}
		if (env->act(action) && action & 0x8000 && !(action & 0x7fff)) {
			// Call synth for learning
		}
		if (env->game_over()) {
			run = false;
		}
	}
	board->update();
	while (input != 'q') {
		input = board->get_input();
	}
	// Game main loop end and cleanup
	cleanup();
	return 0;
	/* ununsed code, that needs to be moved into the Board class
	// Start of actual game code.
	while (true) {
		switch (getch()) {
			case 'q': delete board; exit(1); break;
			case KEY_UP:	board->move('u'); break;
			case KEY_DOWN:	board->move('d'); break;
			case KEY_LEFT:	board->move('l'); break;
			case KEY_RIGHT:	board->move('r'); break;
			default: break;
		}
	}*/
}
