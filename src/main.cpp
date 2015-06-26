#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "blobs.h"
#include "simulation.h"

using std::cout;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	sim::init();
	for (int i; i < 10000; i++) {
		sim::update();
	}

	return 0;
}
