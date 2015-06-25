#include <stdlib.h>
#include "blobs.h"
#include "general.h"
#include "simulation.h"

namespace sim
{
	Blob *pop = new Blob[maxPop];
	double friction = 10.0;

	void init(void)
	{
		for (int b; b < maxPop; b++) {
			/* Assign random coordinates to blobs */
			pop[b].pos.set(getRandRange(0.0, bounds.x-pop[b].size),
				       getRandRange(0.0, bounds.y-pop[b].size));
		}
	}

	void cleanUp(void)
	{
		delete pop;
	}

	void update(void)
	{
		/* Update all blobs */
		for (int b; b < maxPop; b++)
			pop[b].update();
	}
}
