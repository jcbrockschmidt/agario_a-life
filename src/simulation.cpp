#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "blobs.h"
#include "general.h"
#include "simulation.h"

using std::cout;
using std::endl;

void _spawn(void);

namespace sim
{
	CoordVect bounds;
	int initPopCnt;
	int initFoodCnt;
	std::vector<Blob> pop;
	std::vector<Food> food;
	int sumBlobs;
	double peakSize;
	unsigned int curLongestLife;

	void init(double bounds_w, double bounds_h, int popCnt, int foodCnt)
	{
		bounds = CoordVect(bounds_w, bounds_h);
		initPopCnt = popCnt;
		initFoodCnt = foodCnt;
		_spawn();
		curLongestLife = 0;
	}

	void reset(void)
	{
		pop.clear();
		food.clear();
		_spawn();
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
				throw std::out_of_range("sim::eat");
		} catch(const std::out_of_range &e) {
			cout << "Out of Range error: " << e.what() << endl;
			return false;
		}
		food.erase(food.begin()+f);
		return true;
	}

	void repopulate(void)
	{
		double totalFit = 0.0;
		for (std::vector<Blob>::iterator it = pop.begin();
		     it != pop.end(); ++it)
			totalFit += it->fitness;
		int oldPopSize = pop.size();
		double needFit, curFit, x, y;
		for (int i = initPopCnt-pop.size(); i > 0; --i) {
			needFit = getRandRange(0.0, totalFit);
			curFit = 0.0;
			int b;
			for (b = 0; b < oldPopSize-1; b++) {
				curFit += pop[b].fitness;
				if (curFit > needFit) break;
			}
			x = getRandRange(0.0, bounds.x-Blob::stdSize);
			y = getRandRange(0.0, bounds.y-Blob::stdSize);
			double weights[Brain::inNum][Brain::outNum];
			/* Copy weights */
			for (int i = 0; i < Brain::inNum; i++)
				for (int o = 0; o < Brain::outNum; o++)
					weights[i][o] = pop[b].brain.weights[i][o];
			if (getSRand() <= mutateProb) {
				/* Mutate a single weight */
				int i = getRand(Brain::inNum);
				int o = getRand(Brain::outNum);
				weights[i][o] += getRandRange(-5.0, 5.0);
			}
			pop.push_back(Blob(Blob::stdSize, x, y, weights));
			sumBlobs++;
		}
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
		/* Get current longest time alive */
		curLongestLife = 0;
		for (std::vector<Blob>::iterator it = pop.begin();
		     it != pop.end(); ++it)
			if (it->timeAlive > curLongestLife)
			        curLongestLife = it->timeAlive;
		/* Add 1 to current time alive since blob will
		   update its fitness value using curTimeAlive
		   after incrementing its own time alive */
		curLongestLife++;

		/* Update all blobs */
		for (int b; b < pop.size(); b++) {
			/* Decay blob */
			pop[b].size -= pop[b].size*Blob::decayRate;
			if (pop[b].size < Blob::minSize) {
				kill(b--);
			} else {
				if (pop[b].size > peakSize)
					peakSize = pop[b].size;
				/* Update blob if it is still alive */
				pop[b].update();
			}
		}

		/* Check for collisions between blobs and food */
		for (int b = 0; b < pop.size(); b++) {
			for (int f = 0; f < food.size(); f++) {
				if (!testAABBAABB(pop[b].pos, pop[b].size,
						  food[f].pos, food[f].size) )
					continue;

				if (sim::doesCover(pop[b].pos, pop[b].size,
						   food[f].pos, food[f].size)) {
					pop[b].addSize(
						sqrt(pop[b].size*pop[b].size +
						     food[f].size*food[f].size ) -
						pop[b].size );
					pop[b].boundsCorrect();
					eat(f--);
				}
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
					big.addSize(
						sqrt(big.size*big.size +
						     small.size*small.size ) -
						big.size );
					big.boundsCorrect();
					kill(small_i);
					/* Shift iterator values if needed */
					if (small_i == b1)
						b1--;
					b2--;
				}
			}
		}

		/* Add new blobs for ones that have died */
		if (pop.size() < initPopCnt)
			repopulate();

		/* Replenish food */
		for (int f = initFoodCnt-food.size(); f > 0; --f) {
			double x = getRandRange(0.0, bounds.x-Food::size);
			double y = getRandRange(0.0, bounds.y-Food::size);
			food.push_back(Food(x, y));
		}
	}
}

void _spawn(void)
{
	using namespace sim;
	double x, y;
	for (int b=0; b < initPopCnt; b++) {
		x = getRandRange(0.0, bounds.x-Blob::stdSize);
		y = getRandRange(0.0, bounds.y-Blob::stdSize);
		pop.push_back(Blob(Blob::stdSize, x, y));
	}
	sumBlobs = initPopCnt;
	peakSize = Blob::stdSize;
	for (int f=0; f < initFoodCnt; f++) {
		x = getRandRange(0.0, bounds.x-Food::size);
		y = getRandRange(0.0, bounds.y-Food::size);
		food.push_back(Food(x, y));
	}
}
