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
		/* * @brief Reset environment to start conditions, but keep track.
		 *
		 * If a final state is hit, the `reset` method should be called, to
		 * indicate a reset of the Environment. It will reset all variables to
		 * their initial state.
		 *
		 * @notice This method changes internal values, which can be read after.
		 */
		//virtual void reset() = 0;
		/** @brief Perform an action, in the current environment.
		 *
		 * This method will test, if the action is valid and then transition
		 * into a new state.
		 *
		 * @param[in] action Action bitmask, that is requested.
		 *
		 * @return Boolean value, that indicates a state transition with `true`
		 * and is otherwise `false`.
		 *
		 * @notice This method changes internal values, which can be read after.
		 * @notice If more than one action is required, via bitmask, then the
		 * least significant bit action is executed only.
		 */
		virtual bool act(unsigned short action) = 0;
		/** @brief Current reward from last action/state pair */
		inline int reward() { return _reward; }
		/** @brief Playfield witdh in tiles */
		virtual unsigned int width() = 0;
		/** @brief Playfield height in tiles */
		virtual unsigned int height() = 0;
		/** @brief Bitmask encoded valid actions
		 *
		 * This method returns a 16 bit integer, that encodes valid actions (for
		 * current state) in it. Each mask position has a predefined meaning,
		 * where 1s are valid action positions and 0s are invalid ones.
		 *
		 * @return unsigned short integer with a bitmask pattern
		 */
		virtual unsigned short valid_actions() = 0;
		/** @brief Return current state information
		 *
		 * State information are encoded inside of an unsigned int vector.
		 *
		 * @return unsigned int vector with state information
		 */
		virtual std::vector<unsigned int> state() = 0;
		/** @brief indicator for game over state */
		bool game_over() { return _game_over; };
		/** @brief Tile representation of board area
		 *
		 * For UI to work properly, there needs to be a read-only way to read
		 * the entire map content.
		 *
		 * @returns a double pointer to map data (which is a two-dimensional
		 * array)
		 */
		virtual char *map() = 0;
	protected:
		float _reward = 0;
		bool _game_over = false;
};

#endif // ENVIRONMENT_H
