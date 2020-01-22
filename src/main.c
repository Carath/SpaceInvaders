#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDLA.h"
#include "settings.h"
#include "menus.h"
#include "user_inputs.h"
#include "gameplay.h"
#include "init.h"


// Fonts and textures:
TTF_Font *font_small;
TTF_Font *font_medium;
TTF_Font *font_big;

SDL_Texture *wallpaper;
SDL_Texture *game_over;
SDL_Texture *space_ship;
SDL_Texture *alien_easy;
SDL_Texture *alien_medium;
SDL_Texture *alien_hard;
SDL_Texture *current_alien;
SDL_Texture *bonus_model;
SDL_Texture *alien0;
SDL_Texture *alien1;
SDL_Texture *alien2;
SDL_Texture *alien3;
SDL_Texture *alien4;
SDL_Texture *alien5;
SDL_Texture *alien6;


extern Game game_state;

SDL_Window *window;
SDL_Renderer *renderer;

char *Text_input;

const Uint8 *Keyboard_state;

int quit = 0;


int main(void)
{
	srand(time(NULL)); // Initialisation du hasard.

	////////////////////////////////////////////////////////////
	// Initializing SDLA - rendering:

	SDLA_Init(&window, &renderer, "Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT,
		USE_HARDWARE_ACCELERATION, SDLA_BLENDED);

	// Initializing SDLA - text input:
	Text_input = SDLA_InitTextInput(MAX_NAME_LENGTH);

	////////////////////////////////////////////////////////////
	// Loading images:

	wallpaper = SDLA_CreateTexture("images/fond-etoile.png");
	game_over = SDLA_CreateTexture("images/game_over.png");
	space_ship = SDLA_CreateTexture("images/ship_model.png"); // 60: SHIP_SIZE
	alien_easy = SDLA_CreateTexture("images/alien_model1.png"); // 40: ENEMY_SIZE
	alien_medium = SDLA_CreateTexture("images/alien_model0.png");
	alien_hard = SDLA_CreateTexture("images/alien_model4.png");
	bonus_model = SDLA_CreateTexture("images/bonus.png"); // 40: BONUS_SIZE
	alien0 = SDLA_CreateTexture("images/alien0.png");
	alien1 = SDLA_CreateTexture("images/alien1.png");
	alien2 = SDLA_CreateTexture("images/alien2.png");
	alien3 = SDLA_CreateTexture("images/alien3.png");
	alien4 = SDLA_CreateTexture("images/alien4.png");
	alien5 = SDLA_CreateTexture("images/alien5.png");
	alien6 = SDLA_CreateTexture("images/alien6.png");

	////////////////////////////////////////////////////////////
	// Loading fonts:

	char font_name[] = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf";

	font_small = TTF_OpenFont(font_name, 22);
	font_medium = TTF_OpenFont(font_name, 27);
	font_big = TTF_OpenFont(font_name, 45);

	////////////////////////////////////////////////////////////
	// For gameplay input:

	Keyboard_state = SDL_GetKeyboardState(NULL);

	////////////////////////////////////////////////////////////
	// Sets global variables to their default state:

	init_game();

	////////////////////////////////////////////////////////////
	// Main loop:

	int global_time_step = 0;

	while (!quit)
	{
		// For time measurement:
		Uint32 lastTime = SDL_GetTicks();
		clock_t prev_time = clock();

		// Ingame:

		play();

		// Menus:

		render_menu();

		// Input control:

		input_control();

		// Benchmarking:

		++global_time_step;

		int time_passed = SDL_GetTicks() - lastTime; // Surtout pas en Uint32 ! Car si overflow, attend une éternité...

		if (BENCHMARKING && global_time_step % (1000 / FRAME_TIME) == 0)
		{
			double actual_time_passed = time_elapsed(prev_time); // To use before SDL_Delay!
			printf("Time_passed (in ms): %d\t\tactual_time_passed: %lf\n", time_passed, 1000. * actual_time_passed);
		}

		// Refresh rate control:

		SDL_Delay(fmax(0, FRAME_TIME - time_passed)); // In menus: 0% CPU usage.
	}

	////////////////////////////////////////////////////////////
	// Freeing and quitting:

	TTF_CloseFont(font_big);
	TTF_CloseFont(font_medium);
	TTF_CloseFont(font_small);

	SDL_DestroyTexture(alien6);
	SDL_DestroyTexture(alien5);
	SDL_DestroyTexture(alien4);
	SDL_DestroyTexture(alien3);
	SDL_DestroyTexture(alien2);
	SDL_DestroyTexture(alien1);
	SDL_DestroyTexture(alien0);
	SDL_DestroyTexture(bonus_model);
	SDL_DestroyTexture(alien_hard);
	SDL_DestroyTexture(alien_medium);
	SDL_DestroyTexture(alien_easy);
	SDL_DestroyTexture(space_ship);
	SDL_DestroyTexture(game_over);
	SDL_DestroyTexture(wallpaper);

	free(Text_input);

	SDLA_Quit();

	return EXIT_SUCCESS;
}
