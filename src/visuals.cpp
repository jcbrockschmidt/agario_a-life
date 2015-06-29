#include <iostream>
#include "visuals.h"

using std::cout;
using std::endl;

namespace visuals
{
	SDL_Window *win = nullptr;
	SDL_Renderer *ren = nullptr;
	std::string imgPath = "test.bmp";
	SDL_Surface *bmp = nullptr;
	SDL_Texture *tex = nullptr;

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

		bmp = SDL_LoadBMP(imgPath.c_str());
		if (bmp == nullptr) {
			cout << "SDL_LoadBMP Error: " <<
				SDL_GetError() << endl;
			quit();
			return 1;
		}

		tex = SDL_CreateTextureFromSurface(ren, bmp);
		SDL_FreeSurface(bmp);
		if (tex == nullptr) {
			SDL_DestroyRenderer(ren);
			SDL_DestroyWindow(win);
			cout << "SDL_CreateTextureFromSurface Error: " <<
				SDL_GetError() << endl;
			SDL_Quit();
			return 1;
		}
	}

	void quit(void)
	{
		if (!SDL_WasInit(SDL_INIT_VIDEO)) return;
		if (tex != nullptr) SDL_DestroyTexture(tex);
		if (ren != nullptr) SDL_DestroyRenderer(ren);
		if (win != nullptr) SDL_DestroyWindow(win);
		SDL_Quit();
	}
}
