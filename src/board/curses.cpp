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

CursesBoard::CursesBoard() {
	initscr();
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

void CursesBoard::setup(unsigned int w, unsigned int h, std::vector<char> m) {
	width = w; height = h; map = m;
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	start_color();
	// Initialize colors neede to render board/stats
	init_color(COLOR_WHITE, 309, 678, 1000);
	init_color(COLOR_MAGENTA, 870, 0, 549);
	init_color(COLOR_BLUE, 0, 462, 870);
	init_color(COLOR_BLACK, 35, 15, 223);
	init_pair(1, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	board_win = newwin(height*2+3,  width*2+3, 0, 0);
	stats_win = newwin(24-(height*2+3),  (width*2+3), height*2+3, 0);
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
	int i;
	werase(stats_win);
	wborder(stats_win, 0, 0, ' ', 0, ACS_VLINE, ACS_VLINE, 0, 0);
	for (i = 0; i < getmaxy(stats_win)-1; i++) {
		mvwaddch(stats_win, i, 19, ACS_VLINE);
		mvwaddch(stats_win, i, 42, ACS_VLINE);
		mvwaddch(stats_win, i, 65, ACS_VLINE);
	}
	mvwaddch(stats_win, getmaxy(stats_win)-1, 19, ACS_BTEE);
	mvwaddch(stats_win, getmaxy(stats_win)-1, 42, ACS_BTEE);
	mvwaddch(stats_win, getmaxy(stats_win)-1, 65, ACS_BTEE);
	mvwprintw(stats_win, 0, 1, "Score:  %d", score);
	mvwaddstr(stats_win, 1, 1, "Energy: ");
	for (i = 0; i < energy/2; i++) {
		mvwaddch(stats_win, 1, 9+i, ACS_CKBOARD|COLOR_PAIR(4));
	}
	if (energy % 2) {
		mvwaddch(stats_win, 1, 9+i++, ACS_CKBOARD|COLOR_PAIR(3));
	}
	mvwprintw(stats_win, 0, 20, "Time:  %02d:%02d", (seconds / 60) % 60,
		   	seconds % 60);
	mvwaddstr(stats_win, 1, 20, "Drain: ");
	if (time_drain > 30) { time_drain = 30; }
	for (i = 0; i < time_drain / 2; i++) {
		mvwaddch(stats_win, 1, 27+i, ACS_CKBOARD|COLOR_PAIR(4));
	}
	if (time_drain % 2) {
		mvwaddch(stats_win, 1, 27+i++, ACS_CKBOARD|COLOR_PAIR(3));
	}
	mvwprintw(stats_win, 0, 43, "Steps: %d", steps);
	mvwaddstr(stats_win, 1, 43, "Drain: ");
	if (step_drain > 30) { step_drain = 30; }
	for (i = 0; i < step_drain / 2; i++) {
		mvwaddch(stats_win, 1, 50+i, ACS_CKBOARD|COLOR_PAIR(4));
	}
	if (step_drain % 2) {
		mvwaddch(stats_win, 1, 50+i++, ACS_CKBOARD|COLOR_PAIR(3));
	}
	mvwaddstr(stats_win, 0, 67, "Alex: ");
	mvwaddch(stats_win, 0, 73, synth_help);
}

void CursesBoard::game_over() {
	wattron(stats_win, A_BOLD);
	mvwaddstr(stats_win, 0, 67, "Game Over!");
	wattroff(stats_win, A_BOLD);
	refresh();
	wrefresh(board_win);
	wrefresh(stats_win);
}

void CursesBoard::draw_board() {
	unsigned int n = width * height, i, x, y;
	chtype wall =	ACS_CKBOARD | COLOR_PAIR(1);
	chtype player = ACS_CKBOARD | COLOR_PAIR(2);
	chtype floor =	' ' | COLOR_PAIR(3);
	werase(board_win);
	wborder(board_win, 0, 0, 0, 0, 0, 0, ACS_LTEE, ACS_RTEE);
	for (i = 0; i < n; i++) {
		y = ((i % height)+1)*2;
		x =	((i / height)+1)*2;
		mvwaddch(board_win, y, x, floor);
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
	mvwaddch(board_win, ((ry)+1)*2, (rx+1)*2, ACS_DIAMOND | COLOR_PAIR(3));
	mvwaddch(board_win, ((py)+1)*2, ((px)+1)*2, player);
	mvwaddch(board_win, getmaxy(board_win)-1, 19, ACS_TTEE);
	mvwaddch(board_win, getmaxy(board_win)-1, 42, ACS_TTEE);
	mvwaddch(board_win, getmaxy(board_win)-1, 65, ACS_TTEE);
}
