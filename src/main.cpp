#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "blobs.h"
#include "simulation.h"
#include "visuals.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	visuals::init();

	sim::init();
	visuals::draw();
	SDL_Event ev;
	bool quit = false;
	while (true) {
		sim::update();
		visuals::draw();
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) quit = true;
		}
		if (quit) break;
	}

	cout << "Total blobs spawned:  " << sim::sumBlobs << endl;
	cout << "Largest size reached: " << sim::peakSize << endl;

	visuals::quit();

	return 0;
}
