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

#include <vector>
#include <iostream>
#include <ncurses.h>

#include "board.hpp"

#ifndef CURSESBOARD_H
#define CURSESBOARD_H

class CursesBoard : public Board {
	public:
		CursesBoard(Environment *e);
		~CursesBoard();
		bool capable();
		void setup();
		void update();
		char get_input();
	private:
		void draw_stats();
		void draw_board();
		WINDOW *board_win;
		WINDOW *stats_win;
};

#endif // CURSESBOARD_H
