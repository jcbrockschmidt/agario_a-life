#ifndef VISUALS_H
#define VISUALS_H

#include <string>
#include <SDL2/SDL.h>
#include "blobs.h"

namespace visuals
{
	const int win_w = 1024;
	const int win_h = 640;
	const int padding = 10;
	extern SDL_Window *win;
	extern SDL_Renderer *ren;
	extern SDL_Texture *bgTex;
	extern SDL_Rect bgRect;
	extern SDL_Rect blobRect;
	extern double transMult;
	extern Uint32 rmask, gmask, bmask, amask;

	namespace colors {
		extern Uint32 black, red, blue;
	}

	int init(void);
	void quit(void);
	void draw(void);
}

#endif
