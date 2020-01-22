#include <stdio.h>

#include "gameplay.h"


#define MAX_SCORE_LEN 20

static char score_string[MAX_SCORE_LEN];
static char hp_string[MAX_SCORE_LEN];
static char wave_string[MAX_SCORE_LEN];


void play(void)
{
	if (game_state != GAME_ON || current_menu != IN_GAME)
		return;


	// Affichage de l'image de fond.
	SDLA_DrawTexture(wallpaper, SDLA_CENTERED, SDLA_CENTERED);


	// Affichage du vaisseau du joueur:
	if (drawing_ship)
		SDLA_DrawTexture(space_ship, ship.X, ship.Y);


	// Affichage et déplacement des tirs du joueur:
	draw_move_shots(&player_shot_list, &White);


	// Affichage et déplacement des tirs des ennemis:
	draw_move_shots(&enemy_shot_list, &Red);


	// Affichage et déplacement des ennemis:
	draw_move_attack_enemies(&enemy_shot_list, &enemy_list, zigzag, current_alien);
	zigzag = (zigzag + 1) % (4 * actual_enemy_speed);


	// Check for collisions between shots. Addendum: place player_shot_list first:
	collision_datalist_shotlist(&player_shot_list, &enemy_shot_list, SHOT_WIDTH, SHOT_LENGTH, ENEMY_SHOT_REWARD, &score);


	// Check if enemies are hit. Addendum: place enemy_list first:
	collision_datalist_shotlist(&enemy_list, &player_shot_list, ENEMY_SIZE, ENEMY_SIZE, actual_kill_reward, &score);


	// Check if the player is hit:
	if (collision_data_shotlist(&ship, &enemy_shot_list, SHIP_SIZE, SHIP_SIZE)) // Separated from invincibility test: removes shots.
	{
		if (!invincibility)
		{
			// Dégats, et invincibilité temporaire:
			animation_time = time_step + ANIMATION_DURATION;
			invincibility = 1;
			--(ship.Param);
		}
	}


	// Clignotement du vaisseau:
	if (invincibility)
	{
		float ratio_time_remaining = (animation_time - time_step) / ANIMATION_DURATION;
		int animation_index = (2 * ANIMATION_BLINKS_NUMBER - 1) * ratio_time_remaining;

		drawing_ship = animation_index & 1; // ship blinking.

		if (time_step > animation_time) // i.e ratio_time_remaining < 0
		{
			invincibility = 0;
			drawing_ship = 1;
		}
	}


	// Conditions de Gameover:
	if (ship.Param <= 0 || end_reached(enemy_list))
	{
		game_state = GAME_OVER;

		if (in_high_score())
		{
			current_menu = NAME_MENU;
			saving_state = SAVING_SCORE;
		}
		else
			current_menu = GAMEOVER_MENU;

		redraw_menu = 1;
	}


	// Crée une nouvelle vague d'ennemis:
	if (enemy_list == NULL && bonus_spawned && bonus_gone)
	{
		++wave_number;
		update_balance();
		enemy_list = wave_attack();
		bonus_time = time_step + BONUS_MIN_DELAY + rand() % ((int) BONUS_RANGE_DELAY);
		bonus_spawned = 0;
		bonus_gone = 0;
	}


	// Apparition du bonus:
	if (!bonus_spawned && time_step > bonus_time)
	{
		bonus_spawned = 1;
		init_bonus(&bonus);
	}


	// Déplacement et collisions du bonus:
	if (bonus_spawned && !bonus_gone)
	{
		bonus_gone = move_bonus(&bonus); // bonus has moved.

		if (!bonus_gone) // bonus still here.
		{
			draw_unit(&bonus, bonus_model);

			if (collision_data_shotlist(&bonus, &player_shot_list, BONUS_SIZE, BONUS_SIZE))
			{
				bonus_gone = 1; // bonus is killed.
				score += BONUS_REWARD;
			}
		}
	}


	// Vie gagnée:
	if (score / SCORE_TO_GAIN_LIFE > score_steps_number)
	{
		++score_steps_number;
		++(ship.Param);
	}


	// Affichage du score:
	sprintf(score_string, "Score: %d", score);
	SDLA_SlowDrawText(font_medium, &Yellow, 30, 960, score_string);


	// Affichage du round:
	sprintf(wave_string, "Round: %d", wave_number);
	SDLA_SlowDrawText(font_medium, &Yellow, 440, 960, wave_string);


	// Affichage des points de vie du joueur:
	sprintf(hp_string, "HP: %d", ship.Param);
	SDLA_SlowDrawText(font_medium, &Yellow, 890, 960, hp_string);


	++time_step; // Number of passed game frames ~ time passed. Much better than clock(): stops when the game's paused.

	//------------------------------------------------------------
	// Rendering:

	SDL_RenderPresent(renderer); // On affiche le tout!
}
