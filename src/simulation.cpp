#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stdlib.h>
#include "blobs.h"
#include "general.h"
#include "simulation.h"

using std::cout;

namespace sim
{
	std::vector<Blob> pop;
	double friction = 10.0;

	void init(void)
	{
		for (int b; b < initPop; b++) {
			double x = getRandRange(0.0, bounds.x-Blob::stdSize);
			double y = getRandRange(0.0, bounds.y-Blob::stdSize);
			pop.push_back(Blob(Blob::stdSize, x, y));
		}
	}

	void reset(void)
	{
		pop.clear();
		sim::init();
	}

	bool kill(int b)
	{
		try {
			if (b < 0 || b >= pop.size())
				throw std::out_of_range("sim::kill");
		} catch(const std::out_of_range &e) {
			cout << "Out of Range error: " << e.what() << '\n';
			return false;
		}
		pop.erase(pop.begin()+b);
		return true;
	}

	void update(void)
	{
		/* Update all blobs */
		for (int b; b < pop.size(); b++)
			pop[b].update();

		/* Check for collisions */
		for (int b1 = 0; b1 < pop.size()-1; b1++) {
			for (int b2 = b1+1; b2 < pop.size(); b2++) {
				if (b1 == b2)
					continue;
				if (!testAABBAABB(pop[b1].pos, pop[b1].size,
						  pop[b2].pos, pop[b2].size) )
					continue;

				bool b1Bigger;
				int small_i;
				if (pop[b1].size > pop[b2].size) {
					b1Bigger = true;
					small_i = b2;
				} else if (pop[b2].size > pop[b1].size) {
					b1Bigger = false;
					small_i = b1;
				} else { // Same size
					continue;
				}
				Blob &big = b1Bigger ? pop[b1] : pop[b2];
				Blob &small = b1Bigger ? pop[b2] : pop[b1];

				/* Make sure bigger blob is big enough
				   to consume other */
				if (small.size * Blob::howLrg > big.size)
					continue;

				/* Make sure bigger blob covers enough
				   of the smaller blob */
				double w, h;
				if (small.pos.x < big.pos.x)
					w = small.size-(big.pos.x-small.pos.x);
				else if (small.pos.x + small.size <= big.pos.x + big.size)
					w = small.size;
				else
					w = (big.pos.x+big.size)-small.pos.x;
				if (small.pos.y < big.pos.y)
					h = small.size-(big.pos.y-small.pos.y);
				else if (small.pos.y + small.size <= big.pos.y + big.size)
					h = small.size;
				else
					h = (big.pos.y+big.size)-small.pos.y;

				/* Covered area greater than or equal to
				   required percent of smaller blob's area
				   that must be covered
				 */
				double needArea = small.size*small.size *
					Blob::howCover;
				if (w*h >= needArea) {
					/* Consume smaller blob */
					kill(small_i);
					if (small_i == b1) {
						b1--;
						/* b1 will always be smaller
						   than b2 */
						if (small_i <= b2) b2--;
					}
				}
			}
		}
	}
}
