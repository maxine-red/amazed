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

#include <iostream>
#include "environment/maze.hpp"

Maze::Maze(int w, int h, char method) {
   	_width = w; _height = h; x = w/2; y = h/2;
	int i, n = w * h;
	// create map vector
	map.reserve(n);
	for (i = 0; i < n; i++) {
		map.push_back(0);
	}
	srand(time(NULL));
	switch (method) {
		case 'd': depth_first(x,  y); break;
		case 'k': kruskal(); break;
		case 'p': prim(); break;
	}
	// Take out all extra information, before continuing.
	for (i = 0; i < n; i++) {
		map[i] &= 0x0f;
	}
	reset(true);
}

unsigned short Maze::reset(bool with_reward) {
	if (with_reward) {
		reward_x = std::rand() % _width;
		reward_y = std::rand() % _height;
	}
	x = _width / 2;
	y = _height / 2;
	return (x<<8)|y;
}

unsigned short Maze::act(unsigned char action) {
	// test if action is valid
	unsigned char do_action = valid_actions() & action;
	switch (do_action) {
		case 0x0000: break; // No action picked. Invalid move!
		case 0x0001: y--; break; // move up one step
		case 0x0002: x++; break; // move right one step
		case 0x0004: y++; break; // move down one step
		case 0x0008: x--; break; // move left one step
		default: break; // all other actions are invalid
	}
	return (x<<8)|y;
}

unsigned char Maze::valid_actions() {
	return (map_get(x, y) & 0x0f);
}

void Maze::depth_first(int cx, int cy) {
	// set node visited
	int nx = cx, ny = cy, i, r;
	std::vector<char> dirs;
	map_set(cx, cy, 0x30); // set current value visited
	dirs.reserve(4);
	// test if nodes around can be visited
	for (i = 0; i < 4; i++) {
		dirs.push_back(i);
	}
	// improve randomization
	for (i = 0; i < 10; i++) {
		std::random_shuffle(dirs.begin(), dirs.end());
	}
	// walk over all possible node directions and see if we can progress on any
	while (!dirs.empty()) {
		nx = cx; ny = cy; // Reset values, so stuff works.
		r = dirs.back();
		dirs.pop_back(); // remove last value, after fetching it
		switch (r) {
			case 0: ny = cy - 1; break;
			case 1: nx = cx + 1; break;
			case 2: ny = cy + 1; break;
			case 3: nx = cx - 1; break;
		}
		if (ny >= 0 && ny < _height && nx >= 0 && nx < _width &&
			   	!(map_get(nx, ny) & 0x30)) {
			map_set(cx, cy, 1<<r);
			map_set(nx, ny, (0x30 | 1<<(r^2)));
			depth_first(nx, ny);
		}
	}
}

void Maze::kruskal() {
	int i, u, n = _width * _height, wall, wx, wy, vx, vy, cell;
	char dir;
	std::vector<int> cells, walls;
	cells.reserve(n);
	walls.reserve(n*4);
	for (i = 0; i < n*4; i++) {
		if (i % 4 == 0) { cells.push_back(i / 4); }
		walls.push_back(i);
	}
	std::random_shuffle(walls.begin(), walls.end());
	for (i = 0; i < n*4; i++) {
		wall = walls[i];
		wy = (wall / 4) % _height;
		wx = (wall / 4) / _height;
		vy = wy; vx = wx;
		dir = wall % 4;
		switch (dir) {
			case 0: vy = wy - 1; break;
			case 1: vx = wx + 1; break;
			case 2: vy = wy + 1; break;
			case 3: vx = wx - 1; break;
		}
		if (vy >= 0 && vy < _height && vx >= 0 && vx < _width) {
			if (cells[wx * _height + wy] != cells[vx * _height + vy]) {
				cell = cells[vx * _height + vy];
				map_set(wx, wy, 1<<dir);
				map_set(vx, vy, 1<<(dir^2));
				for (u = 0; u < n; u++) {
					if (cells[u] == cell) {
						cells[u] = cells[wx * _height + wy];
					}
				}
			}
		}
	}
}

void Maze::prim() {
	int i, wall, wx = x, wy = y, vx, vy;
	char dir;
	std::vector<int> walls;
	walls.reserve(_width * _height * 4);
	map_set(wx, wy, 0x10);
	for (i = 0; i < 4; i++) {
		walls.push_back((wx * _height + wy) * 4 + i);
	}
	do {
		std::random_shuffle(walls.begin(), walls.end());
		wall = walls.back();
		walls.pop_back(); // remove last value, after fetching it
		wy = (wall / 4) % _height;
		wx = (wall / 4) / _height;
		vy = wy; vx = wx;
		dir = wall % 4;
		switch (dir) {
			case 0: vy = wy - 1; break;
			case 1: vx = wx + 1; break;
			case 2: vy = wy + 1; break;
			case 3: vx = wx - 1; break;
		}
		if (vy >= 0 && vy < _height && vx >= 0 && vx < _width) {
			if (!(map_get(vx, vy)&0x10)) {
				map_set(wx, wy, 1<<dir);
				map_set(vx, vy, 1<<(dir^2));
				map_set(vx, vy, 0x10);
				for (i = 0; i < 4; i++) {
					walls.push_back((vx * _height + vy) * 4 + i);
				}
			}
		}
		
	} while (!walls.empty());
}
