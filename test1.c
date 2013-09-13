#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause();
SDL_Surface* createWindow();

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "An error occured during SDL initialization: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Surface *screen;
	screen = createWindow("Title");

	if (screen == NULL) {
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	// main loop
	pause();

	SDL_Quit();
	return EXIT_SUCCESS;
}

void pause()
{
	int continuer = 1;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
		}
	}
}

SDL_Surface* createWindow(char* title)
{
	SDL_Surface *screen;

	// window title
	SDL_WM_SetCaption(title, NULL);
	// window opening
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

	if (screen == NULL) {
		fprintf(stderr, "An error occured during the creation of the window: %s\n", SDL_GetError());
		return NULL;
	}

	return screen;
}
