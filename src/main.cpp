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
	for (int i; i < 10000; i++) {
		sim::update();
		visuals::draw();
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) quit = true;
		}
		if (quit) break;
		SDL_Delay(100);
	}

	visuals::quit();

	return 0;
}
