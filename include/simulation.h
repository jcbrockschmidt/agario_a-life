#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "blobs.h"

namespace sim
{
	const double friction = 0.5;
	const double mutateProb = 0.20;
	extern CoordVect bounds;
	extern int initPopCnt;
	extern int initFoodCnt;
        extern std::vector<Blob> pop;
        extern std::vector<Food> food;
	extern int sumBlobs;
	extern double peakSize;

	/**
	   Initializes the simulation. Should only be called once.

	   @param bound_w Width of outer boundary.
	   @param bound_h Height of outer boundary.
	   @param popCnt Number of blobs in simulation.
	   @param foodCnt Number of food pellets in simulation.
	*/
	void init(double bound_w, double bound_h, int popCnt, int foodCnt);

	/**
	   Resets/re-initializes simulation.
	*/
	void reset(void);

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
	   Repopulate until population size is equal to initPopCnt.
	 */
	void repopulate(void);

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
	   Runs the simulation for 1 tick.
	*/
	void update(void);
}

#endif
