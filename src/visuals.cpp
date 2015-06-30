#include <iostream>
#include "visuals.h"

using std::cout;
using std::endl;

namespace visuals
{
	SDL_Window *win = nullptr;
	SDL_Renderer *ren = nullptr;

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
	}

	void quit(void)
	{
		if (!SDL_WasInit(SDL_INIT_VIDEO)) return;
		if (ren != nullptr) SDL_DestroyRenderer(ren);
		if (win != nullptr) SDL_DestroyWindow(win);
		SDL_Quit();
	}
}
