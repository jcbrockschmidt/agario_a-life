/* DOIT
 * - Format of save file does not accomadate for the fact that different
 *   platforms can have a different default number of bytes for each
 *   variable type.
 */

#include <fstream>
#include <iostream>
#include "blobs.h"
#include "saveload.h"
#include "simulation.h"

using std::cout;
using std::endl;

bool _loadData(void);

namespace saveload {
	bool saveData(void)
	{
		std::ofstream out(filename, std::ifstream::binary);
		if (!out) {
			cout << "Error opening '" << filename << "' for writing" << endl;
			return false;
		}

		/* Write file format version */
		out.write(reinterpret_cast<const char *>(&version), sizeof(uint8_t));

		/* Write simulation parameter data */
		out.write(reinterpret_cast<const char *>(&sim::bounds.x), sizeof(double));
		out.write(reinterpret_cast<const char *>(&sim::bounds.y), sizeof(double));
		out.write(reinterpret_cast<const char *>(&sim::initPopCnt), sizeof(int));
		out.write(reinterpret_cast<const char *>(&sim::initFoodCnt), sizeof(int));

		/* Write other data */
		out.write(reinterpret_cast<const char *>(&sim::sumBlobs), sizeof(int));
		out.write(reinterpret_cast<const char *>(&sim::peakSize), sizeof(double));
		int curPopCnt = sim::pop.size();
		out.write(reinterpret_cast<const char *>(&curPopCnt), sizeof(int));
	        int curFoodCnt = sim::food.size();
		out.write(reinterpret_cast<const char *>(&curFoodCnt), sizeof(int));

		/* Write blob data */
		for (std::vector<Blob>::iterator it=sim::pop.begin();
		     it != sim::pop.end(); ++it) {
			out.write(reinterpret_cast<const char *>(&it->size), sizeof(double));
			out.write(reinterpret_cast<const char *>(&it->pos.x), sizeof(double));
			out.write(reinterpret_cast<const char *>(&it->pos.y), sizeof(double));
			out.write(reinterpret_cast<const char *>(&it->vel.x), sizeof(double));
			out.write(reinterpret_cast<const char *>(&it->vel.y), sizeof(double));
			for (int i=0; i<Brain::inNum; i++)
				for (int o=0; o<Brain::outNum; o++)
					out.write(reinterpret_cast<const char *>(&it->brain.weights[i][o]),
						  sizeof(double));
		}

		/* Write food data */
		for (std::vector<Food>::iterator it=sim::food.begin();
		     it != sim::food.end(); ++it) {
			out.write(reinterpret_cast<const char *>(&it->pos.x), sizeof(double));
			out.write(reinterpret_cast<const char *>(&it->pos.y), sizeof(double));
		}

		out.close();
		return true;
	}

	bool loadData(void)
	{
		/* Use helper function to avoid excessive tabbing */
		return _loadData();
	}
}

bool _loadData(void)
{
	using namespace saveload;

	std::ifstream in(filename, std::ifstream::binary);
	if (!in) {
		cout << "Error opening '" << filename << "' for reading" << endl;
		cout << "The file might not exist" << endl;
		return false;
	}

	in.seekg(0, in.end);
	long fileSize = in.tellg();
	if (fileSize == 0) {
		cout << "No save data to load" << endl;
		in.close();
		return false;
	}

	in.seekg(0);
	in.exceptions(std::ifstream::eofbit);

	double b_w, b_h;
	int popCnt, foodCnt;
	int sumBlobs;
	double peakSize;
	std::vector<Blob> pop;
	std::vector<Food> food;
	try {
		/* Check file version */
		uint8_t fileVersion;
		in.read(reinterpret_cast<char *>(&fileVersion), sizeof(uint8_t));
		if (fileVersion != version) {
			if (fileVersion < version) {
				cout << "Save file format is outdated" << endl;
			} else {
				cout << "Save file format is unrecognized" << endl;
				cout << "You may need a more up-to-date " <<
					"version of the simulation engine" << endl;
			}
			return false;
		}

		/* Read simulation parameter data */
		in.read(reinterpret_cast<char *>(&b_w), sizeof(double));
		in.read(reinterpret_cast<char *>(&b_h), sizeof(double));
		in.read(reinterpret_cast<char *>(&popCnt), sizeof(int));
		in.read(reinterpret_cast<char *>(&foodCnt), sizeof(int));

		/* Read other data */
		int curPopCnt, curFoodCnt;
		in.read(reinterpret_cast<char *>(&sumBlobs), sizeof(int));
		in.read(reinterpret_cast<char *>(&peakSize), sizeof(double));
		in.read(reinterpret_cast<char *>(&curPopCnt), sizeof(int));
		in.read(reinterpret_cast<char *>(&curFoodCnt), sizeof(int));

		/* Read data for blobs */
		double size, x, y, vel_x, vel_y;
		double weights[Brain::inNum][Brain::outNum];
		pop.clear();
		for (int b=0; b<curPopCnt; b++) {
			in.read(reinterpret_cast<char *>(&size), sizeof(double));
			in.read(reinterpret_cast<char *>(&x), sizeof(double));
			in.read(reinterpret_cast<char *>(&y), sizeof(double));
			in.read(reinterpret_cast<char *>(&vel_x), sizeof(double));
			in.read(reinterpret_cast<char *>(&vel_y), sizeof(double));
			for (int i=0; i<Brain::inNum; i++)
				for (int o=0; o<Brain::outNum; o++)
					in.read(reinterpret_cast<char *>(&weights[i][o]),
						sizeof(double));
			pop.push_back(Blob(size, x, y, weights));
			pop[b].vel.set(vel_x, vel_y);
		}

		/* Read data for food */
		food.clear();
		for (int f=0; f<curFoodCnt; f++) {
			in.read(reinterpret_cast<char *>(&x), sizeof(double));
			in.read(reinterpret_cast<char *>(&y), sizeof(double));
			food.push_back(Food(x, y));
		}
	} catch (std::ifstream::failure e) {
		cout << "Error while parsing save file: " << e.what() << endl;
		cout << "Save file is corrupt" << endl;

		in.close();
		return false;
	}

	in.close();

	sim::bounds.x = b_w;
	sim::bounds.y = b_h;
	sim::initPopCnt = popCnt;
	sim::initFoodCnt = foodCnt;
	sim::sumBlobs = sumBlobs;
	sim::peakSize = peakSize;
	sim::pop.clear();
	sim::pop.swap(pop);
	sim::food.clear();
	sim::food.swap(food);

	return true;
}
