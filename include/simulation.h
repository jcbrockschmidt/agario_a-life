#ifndef SIMULATION_H
#define SIMULATION_H

#include "blobs.h"

namespace sim
{
	const int maxPop = 50;
	const CoordVect bounds(1000.0, 1000.0);
        extern Blob *pop;
	extern double friction;

	/**
	   Initializes the simulation.
	*/
	void init(void);

	/**
	   Cleans up leftover data from simulation.
	*/
	void cleanUp(void);

	/**
	   Runs the simulation for 1 tick.
	*/
	void update(void);
}

#endif
