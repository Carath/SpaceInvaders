#include <stdio.h>

#include "user_inputs.h"


static SDL_Keycode last_pressed_key;
static int key_still_down = 0; // A besoin d'être mis à 0 la 1ère fois, mais pas lors d'un appel de 'init_game'.


// Handle user inputs.
void input_control()
{
	SDL_Event event;

	// Pour SDL_QUIT et text input:
	while (SDL_PollEvent(&event)) {} // 'while' in order to empty the event list (nails the mouse motion bug).
	// Nota Bene: SDL_PollEvent() implicitly calls SDL_PumpEvents().

	if (event.type == SDL_QUIT || Keyboard_state[SDL_GetScancodeFromKey(SDLK_ESCAPE)])
	{
		init_game(); // Pour libérer les listes.
		quit = 1; // On va arrêter l'exécution du programme.
		return;
	}


	else if (key_still_down) // Nothing happens if the last key is still down.
	{
		if (!Keyboard_state[SDL_GetScancodeFromKey(last_pressed_key)])
			key_still_down = 0;
	}


	else switch (current_menu)
	{
		case IN_GAME:

			if (Keyboard_state[SDL_GetScancodeFromKey(SDLK_LEFT)])
				move_spaceship(&ship, -1);

			if (Keyboard_state[SDL_GetScancodeFromKey(SDLK_RIGHT)])
				move_spaceship(&ship, 1);

			if (Keyboard_state[SDL_GetScancodeFromKey(SDLK_SPACE)] && time_step >= input_time)
			{
				player_shoot(&player_shot_list, &ship); // On ajoute un nouveau tir du joueur.
				input_time = time_step + INPUT_DELAY;
			}

			if (key_pressed(SDLK_KP_1))
			{
				game_state = GAME_PAUSED;
				current_menu = PAUSE_MENU;
			}

			break;


		case MAIN_MENU:

			if (key_pressed(SDLK_KP_1))
			{
				if (game_state == GAME_PAUSED)
				{
					current_menu = IN_GAME;
					game_state = GAME_ON;
				}

				else
					current_menu = DIFFICULTY_MENU;
			}

			else if (key_pressed(SDLK_KP_2))
			{
				current_menu = NAME_MENU;
				saving_state = LOADING_GAME;
			}

			else if (key_pressed(SDLK_KP_3))
				current_menu = COMMANDS_MENU;

			else if (key_pressed(SDLK_KP_4))
				current_menu = SCORES_MENU;

			break;


		case PAUSE_MENU:

			if (key_pressed(SDLK_KP_1))
			{
				current_menu = IN_GAME;
				game_state = GAME_ON;
			}

			else if (key_pressed(SDLK_KP_2))
				current_menu = MAIN_MENU;

			else if (key_pressed(SDLK_KP_3))
				current_menu = DIFFICULTY_MENU;

			else if (key_pressed(SDLK_KP_4))
			{
				current_menu = NAME_MENU;
				saving_state = SAVING_GAME;
			}

			else if (key_pressed(SDLK_KP_5))
				init_game(); // On libère les listes, et réinitialise le tout.

			break;


		case DIFFICULTY_MENU:

			if (key_pressed(SDLK_KP_1)) // Difficulty: easy
			{
				difficulty = EASY;
				current_alien = alien_easy;

				current_menu = IN_GAME;
				game_state = GAME_ON;
				update_balance();
			}

			else if (key_pressed(SDLK_KP_2)) // Difficulty: medium
			{
				difficulty = MEDIUM;
				current_alien = alien_medium;

				current_menu = IN_GAME;
				game_state = GAME_ON;
				update_balance();
			}

			else if (key_pressed(SDLK_KP_3)) // Difficulty: hard
			{
				difficulty = HARD;
				current_alien = alien_hard;

				current_menu = IN_GAME;
				game_state = GAME_ON;
				update_balance();
			}

			else if (key_pressed(SDLK_KP_4))
				current_menu = MAIN_MENU;

			break;


		case COMMANDS_MENU:

			if (key_pressed(SDLK_KP_1))
				current_menu = MAIN_MENU;

			break;


		case SCORES_MENU:

			if (key_pressed(SDLK_KP_1))
				current_menu = MAIN_MENU;

			break;


		case GAMEOVER_MENU:

			if (key_pressed(SDLK_KP_1))
				init_game(); // On libère les listes, et réinitialise le tout.

			break;


		case NAME_MENU:

			if (key_pressed(SDLK_RETURN)) // On appuie sur entrée.
			{
				if (Text_input == NULL || Text_input[0] == '\0') // Il faut rentrer un nom!
					redraw_menu = 0; // No need to redraw the menu, but redraw_menu already equals 1.

				else switch (saving_state)
				{
					case SAVING_SCORE: // Saving score, if in top 10.

						;char *filename = get_filename(SCORES_FOLDER, BEST_SCORES_FILENAME, ".txt"); // Trié dans l'ordre décroissant.

						save_scores(filename, Text_input);

						free(filename);

						init_game(); // On libère les listes, et réinitialise le tout.
						break;

					case SAVING_GAME:

						save_game_state(); // Peut être mettre Text_input en argument plutôt qu'externe pour ça?

						init_game(); // On libère les listes, et réinitialise le tout.
						break;

					case LOADING_GAME: // Get the state of an unfinished game from a text file.

						loading_succeed = load_data(); // Peut être mettre Text_input en argument plutôt qu'externe pour ça?

						if (loading_succeed) // lists have been freed before receiving their new content.
						{
							current_menu = IN_GAME;
							game_state = GAME_ON;
							saving_state = NOT_SAVING;

							score_steps_number = score / SCORE_TO_GAIN_LIFE;
							time_step = 0; // On remet les pendules à 0.

							update_balance();

							// Corrige un bug se produisant lorsqu'on charge une partie lors d'un clignotement:
							if (!invincibility)
								drawing_ship = 1;

							if (difficulty == EASY)
								current_alien = alien_easy;
							else if (difficulty == MEDIUM)
								current_alien = alien_medium;
							else // difficulty == HARD
								current_alien = alien_hard;
						}

						// else, back to typing.

						break;

					default:
						break;
				}

				SDLA_ResetTextInput(Text_input);
			}


			else if (key_pressed(SDLK_TAB)) // TAB: Perdre la saisie et retourner en arrière.
			{
				switch (saving_state)
				{
					case SAVING_SCORE: // On perd le score atteint, et on retourne au menu principal.

						init_game(); // On libère les listes, et réinitialise le tout.
						break;

					case SAVING_GAME: // On va reprendre la partie.

						current_menu = IN_GAME;
						game_state = GAME_ON;
						saving_state = NOT_SAVING;
						break;

					case LOADING_GAME: // On retourne au menu principal sans toucher à l'état de la partie (en cours ou non commencée).

						current_menu = MAIN_MENU;
						saving_state = NOT_SAVING;

						loading_succeed = 1; // by default, for the Name menu.
						break;

					default:
						break;
				}

				SDLA_ResetTextInput(Text_input);
			}


			else
				redraw_menu = SDLA_UpdateTextInput(Text_input, &event);

			break;


		default:
			break;
	}
}


// Returns 1 if the given key is actually pressed, and if so updates last_pressed_key, key_still_down and redraw_menu.
int key_pressed(SDL_Keycode key)
{
	if (Keyboard_state[SDL_GetScancodeFromKey(key)])
	{
		last_pressed_key = key;
		key_still_down = 1;
		redraw_menu = 1;
		return 1;
	}

	else
		return 0;
}
