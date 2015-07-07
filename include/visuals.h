#ifndef VISUALS_H
#define VISUALS_H

#include <string>
#include <SDL2/SDL.h>
#include "blobs.h"

namespace visuals
{
	const int win_w = 960;
	const int win_h = 960;
	const int padding = 10;
	const unsigned int blobColors_size = 12;
	extern SDL_Window *win;
	extern SDL_Renderer *ren;
	extern SDL_Texture *bgTex;
	extern SDL_Rect bgRect;
	extern SDL_Rect blobRect;
	extern double transMult;
	extern Uint32 rmask, gmask, bmask, amask;
	extern Uint32 blobColors[blobColors_size];

	namespace colors {
		extern Uint32 black, red, green, blue, redorange, orange,
			yelloworange, yellowgreen, bluegreen, blueviolet,
			violet, redviolet, cyan;
	}

	int init(void);
	void quit(void);
	Uint32 getRandBlobColor(void);
	void draw(void);
}

#endif
