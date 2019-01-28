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

#include "config.hpp"
#include "environment.hpp"

#ifndef BOARD_H
#define BOARD_H

/** @class Board
 *
 * @brief Abstract class for user interfaces.
 *
 * An abstract class, that defines general behavior of user interfaces and
 * allows for different interfaces to be used.
 *
 * @author Maxine Michalski
 */
class Board {
	public:
		// virtual Board(Environment *env) = 0;
		/** @brief Deconstructor for UIs
		 *
		 * This method makes sure, that decostruction of UIs is handled
		 * properly.
		 */
		virtual ~Board() {};
		/** @brief Test if system is capable of running this game.
		 *
		 * This method will run tests to see if the system can handle the
		 * requested user interface or not.
		 *
		 * @notice Make sure to clean up, after failure too.
		 *
		 * @notice This method sets `error_message` if a test fails.
		 *
		 * @return Boolean that shows if this system is capable or not
		 */
		virtual bool capable() = 0;
		/** @brief Setup UI
		 *
		 * A method that handles UI setup and accepts two parameters
		 * @param[in] unsigned int w - Width of board, in tiles
		 * @param[in] unsigned int h - Height of board, in tiles
		 * @param[in] vector<char> m - Height of board, in tiles
		 */
		virtual void setup(unsigned int w, unsigned int h, std::vector<char> m) = 0;
		/** @brief Update UI to show changes
		 *
		 * Updates the UI with current state changes.
		 *
		 */
		virtual void update() = 0;
		virtual char menu(std::vector<const char*> items) = 0;
		virtual void game_over() = 0;
		virtual char get_input() = 0;
		std::string error_message;
		char synth_help = ' ';
		unsigned char px, py, rx, ry;
		unsigned short  score;
		unsigned int seconds, steps;
		int energy, time_drain, step_drain;
	protected:
		unsigned int width, height;
		std::vector<char> map;
};

#endif // BOARD_H
