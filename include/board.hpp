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
		virtual void setup() = 0;
		virtual void update() = 0;
		virtual char get_input() = 0;
		std::string error_message;
		int score = 0;
	protected:
		Environment *env;
};

#endif // BOARD_H
