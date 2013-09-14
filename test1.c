#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <SDL/SDL.h>

typedef struct {
	SDL_Surface *surface;
	float x;
	float y;
	float z;
} cloud;

#define WIND_SPEED .1
#define NB_CLOUDS 10

void loop(SDL_Surface *screen, cloud* clouds);
SDL_Surface* createWindow();
cloud* createClouds();
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

	cloud* clouds = createClouds();

	// main loop
	loop(screen, clouds);

	SDL_Quit();
	return EXIT_SUCCESS;
}

void loop(SDL_Surface *screen, cloud* clouds)
{
	int c, continuer = 1;
	SDL_Event event;
	SDL_Rect position;

	while (continuer)
	{
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 71, 170, 253));

		for (c = 0; c < NB_CLOUDS; c++) {
			clouds[c].x += WIND_SPEED;
			position.x = clouds[c].x;
			position.y = clouds[c].y;
			SDL_BlitSurface(clouds[c].surface, NULL, screen, &position);
		}

		SDL_Flip(screen);

		//~SDL_WaitEvent(&event);
		//~switch(event.type)
		//~{
			//~case SDL_QUIT:
				//~continuer = 0;
		//~}
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

cloud* createClouds()
{
	int c;
	cloud *clouds;

	clouds = (cloud*) calloc(NB_CLOUDS, sizeof(cloud));

	for (c = 0; c < NB_CLOUDS; c++) {
		clouds[c] = createCloud();
	}

	return clouds;
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
	c.x = (float) get_random_int(0, 640);
	c.y = (float) get_random_int(0, 240);
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
