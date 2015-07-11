#include <cstdint>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "blobs.h"
#include "saveload.h"
#include "simulation.h"
#include "visuals.h"

using std::cout;
using std::cin;
using std::endl;

const double world_w = 3000.0;
const double world_h = 3000.0;
const double blobNum = 50;
const double foodNum = 500;

enum class initCase : std::int8_t {LOAD, NOLOAD};

int main(int argc, char *argv[])
{
	srand(time(NULL));

	/* Prompt user for loading save data */
	initCase initType = initCase::NOLOAD;
	struct stat fileStat;
	if (stat(saveload::filename.c_str(), &fileStat) == 0) {
		char option;
		while (1) {
			cout << "Would you like to load the simulation data? "
			     << "(Y/n)" << endl;
			cin.get(option);
			if (option == 'Y' || option == 'y' || option == '\n') {
				initType = initCase::LOAD;
				break;
			} else if (option == 'N' || option == 'n') {
				cout << "Save data will not be loaded" << endl;
				cout << "Understand that saving data at the "
				     << "end of this simulation will "
				     << "overwrite save data from the "
				     << "previously saved simulation"
				     << endl;
				initType = initCase::NOLOAD;
				break;
			} else {
				cout << "Invalid option" << endl;
			}
			cin.ignore(std::numeric_limits<std::streamsize>::max());
		}
	}

	switch (initType) {
	case initCase::LOAD:
		cout << "Loading simulation..." << endl;
		if (!saveload::loadData()) {
			cout << "Could not load simulation data" << endl;
			initType = initCase::NOLOAD;
		} else {
			break;
		}
	case initCase::NOLOAD:
		cout << "Creating a new simulation..." << endl;
		sim::init(world_w, world_h, blobNum, foodNum);
		break;
	default:
		cout << "Invalid initialization case" << endl;
		cout << "Aborting..." << endl;
		return 1;
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
