#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "blobs.h"

namespace sim
{
	const int initPop = 1;
	const CoordVect bounds(1000.0, 1000.0);
        extern std::vector<Blob> pop;
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
	   Kills of a member of the population

	   @param b index of blob in population.
	 */
        bool kill(unsigned int b);

	/**
	   Runs the simulation for 1 tick.
	*/
	void update(void);
}

#endif
