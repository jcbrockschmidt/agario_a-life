#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "blobs.h"
#include "saveload.h"
#include "simulation.h"
#include "visuals.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if (!saveload::loadData()) {
		cout << "Could not load simulation data" << endl;
		sim::init(3000.0, 3000.0, 50, 500);
	}
	visuals::init();
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

	if (!saveload::saveData())
		cout << "Could not save simulation data" << endl;

	cout << "Total blobs spawned:  " << sim::sumBlobs << endl;
	cout << "Largest size reached: " << sim::peakSize << endl;

	visuals::quit();

	return 0;
}
