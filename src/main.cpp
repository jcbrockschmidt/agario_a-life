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

enum class saveLoadCase : std::int8_t {DO, DONOT};

int promptLoad(void);
int promptSave(void);

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if (promptLoad() == 1) {
		cout << endl << "Aborting..." << endl;
		return 1;
	}
	cout << endl;

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

	visuals::quit();

	cout << "Total blobs spawned:  " << sim::sumBlobs << endl
	     << "Largest size reached: " << sim::peakSize << endl
	     << endl;

	if (promptSave() == 1) {
		cout << endl << "Aborting..." << endl;
		return 1;
	}

	return 0;
}

int promptLoad(void)
{
	/* Prompt user for loading save data */
	saveLoadCase loadCase = saveLoadCase::DONOT;
	struct stat fileStat;
	if (stat(saveload::filename.c_str(), &fileStat) == 0) {
		char option;
		bool cont = true;
		while (cont) {
			cout << "Would you like to load the simulation data? "
			     << "(Y/n)" << endl;
			cin.get(option);
			if (option != '\n')
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (option == 'Y' || option == 'y' || option == '\n') {
				loadCase = saveLoadCase::DO;
				cont = false;
			} else if (option == 'N' || option == 'n') {
				cout << endl
				     << "Save data will not be loaded"
				     << endl
				     << "Understand that saving data at the "
				     << "end of this simulation will "
				     << "overwrite save data from the "
				     << "previously saved simulation"
				     << endl;
				loadCase = saveLoadCase::DONOT;
				cont = false;
			} else {
				cout << "Invalid option" << endl;
			}
		}
	}

	switch (loadCase) {
	case saveLoadCase::DO:
		cout << "Loading simulation..." << endl;
		if (!saveload::loadData()) {
			cout << endl
			     << "Could not load simulation data" << endl;
			loadCase = saveLoadCase::DONOT;
		} else {
			break;
		}
	case saveLoadCase::DONOT:
		cout << endl
		     << "Creating a new simulation..." << endl;
		sim::init(world_w, world_h, blobNum, foodNum);
		break;
	default:
		cout << "Invalid load case" << endl;
		return 1;
	}

	return 0;
}

int promptSave(void)
{
	/* Prompt user for saving simulation data */
	saveLoadCase saveCase = saveLoadCase::DONOT;
	char option;
	bool cont = true;
	while (cont) {
		cout << "Would you like to save the simulation data? "
		     << "(Y/n)" << endl;
		cout << "WARNING: This will overwrite any previously saved "
		     << "simulation data!" << endl;
		cin.get(option);
		if (option != '\n')
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (option == 'Y' || option == 'y' || option == '\n') {
			saveCase = saveLoadCase::DO;
			cont = false;
		} else if (option == 'N' || option == 'n') {
			saveCase = saveLoadCase::DONOT;
			cont = false;
		} else {
			cout << "Invalid option" << endl;
			cin.ignore();
		}
	}

	switch (saveCase) {
	case saveLoadCase::DO:
		cout << endl << "Saving simulation..." << endl;
		if (!saveload::saveData()) {
			cout << "Could not save simulation data" << endl;
		        saveCase = saveLoadCase::DONOT;
		} else {
			break;
		}
	case saveLoadCase::DONOT:
		cout << endl << "All simulation data will be lost" << endl;
		break;
	default:
		cout << "Invalid save case" << endl;
		return 1;
	}

	return 0;
}
