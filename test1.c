#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <SDL/SDL.h>

typedef struct {
	SDL_Surface *surface;
	int width;
	int height;
	float x;
	float y;
	float z;
} w_element;

typedef struct {
	w_element element;
	float weight;
	float speed;
} w_weighted_element;


#define WIND_SPEED .1
#define NB_CLOUDS 10
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define SPEED 2
#define GRAVITY 1
#define DRAG 0.98
#define BOUNCE .9

void loop(SDL_Surface* screen, w_element ground, w_element* clouds, w_weighted_element player);
SDL_Surface* create_window(char* title);
w_element create_ground();
w_element* create_clouds();
w_element create_cloud();
w_weighted_element create_player();
void handle_player_gravity(w_weighted_element* player, w_element* ground);
unsigned int get_random_int(unsigned int min, unsigned int max);

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "An error occured during SDL initialization: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Surface* screen;
	int c;
	w_element ground;
	w_weighted_element player;
	w_element* clouds;
	screen = create_window("Title");

	if (screen == NULL) {
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	ground = create_ground();
	clouds = create_clouds();
	player = create_player();

	// main loop
	loop(screen, ground, clouds, player);

	for (c = 0; c < NB_CLOUDS; c++) {
		SDL_FreeSurface(clouds[c].surface);
	}
	free(clouds);
	SDL_FreeSurface(ground.surface);
	SDL_Quit();
	return EXIT_SUCCESS;
}

w_weighted_element create_player()
{
	w_weighted_element player;
	SDL_Surface *image;
	SDL_Rect srcPos;

	player.element.width = 32;
	player.element.height = 40;
	// area of the sprite to diplay in the player surface
	srcPos.x = 32;
	srcPos.y = 40;
	srcPos.w = player.element.width;
	srcPos.h = player.element.height;

	// Player sprite
	image = SDL_LoadBMP("resources/player.bmp");

	// Player surface
	player.element.surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 40, 32, 0, 0, 0, 0);
	// Blit in the player surface the subsection of the image represented by
	// srcPos
	SDL_BlitSurface(image, &srcPos, player.element.surface, NULL);
	// Make transparent every black pixels
	SDL_SetColorKey(player.element.surface, SDL_SRCCOLORKEY, SDL_MapRGB(player.element.surface->format, 0, 0, 0));

	// Set some player's informations
	player.element.x = WINDOW_WIDTH / 2;
	player.element.y = WINDOW_HEIGHT / 2;
	player.weight = 1;
	player.speed = SPEED;

	// Free the image surface
	SDL_FreeSurface(image);
	return player;
}

w_element create_ground()
{
	w_element ground;

	ground.x = 0;
	ground.y = WINDOW_HEIGHT - 50;
	ground.surface = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH, 50, 8, 44, 175, 0, 0);
	SDL_FillRect(ground.surface, NULL, SDL_MapRGB(ground.surface->format, 44, 175, 0));
	return ground;
}

void loop(SDL_Surface* screen, w_element ground, w_element* clouds, w_weighted_element player)
{
	int c, quit = 0;
	SDL_Event event;
	SDL_Rect position;

	while (!quit)
	{
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 71, 170, 253));
		position.x = ground.x;
		position.y = ground.y;
		SDL_BlitSurface(ground.surface, NULL, screen, &position);

		for (c = 0; c < NB_CLOUDS; c++) {
			clouds[c].x += WIND_SPEED * 5 / clouds[c].z;
			if (clouds[c].x > WINDOW_WIDTH) {
				clouds[c].x = 0 - clouds[c].width;
			}

			position.x = clouds[c].x;
			position.y = clouds[c].y;
			SDL_BlitSurface(clouds[c].surface, NULL, screen, &position);
		}

		handle_player_gravity(&player, &ground);

		position.x = player.element.x;
		position.y = player.element.y;
		SDL_BlitSurface(player.element.surface, NULL, screen, &position);

		SDL_Flip(screen);

		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
		}
	}
}

void handle_player_gravity(w_weighted_element* player, w_element* ground)
{
	(*player).element.y += (*player).speed;
	if ((*player).element.y + (*player).element.height > (*ground).y) {
		(*player).element.y = (*ground).y - (*player).element.height;
		(*player).speed = -(*player).speed * BOUNCE;
	}
	(*player).speed = (*player).speed * DRAG + GRAVITY;
}

SDL_Surface* create_window(char* title)
{
	SDL_Surface *screen;

	// window title
	SDL_WM_SetCaption(title, NULL);
	// window opening
	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE);

	if (screen == NULL) {
		fprintf(stderr, "An error occured during the creation of the window: %s\n", SDL_GetError());
		return NULL;
	}

	return screen;
}

w_element* create_clouds()
{
	int c;
	w_element *clouds;

	clouds = (w_element*) calloc(NB_CLOUDS, sizeof(w_element));

	for (c = 0; c < NB_CLOUDS; c++) {
		clouds[c] = create_cloud();
	}

	return clouds;
}

w_element create_cloud()
{
	w_element c;
	int width, height, depth;
	SDL_Surface *surface;

	width = get_random_int(50, 100);
	height = get_random_int(50, 100);
	depth = get_random_int(0, 100);

	//calculate visual size and position from width, height, depth (Tales)

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 8, 255, 255, 255, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

	c.surface = surface;
	c.width = width;
	c.height = height;
	c.x = (float) get_random_int(0, WINDOW_WIDTH);
	c.y = (float) get_random_int(0, WINDOW_HEIGHT / 2);
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
