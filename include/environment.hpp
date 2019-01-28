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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/** @class Environment
 *
 * @brief Abstract Environment class.
 *
 * An abstract class, to define common environment behavior. This will define
 * game rules and acceptable actions, as well as providing reward.
 *
 * @author Maxine Michalski
 */
class Environment {
	public:
		virtual ~Environment() {}
		/** @brief Reset environment to start conditions.
		 *
		 * This method will reset states and optionally change reward placement.
		 * It should be called if an end state (reward hit) is reached.
		 *
		 * @param[in] bool with_reward - A setter to indicate that reward
		 * placement (and thus path) should be changed too.
		 *
		 * @return new state in state encoding of an unsigned short
		 *
		 * @notice This method changes internal values, which can be read after.
		 */
		virtual unsigned short reset(bool with_reward) = 0;
		/** @brief Perform an action, in the current environment.
		 *
		 * This method will test, if the action is valid and then transition
		 * into a new state.
		 *
		 * @param[in] action Action bitmask, that is requested.
		 *
		 * @return new state in state encoding of an unsigned short
		 *
		 * @notice This method changes internal values, which can be read after.
		 * @notice If more than one action is required, via bitmask, then the
		 * least significant bit action is executed only.
		 */
		virtual unsigned short act(unsigned char action) = 0;
		/** @brief Bitmask encoded valid actions
		 *
		 * This method returns an 8 bit integer, that encodes valid actions (for
		 * current state) in it. Each mask position has a predefined meaning,
		 * where 1s are valid action positions and 0s are invalid ones.
		 *
		 * @notice valid positions with meanings:
		 * - 0x01 move up
		 * - 0x02 move right
		 * - 0x04 move down
		 * - 0x08 move left
		 *
		 * @return unsigned char integer with a bitmask pattern
		 */
		virtual unsigned char valid_actions() = 0;
		/** @brief Return current state information
		 *
		 * State information are encoded inside an unsigned short.
		 *
		 * @notice The x coordinate is in the left hand byte, while the y
		 * coordinate is in the right hand byte.
		 *
		 * @return unsigned short with positional state information.
		 */
		unsigned short state() { return (x<<8)|y; };
		/** @brief Reward position indicator
		 *
		 * This method returns the current reward position.
		 *
		 * @notice Encoding is the same as in state()
		 *
		 * @return unsigned short with positional information.
		 */
		unsigned short reward_position() { return (reward_x<<8)|reward_y; };
		/** @brief Current reward from last action/state pair */
		int reward() { return _reward; }
		/** @brief Playfield witdh in tiles */
		int width() { return _width; };
		/** @brief Playfield height in tiles */
		int height() { return _height; };
		/** @brief Return the entire map as data */
		std::vector<char> nodes() { return map; };
	protected:
		/** @brief Internal helper function, to set map node values
		 *
		 * @param[in] char x - X coordinate, to change value of
		 * @param[in] char y - Y coordinate, to change value of
		 * @param[in] char v - Value, used for change
		 *
		 * @notice This method only sets bit masks, ORing them with the current
		 * value, that is present already.
		 */
		void map_set(char x, char y, char v) { map[x * _height + y] |= v; };
		/** @brief Internal helper function, to get map node values
		 *
		 * @param[in] char x - X coordinate, to fetch value from
		 * @param[in] char y - Y coordinate, to fetch value from
		 *
		 * @return char value of requested node
		 */
		char map_get(char x, char y) { return map[x * _height + y]; };
		/** @brief internal variable to hold information about current reward */
		float _reward = 0;
		/** @brief internal variables for map with and height specifications */
		int _width, _height;
		/** @brief internal variables to hold position information */
		char x, y, reward_x, reward_y;
		/** @brief internal variable for all map data */
		std::vector<char> map;
};

#endif // ENVIRONMENT_H
