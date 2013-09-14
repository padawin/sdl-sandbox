#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <SDL/SDL.h>

typedef struct {
	SDL_Surface *surface;
	SDL_Rect position;
	int z;
} cloud;

void loop();
SDL_Surface* createWindow();
void createSky(SDL_Surface* screen);
cloud createCloud();
unsigned int get_random_int(unsigned int min, unsigned int max);

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

	createSky(screen);

	// main loop
	loop();

	SDL_Quit();
	return EXIT_SUCCESS;
}

void loop()
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

void createSky(SDL_Surface* screen)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 71, 170, 253));

	int c, nbClouds;
	SDL_Rect cloudPosition;

	nbClouds = 10;
	cloud clouds[nbClouds];

	for (c = 0; c < nbClouds; c++) {
		clouds[c] = createCloud();
		SDL_BlitSurface(clouds[c].surface, NULL, screen, &clouds[c].position);
	}

	SDL_Flip(screen);
}

cloud createCloud()
{
	cloud c;
	int width, height, depth;
	SDL_Surface *surface;

	width = get_random_int(50, 100);
	height = get_random_int(50, 100);
	depth = get_random_int(0, 100);

	//calculate visual size and position from width, height, depth (Tales)

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 8, 255, 255, 255, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

	c.surface = surface;
	c.position.x = get_random_int(0, 640);
	c.position.y = get_random_int(0, 240);
	c.z = depth;

	return c;
}

/**
 * Returns a randomly generated int.
 * Uses /dev/urandom
 * @TODO Has to be improved to work on non unix system
 */
unsigned int get_random_int(unsigned int min, unsigned int max)
{
	int randomData = open("/dev/urandom", O_RDONLY);
	unsigned int rInt;
	read(randomData, &rInt, sizeof rInt);

	rInt = rInt % (max - min) + min;
	close(randomData);

	return rInt;
}
