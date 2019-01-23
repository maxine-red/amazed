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

#include <iostream>
using namespace std;

#include "environment/maze.hpp"

Maze::Maze() {
	srand(time(NULL));
	// Create a maze from a given starting point. This can even be changed to
	// any other valid node/tile on the playfield!
	process_node(x, y);
	nodes[std::rand() % _width][std::rand() % _height] |= 0x40;
}

bool Maze::act(unsigned short action) {
	unsigned short do_action = valid_actions() & action;
	bool valid = false, call_alex = false;
	switch (do_action) {
		case 0x0000: break; // No action picked. Invalid move!
		case 0x0001: y--; valid = true; break; // move up one step
		case 0x0002: x++; valid = true; break; // move right one step
		case 0x0004: y++; valid = true; break; // move down one step
		case 0x0008: x--; valid = true; break; // move left one step
		// Send out Alex to explore and better their help
		case 0x8000: call_alex = true; valid = true; break;
		default: break; // all other actions are invalid
	}
	if (valid && !call_alex) {
		energy -= drain;
		_reward = -0.1;
	}
	else if (valid && call_alex) {
		energy -= 10;
		_reward = 0;
	}
	if (energy < 0) {
		energy = 0;
	}
	if (nodes[x][y] & 0x40) {
		nodes[x][y] &= 0xbf;
		nodes[std::rand() % _width][std::rand() % _height] |= 0x40;
		energy += 200;
		drain++;
		_reward = 20;
	}
	else if (!energy) {
		_game_over = true;
	}
	return valid;
}

unsigned short Maze::valid_actions() {
	return (nodes[x][y] & 0x0f) | 0x8000;
}

std::vector<unsigned int> Maze::state() {
	std::vector<unsigned int> cstate;
	cstate.reserve(3);
	cstate.push_back(x);
	cstate.push_back(y);
	cstate.push_back(energy);
	return cstate;
}

void Maze::process_node(int cx, int cy) {
	// set node visited
	int nx = cx, ny = cy;
	nodes[cx][cy] |= 0x30;
	char *old_node = &nodes[cx][cy];
	int i, r;
	std::vector<char> dirs;
	dirs.reserve(4);
	// test if nodes around can be visited
	for (i = 0; i < 4; i++) {
		dirs.push_back(i);
	}
	for (i = 0; i < 10; i++) {
		std::random_shuffle(dirs.begin(), dirs.end());
	}
	while (!dirs.empty()) {
		nx = cx; ny = cy; // Reset values, so stuff works.
		r = dirs.back();
		dirs.pop_back();
		switch (r) {
			case 0: ny = cy - 1; break;
			case 1: nx = cx + 1; break;
			case 2: ny = cy + 1; break;
			case 3: nx = cx - 1; break;
		}
		if (ny >= 0 && ny < _height && nx >= 0 && nx < _width &&
			   	!(nodes[nx][ny] & 0x30)) {
			*old_node |= 1<<r;
			nodes[nx][ny] |= 0x30 | 1<<(r^2);
			process_node(nx, ny);
		}
	}
}
