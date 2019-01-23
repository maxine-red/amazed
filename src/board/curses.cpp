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

#include "board/curses.hpp"

using namespace std;

CursesBoard::CursesBoard(Environment *e) {
	initscr();
	env = e;
}

CursesBoard::~CursesBoard() {
	endwin();
}

bool CursesBoard::capable() {
	int x, y;
	getmaxyx(stdscr, y, x);
	if (y < 24 || x < 80) {
		error_message = "Terminal must be minimum of 80x24 characters!";
		return false;
	}
	if (!has_colors()) {
		error_message = "Terminal doesn't support colors";
		return false;
	}
	if (!can_change_color()) {
		error_message = "Terminal can't change color definitions";
		return false;
	}
	return true;
}

void CursesBoard::setup() {
	unsigned int width = env->width(), height = env->height();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	start_color();
	init_color(COLOR_WHITE, 309, 678, 1000);
	init_color(COLOR_MAGENTA, 870, 0, 549);
	init_color(COLOR_BLUE, 0, 462, 870);
	init_color(COLOR_BLACK, 35, 15, 223);
	init_pair(1, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	board_win = newwin(height*2+3,  width*2+3, 0, 0);
	stats_win = newwin(height*2+3,  80-(width*2+3), 0, width*2+3);
}


void CursesBoard::update() {
	draw_stats();
	draw_board();
	refresh();
	wrefresh(board_win);
	wrefresh(stats_win);
}

char CursesBoard::get_input() {
	char input = ' ';
	switch (getch()) {
		case KEY_UP:	input = 'u'; break;
		case KEY_RIGHT:	input = 'r'; break;
		case KEY_DOWN:	input = 'd'; break;
		case KEY_LEFT:	input = 'l'; break;
		case 'q':		input = 'q'; break;
	}
	return input;
}

void CursesBoard::draw_stats() {
	werase(stats_win);
	wborder(stats_win, ' ', 0, 0, 0, ACS_HLINE, 0, ACS_HLINE, 0);
	mvwprintw(stats_win, 3, 3, "Score: %d", score);
	mvwprintw(stats_win, 5, 3, "Energy: %d", env->state()[2]);
	if (env->game_over()) {
		wattron(stats_win, A_BOLD);
		mvwaddstr(stats_win, 9, 3, "Game Over!");
		wattroff(stats_win, A_BOLD);
	}
}

void CursesBoard::draw_board() {
	unsigned int n = env->width() * env->height(), i, h = env->height(), x, y;
	char *map = env->map();
	chtype wall =	ACS_CKBOARD | COLOR_PAIR(1);
	chtype player = ACS_CKBOARD | COLOR_PAIR(2);
	chtype floor =	' ' | COLOR_PAIR(3);
	werase(board_win);
	wborder(board_win, 0, 0, 0, 0, 0, ACS_TTEE, 0, ACS_BTEE);
	for (i = 0; i < n; i++) {
		y = ((i % h)+1)*2;
 		x =	((i / h)+1)*2;
		if (map[i] & 0x40) {
			mvwaddch(board_win, y, x, ACS_DIAMOND | COLOR_PAIR(3));
		}
		else {
			mvwaddch(board_win, y, x, floor);
		}
		mvwaddch(board_win, y-1, x-1, wall);
		if (map[i] & 0x01) { mvwaddch(board_win, y-1, x, floor); }
		else { mvwaddch(board_win, y-1, x, wall); }
		mvwaddch(board_win, y-1, x+1, wall);
		if (map[i] & 0x02) { mvwaddch(board_win, y, x+1, floor); }
		else { mvwaddch(board_win, y, x+1, wall); }
		mvwaddch(board_win, y+1, x+1, wall);
		if (map[i] & 0x04) { mvwaddch(board_win, y+1, x, floor); }
		else { mvwaddch(board_win, y+1, x, wall); }
		mvwaddch(board_win, y+1, x-1, wall);
		if (map[i] & 0x08) { mvwaddch(board_win, y, x-1, floor); }
		else { mvwaddch(board_win, y, x-1, wall); }
	}
	mvwaddch(board_win, (env->state()[1]+1)*2, (env->state()[0]+1)*2, player);
}
