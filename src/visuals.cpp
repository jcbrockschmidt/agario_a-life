#include <algorithm>
#include <iostream>
#include "simulation.h"
#include "visuals.h"

using std::cout;
using std::endl;

namespace visuals
{
	SDL_Window *win = NULL;
	int win_w, win_h;
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

		SDL_GetWindowSize(win, &win_w, &win_h);

		bgRect = {0, 0, win_w, win_h};

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
		SDL_Surface *blobSurf =
			SDL_CreateRGBSurface(0, win_w, win_h, 32,
					     rmask, gmask, bmask, amask );
		if (blobSurf == NULL) {
			cout << "SDL_CreateRGBSurface Error: " <<
				SDL_GetError() << endl;
		} else {
			/* Draw each blob to the surface individually */
			SDL_Rect bRect;
			for (std::vector<Blob>::iterator it = sim::pop.begin();
			     it != sim::pop.end(); ++it) {
				bRect.x = (int)(it->pos.x * transMult.x);
				bRect.y = (int)(it->pos.y * transMult.y);
				bRect.w = std::max(1, (int)(it->size*transMult.x));
				bRect.h = std::max(1, (int)(it->size*transMult.y));
				SDL_FillRect(blobSurf, &bRect, 0xff000000);
			}
			SDL_Texture *blobTex =
				SDL_CreateTextureFromSurface(ren, blobSurf);
			SDL_FreeSurface(blobSurf);
			if (blobTex == NULL) {
				cout << "SDL_CreateTextureFromSurface Error: " <<
					SDL_GetError() << endl;
			} else {
				SDL_RenderCopy(ren, blobTex, NULL, &bgRect);
				SDL_DestroyTexture(blobTex);
			}
		}

		/* Update screen */
		SDL_RenderPresent(ren);
	}
}

/* This is here because it messes up Emacs's
   (my text editor) auto-indentation feature,
   which is extremely annoying.
*/
namespace visuals {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
#else
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
#endif
}
