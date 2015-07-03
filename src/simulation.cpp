#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stdlib.h>
#include "blobs.h"
#include "general.h"
#include "simulation.h"

using std::cout;
using std::endl;

namespace sim
{
	std::vector<Blob> pop;
	std::vector<Food> food;
	double friction = 0.5;

	void init(void)
	{
		for (int b; b < initPopCnt; b++) {
			double x = getRandRange(0.0, bounds.x-Blob::stdSize);
			double y = getRandRange(0.0, bounds.y-Blob::stdSize);
			pop.push_back(Blob(Blob::stdSize, x, y));
			//DEBUG
			pop[b].vel.set(-20.0, -20.0);
			if (b%2 == 0)
				pop[b].size = 32.0;
			//EOF DEBUG
		}
		for (int f; f < initFoodCnt; f++) {
			double x = getRandRange(0.0, bounds.x-Food::size);
			double y = getRandRange(0.0, bounds.y-Food::size);
			food.push_back(Food(x, y));
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
			cout << "Out of Range error: " << e.what() << endl;
			return false;
		}
		pop.erase(pop.begin()+b);
		return true;
	}

	bool eat(unsigned int f)
	{
		try {
			if (f < 0 || f >= food.size())
				throw std::out_of_range("sim::kill");
		} catch(const std::out_of_range &e) {
			cout << "Out of Range error: " << e.what() << endl;
			return false;
		}
		food.erase(food.begin()+f);
		return true;
	}

	bool doesCover(CoordVect &big_pos, double big_size,
		       CoordVect &small_pos, double small_size)
	{
		/* Make sure bigger blob covers enough of the smaller blob */
		double w, h;
		if (small_pos.x < big_pos.x)
			w = small_size-(big_pos.x-small_pos.x);
		else if (small_pos.x + small_size <= big_pos.x + big_size)
			w = small_size;
		else
			w = (big_pos.x+big_size)-small_pos.x;
		if (small_pos.y < big_pos.y)
			h = small_size-(big_pos.y-small_pos.y);
		else if (small_pos.y + small_size <= big_pos.y + big_size)
			h = small_size;
		else
			h = (big_pos.y+big_size)-small_pos.y;

		/* Covered area greater than or equal to required percent of
		   smaller blob's area that must be covered
		*/
		double needArea = small_size*small_size * Blob::howCover;
		return (w*h >= needArea);
	}

	void update(void)
	{
		/* Update all blobs */
		for (int b; b < pop.size(); b++)
			pop[b].update();

		/* Check for collisions between blobs and food */
		for (int b = 0; b < pop.size(); b++) {
			for (int f = 0; f < food.size(); f++) {
				if (!testAABBAABB(pop[b].pos, pop[b].size,
						  food[f].pos, food[f].size) )
					continue;

				if (sim::doesCover(pop[b].pos, pop[b].size,
						   food[f].pos, food[f].size))
					eat(f--);
			}
		}

		/* Check for collisions between blobs */
		for (int b1 = 0; b1 < pop.size()-1; b1++) {
			for (int b2 = b1+1; b2 < pop.size(); b2++) {
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

				if (sim::doesCover(big.pos, big.size,
						   small.pos, small.size)) {
					/* Consume smaller blob */
					kill(small_i);
					/* Shift iterator values if needed */
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
