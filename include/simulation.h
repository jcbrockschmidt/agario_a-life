#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "blobs.h"

namespace sim
{
	const int initPopCnt = 100;
	const int initFoodCnt = 100;
	const CoordVect bounds(1000.0, 1000.0);
        extern std::vector<Blob> pop;
        extern std::vector<Food> food;
	extern double friction;

	/**
	   Initializes the simulation. Should only be called once.
	*/
	void init(void);

	/**
	   Resets/re-initializes simulation.
	*/
	void reset(void);

	/**
	   Determines if a bigger square sufficiently covers a smaller square.

	   @param big_pos Position of bigger square.
	   @param big_size Size of each of bigger square's sides
	   @param small_pos Position of smaller square.
	   @param small_size Size of each of smaller square's sides
	   @return True if big square sufficient covers smaller square.
	           False if it does not.
	 */
	bool doesCover(CoordVect &big_pos, double big_size,
		       CoordVect &small_pos, double small_size);

	/**
	   Kills of a member of the population.

	   @param b Index of blob in population.
	 */
        bool kill(unsigned int b);

	/**
	   Removes a food pellet.

	   @param f Index of food pellet.
	 */
        bool eat(unsigned int f);

	/**
	   Runs the simulation for 1 tick.
	*/
	void update(void);
}

#endif
