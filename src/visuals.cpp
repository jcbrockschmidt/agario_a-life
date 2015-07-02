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
	SDL_Rect blobRect;
	double transMult;

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
		transMult = std::min((double)(win_w-padding*2)/sim::bounds.x,
				     (double)(win_h-padding*2)/sim::bounds.y );
		blobRect.w = (int)(sim::bounds.x*transMult);
		blobRect.h = (int)(sim::bounds.y*transMult);
		blobRect.x = (win_w-blobRect.w) / 2;
		blobRect.y = (win_h-blobRect.h) / 2;

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

		/* Draw borders */
		SDL_RenderDrawLine(ren,
				   blobRect.x,
				   blobRect.y,
				   blobRect.x,
				   blobRect.y+blobRect.h );
		SDL_RenderDrawLine(ren,
				   blobRect.x+blobRect.w,
				   blobRect.y,
				   blobRect.x+blobRect.w,
				   blobRect.y+blobRect.h);
		SDL_RenderDrawLine(ren,
				   blobRect.x,
				   blobRect.y,
				   blobRect.x+blobRect.w,
				   blobRect.y );
		SDL_RenderDrawLine(ren,
				   blobRect.x,
				   blobRect.y+blobRect.h,
				   blobRect.x+blobRect.w,
				   blobRect.y+blobRect.h );

		/* Draw blobs */
		SDL_Surface *blobSurf =
			SDL_CreateRGBSurface(0, blobRect.w, blobRect.h, 32,
					     rmask, gmask, bmask, amask );
		if (blobSurf == NULL) {
			cout << "SDL_CreateRGBSurface Error: " <<
				SDL_GetError() << endl;
		} else {
			/* Draw each blob to the surface individually */
			SDL_Rect bRect;
			for (std::vector<Blob>::iterator it = sim::pop.begin();
			     it != sim::pop.end(); ++it) {
				bRect.x = (int)(it->pos.x * transMult);
				bRect.y = (int)(it->pos.y * transMult);
				bRect.w = std::max(1, (int)(it->size*transMult));
				bRect.h = std::max(1, (int)(it->size*transMult));
				SDL_FillRect(blobSurf, &bRect, colors::black);
			}
			SDL_Texture *blobTex =
				SDL_CreateTextureFromSurface(ren, blobSurf);
			SDL_FreeSurface(blobSurf);
			if (blobTex == NULL) {
				cout << "SDL_CreateTextureFromSurface Error: " <<
					SDL_GetError() << endl;
			} else {
				SDL_RenderCopy(ren, blobTex, NULL, &blobRect);
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
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
namespace visuals {
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
	namespace colors {
		Uint32 black = 0x000000ff;
	}
}
#else
namespace visuals {
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
	namespace colors {
		Uint32 black = 0xff000000;
	}
}
#endif
