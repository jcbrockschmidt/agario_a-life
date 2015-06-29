#ifndef VISUALS_H
#define VISUALS_H

#include <string>
#include <SDL2/SDL.h>

namespace visuals
{
	extern SDL_Window *win;
	extern SDL_Renderer *ren;
	extern std::string imgPath;
	extern SDL_Surface *bmp;
	extern SDL_Texture *tex;

	int init(void);
	void quit(void);
}

#endif
