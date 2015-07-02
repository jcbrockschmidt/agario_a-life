#ifndef VISUALS_H
#define VISUALS_H

#include <string>
#include <SDL2/SDL.h>
#include "blobs.h"

namespace visuals
{
	extern SDL_Window *win;
	extern int win_w, win_h;
	extern SDL_Renderer *ren;
	extern SDL_Texture *bgTex;
	extern SDL_Rect bgRect;
	extern SDL_Rect blobRect;
	extern double transMult;
	extern Uint32 rmask, gmask, bmask, amask;

	namespace colors {
		extern Uint32 black;
	}

	int init(void);
	void quit(void);
	void draw(void);
}

#endif
