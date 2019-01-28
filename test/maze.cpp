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

#include <criterion/criterion.h>

#include "environment/maze.hpp"
Environment *env = new Maze;

Test(Maze, reward) {
	cr_expect(env->reward() == 0, "Expected 'reward' to be zero on initialization");
}

Test(Maze, width) {
	cr_expect(env->width() == 16, "Expected 'width' to be fixed to 16");
}

Test(Maze, height) {
	cr_expect(env->height() == 10, "Expected 'height' to be fixed to 10");
}

Test(Maze, game_over) {
	cr_expect(env->game_over() == false, "Expected game to not be over after starting");
}
Test(Maze, valid_actions) {
	cr_expect(env->valid_actions() & 0x800f, "Expected valid actions to be returned");
	cr_expect(!(env->valid_actions() & 0x7ff0), "Expected valid actions to be returned");
}

Test(Maze, act) {
	cr_expect(env->act(0x8000) == true, "Expected 'call Alex' to be a valid action");
	cr_expect(env->state()[2] == 190, "Expected energy to be 190");
	cr_expect(env->act(0) == false, "Expected no action to be invalid");
	cr_expect(env->state()[2] == 190, "Expected energy to be 190");
	cr_expect(env->act(0x7ff0) == false, "Expected invalid actions to be invalid");
	cr_expect(env->state()[2] == 190, "Expected energy to be 190");
}

Test(Maze, state) {
	cr_expect(env->state()[0] == 0, "Expected x coordinate to be 0");
	cr_expect(env->state()[1] == 0, "Expected y coordinate to be 0");
	cr_expect(env->state()[2] == 200, "Expected energy to be 200");
}
