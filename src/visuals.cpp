#include <iostream>
#include "simulation.h"
#include "visuals.h"

using std::cout;
using std::endl;

namespace visuals
{
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;
	SDL_Texture *bgTex = NULL;
	SDL_Rect bgRect;
	CoordVect transMult = CoordVect();

	int init(void)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			cout << "SDL_Init Error: " << SDL_GetError() << endl;
			return 1;
		}

		win = SDL_CreateWindow("Test", 100, 100, 640, 480,
				       SDL_WINDOW_SHOWN );
		if (win == nullptr) {
			cout << "SDL_CreateWindow Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}

		ren = SDL_CreateRenderer(win, -1,
					 SDL_RENDERER_ACCELERATED |
					 SDL_RENDERER_PRESENTVSYNC );
		if (ren == nullptr) {
			cout << "SDL_CreateRenderer Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}

		int win_w, win_h;
		SDL_GetWindowSize(win, &win_w, &win_h);

		bgRect = {0, 0, win_w, win_h};
		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		SDL_Surface *bgSurf =
			SDL_CreateRGBSurface(0, win_w, win_h, 32,
			                     rmask, gmask, bmask, amask );
		if (bgSurf == NULL) {
			cout << "SDL_CreateRGBSurface Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}
		if (SDL_FillRect(bgSurf, &bgRect, 0xffffffff) != 0) {
			cout << "SDL_FillRect Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}
		Uint32 format = SDL_GetWindowPixelFormat(win);
		if (format == SDL_PIXELFORMAT_UNKNOWN) {
			cout << "SDL_GetWindowPixelFormat Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}
		/*
		bgTex = SDL_CreateTexture(ren, format,
					  SDL_TEXTUREACCESS_TARGET,
					  win_w, win_h );
		*/
		bgTex = SDL_CreateTextureFromSurface(ren, bgSurf);
		SDL_FreeSurface(bgSurf);
		if (bgTex == NULL) {
			cout << "SDL_CreateTexture Error: " <<
				SDL_GetError() << endl;
		        quit();
			return 1;
		}

		transMult.set((double)win_w, (double)win_h);
		transMult.div(sim::bounds.x, sim::bounds.y);
	}

	void quit(void)
	{
		if (!SDL_WasInit(SDL_INIT_VIDEO)) return;
		if (bgTex != NULL) SDL_DestroyTexture(bgTex);
		if (ren != NULL) SDL_DestroyRenderer(ren);
		if (win != NULL) SDL_DestroyWindow(win);
		SDL_Quit();
	}

	void draw(void)
	{
		SDL_RenderClear(ren);

		/* Draw background */
		SDL_RenderCopy(ren, bgTex, NULL, &bgRect);

		/* Draw blobs */

		/* Update screen */
		SDL_RenderPresent(ren);
	}
}
