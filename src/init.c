#include "init.h"


// Global variables:
SDL_Color White = {255, 255, 255, 255};
SDL_Color Yellow = {255, 255, 0, 255};
SDL_Color Red = {255, 0, 0, 255};

Menu current_menu;
Game game_state;
Saving saving_state;

int redraw_menu;

int loading_succeed;

int score;
int score_steps_number;
int wave_number;

int time_step;
int input_time;
int animation_time;
int bonus_time;

int bonus_spawned; // in order to generate the first wave.
int bonus_gone; // same.

int invincibility;
int drawing_ship;
int zigzag; // used for the enemy's movement.

NODE *enemy_list;
NODE *enemy_shot_list;
NODE *player_shot_list;

Data ship;
Data bonus;

Difficulty difficulty;
int actual_enemy_speed;
float actual_shot_speed;
float actual_bonus_speed;
int actual_kill_reward;


// Met toutes les variables globales à leur état d'origine:
void init_game(void)
{
	current_menu = MAIN_MENU;
	game_state = GAME_NOT_STARTED;
	saving_state = NOT_SAVING;

	redraw_menu = 1;

	loading_succeed = 1; // by default, for the Name menu.

	score = 0;
	score_steps_number = 0;
	wave_number = 0;

	time_step = 0; // On met les pendules à 0.
	input_time = 0;
	animation_time = 0;
	bonus_time = 0;

	bonus_spawned = 1; // In order to generate the first wave.
	bonus_gone = 1; // same.

	invincibility = 0;
	drawing_ship = 1;
	zigzag = 0;

	free_list(enemy_list); // Nécessaire lorsqu'on quitte une partie.
	free_list(enemy_shot_list); // same.
	free_list(player_shot_list); // same.

	enemy_list = NULL;
	enemy_shot_list = NULL;
	player_shot_list = NULL;

	init_ship(&ship);
	init_bonus(&bonus);
}
