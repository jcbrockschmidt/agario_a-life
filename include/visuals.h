#ifndef VISUALS_H
#define VISUALS_H

#include <string>
#include <SDL2/SDL.h>

namespace visuals
{
	extern SDL_Window *win;
	extern SDL_Renderer *ren;

	int init(void);
	void quit(void);
}

#endif
