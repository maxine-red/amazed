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

#include <vector>
#include <iostream>
#ifdef WINDOWS
	#include <curses.h>
#else
	#include <ncurses.h>
#endif

#include "board.hpp"

#ifndef CURSESBOARD_H
#define CURSESBOARD_H
/** @class CursesBoard
 *
 * @brief Curses bases user interface.
 *
 * This class covers the curses based User Interface.
 */
class CursesBoard : public Board {
	public:
		/** @brief Curses initializer, that calls initscr() internally. */
		CursesBoard();
		/** @brief Desconstructor, that calls endwin() internally */
		~CursesBoard();
		/** @see Board for more information on any method */
		bool capable();
		void setup();
		void setup(unsigned int w, unsigned int h, std::vector<char> m);
		void update();
		void menu(std::vector<const char*> items, unsigned char active);
		void menu(std::vector<const char*> items, unsigned char active, std::vector<unsigned char> set);
		void patrons(std::vector<const char*> names);
		void game_over();
		char get_input();
	protected:
		void copy_notice() {
			mvprintw(LINES-2, 0, "%s v%d.%d.%d (c) %s %s (%s)", PROGCANNAME,
				   	MAJOR, MINOR, PATCH, COPYRIGHT, AUTHOR, LICENSE);
			mvaddstr(LINES-1, 0, "Key bindings: arrow keys - movement, q - exit (to main menu), enter - pick item");
	   	};
	private:
		/** @brief Method to handle statistics drawing */
		void draw_stats();
		/** @brief Method to handle game field drawing */
		void draw_board();
		WINDOW *board_win;
		WINDOW *stats_win;
};

#endif // CURSESBOARD_H
