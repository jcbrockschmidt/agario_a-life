#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <cstdint>
#include <string>

namespace saveload {
	const uint8_t version = 2;
	const std::string filename = "save.dat";

	/**
	   Saves data from current simulation into a file. This data can later
	   be loaded for a simulation with saveload::saveData.

	   @return True when every goes fine. False when something goes wrong.
	 */
	bool saveData(void);

	/**
	   Loads saved data. May be used inplace of sim::init or sim::reset.

	   @return True when every goes fine. False when something goes wrong.
	 */
	bool loadData(void);
}

#endif
