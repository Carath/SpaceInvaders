#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menus.h"


// Render the current menu, if redraw_menu = 1.
void render_menu(void)
{
	if (!redraw_menu || current_menu == IN_GAME)
		return;

	SDLA_ClearWindow(NULL);

	draw_menu();

	SDL_RenderPresent(renderer);

	redraw_menu = 0;
}


// Figure which menu is to be drawn.
void draw_menu(void)
{
	switch (current_menu)
	{
		case MAIN_MENU:

			wallpaper_menu();
			SDLA_SlowDrawText(font_big, &White, SDLA_CENTERED, 0.25*WINDOW_HEIGHT, "SPACE INVADERS");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-330)/2, WINDOW_HEIGHT/2 - 100, "1: Jouer / reprendre");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-330)/2, WINDOW_HEIGHT/2 - 25, "2: Charger une partie");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-330)/2, WINDOW_HEIGHT/2 + 50, "3: Voir les commandes");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-330)/2, WINDOW_HEIGHT/2 + 125, "4: Tableau des scores");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-330)/2, WINDOW_HEIGHT/2 + 225, "Echap: Quitter le jeu.");
			SDLA_SlowDrawText(font_small, &White, SDLA_CENTERED, WINDOW_HEIGHT/2 + 335, "Created by: D., J., M., O., S.");
			break;

		case PAUSE_MENU:

			SDLA_SlowDrawText(font_big, &White, SDLA_CENTERED, 0.15*WINDOW_HEIGHT, "PAUSE");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-450)/2, 0.3*WINDOW_HEIGHT, "1. Reprendre");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-450)/2, 0.4*WINDOW_HEIGHT, "2. Retour au menu, partie pausee.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-450)/2, 0.5*WINDOW_HEIGHT, "3. Changer la difficulte.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-450)/2, 0.6*WINDOW_HEIGHT, "4. Sauvegarder et quitter la partie.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-450)/2, 0.7*WINDOW_HEIGHT, "5. Quitter la partie sans sauvegarder.");
			SDLA_SlowDrawText(font_medium, &White, SDLA_CENTERED, 0.85*WINDOW_HEIGHT, "Echap: Quitter le jeu.");
			break;

		case DIFFICULTY_MENU:

			wallpaper_menu();
			SDLA_SlowDrawText(font_big, &White, SDLA_CENTERED, 0.3*WINDOW_HEIGHT, "Choisir une difficulte.");
			SDLA_SlowDrawText(font_medium, &White, SDLA_CENTERED, 0.4*WINDOW_HEIGHT - 20, "Une difficulte elevee rapporte plus de points.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-300)/2, 0.67*WINDOW_HEIGHT - 175, "1. Niveau facile");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-300)/2, 0.67*WINDOW_HEIGHT - 100, "2: Niveau moyen");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-300)/2, 0.67*WINDOW_HEIGHT - 25, "3: Niveau difficile");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-400)/2, 0.67*WINDOW_HEIGHT + 150, "4: Retour au menu principal.");
			SDLA_SlowDrawText(font_medium, &White, SDLA_CENTERED, 0.67*WINDOW_HEIGHT + 225, "Echap: Quitter le jeu.");
			break;

		case COMMANDS_MENU:

			wallpaper_menu();
			SDLA_SlowDrawText(font_big, &White, SDLA_CENTERED, 0.225 * WINDOW_HEIGHT, "Commandes:");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-350)/2, WINDOW_HEIGHT/2 - 100, "Deplacement gauche: <-");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-350)/2, WINDOW_HEIGHT/2 - 25, "Deplacement droite: ->");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-350)/2, WINDOW_HEIGHT/2 + 50, "Tirer: espace");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-350)/2, WINDOW_HEIGHT/2 + 125, "Pause: 1");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-400)/2, WINDOW_HEIGHT/2 + 275, "1: Retour au menu principal.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-400)/2, WINDOW_HEIGHT/2 + 350, "Echap: Quitter le jeu.");
			break;

		case SCORES_MENU:

			SDLA_SlowDrawText(font_big, &White, SDLA_CENTERED, WINDOW_HEIGHT/8, "Meilleurs scores:");
			char *filename = get_filename(SCORES_FOLDER, BEST_SCORES_FILENAME, ".txt");
			Result *tab = get_array(filename);
			char result[MAX_NAME_LENGTH + 5];
			for (int i = 0; i < NB_MAX_SCORES; ++i)
			{
				sprintf(result, "%d. %s", i+1, tab[i].Nom);
				int score_height = (0.275 + 0.05*i) * WINDOW_HEIGHT;
				SDLA_SlowDrawText(font_medium, &White, 0.2*WINDOW_WIDTH, score_height, result);
				sprintf(result, "%d", tab[i].Score);
				SDLA_SlowDrawText(font_medium, &White, 0.7*WINDOW_WIDTH, score_height, result);
			}
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-425) /2, 850, "1: Retour au menu principal.");
			SDLA_SlowDrawText(font_medium, &White, (WINDOW_WIDTH-425) /2, 900, "Echap: Quitter le jeu.");
			free(tab);
			free(filename);
			break;

		case GAMEOVER_MENU:

			SDLA_DrawTexture(game_over, SDLA_CENTERED, SDLA_CENTERED);
			int game_over_height;
			if (SDL_QueryTexture(game_over, NULL, NULL, NULL, &game_over_height) != 0)
				SDLA_ExitWithError("Impossible de trouver les dimensions de la texture.");

			int alien_coordy = fmax((WINDOW_HEIGHT - game_over_height)/2 - 200, 0);
			SDLA_DrawTexture(alien0, WINDOW_WIDTH/3, alien_coordy);

			char score_str[14];
			sprintf(score_str, "Score: %d", score);

			SDLA_SlowDrawText(font_medium, &White, SDLA_CENTERED, WINDOW_HEIGHT/2 + 225, score_str);
			SDLA_SlowDrawText(font_medium, &White, 75, 875, "1: Retour au menu principal.");
			SDLA_SlowDrawText(font_medium, &White, 75, 925, "Echap: Quitter");
			break;

		case NAME_MENU:

			if (saving_state == SAVING_SCORE)
			{
				char score_str[14];
				sprintf(score_str, "Score: %d", score);
				SDLA_SlowDrawText(font_medium, &White, SDLA_CENTERED, 0.25*WINDOW_HEIGHT, score_str);
				SDLA_SlowDrawText(font_medium, &White, 275, 0.35*WINDOW_HEIGHT, "Vous etes dans le top 10!");
				SDLA_SlowDrawText(font_medium, &White, 75, 875, "Tab: Perdre le score, et retourner au menu principal.");
			}
			else
				SDLA_SlowDrawText(font_medium, &White, 75, 875, "Tab: Retourner en arriere.");

			char to_print[35];
			sprintf(to_print, "Entrez un nom (%d caracteres max):", MAX_NAME_LENGTH);
			SDLA_SlowDrawText(font_medium, &White, 200, WINDOW_HEIGHT/2, to_print);
			int size = 0;
			if (Text_input != NULL)
			{
				SDLA_SlowDrawText(font_medium, &White, 200, WINDOW_HEIGHT/2 + 75, Text_input);
				size = SDLA_TextSize(font_medium, Text_input);
			}
			SDLA_SlowDrawText(font_medium, &White, 200 + size, WINDOW_HEIGHT/2 + 85, "_");
			SDLA_SlowDrawText(font_medium, &White, 75, 925, "Echap. Quitter");
			if (saving_state == LOADING_GAME && !loading_succeed)
				SDLA_SlowDrawText(font_medium, &White, 200, WINDOW_HEIGHT/2 + 150, "Partie non trouvee...");

			break;

		default:
			break;
	}
}


// Afficher le cadre du menu.
void wallpaper_menu(void)
{
	int alien_width, alien_height;
	if (SDL_QueryTexture(alien1, NULL, NULL, &alien_width, &alien_height) != 0)
		SDLA_ExitWithError("Impossible de trouver les dimensions de la texture.");

	SDLA_DrawTexture(alien1, 0, 0);
	SDLA_DrawTexture(alien2, 0, (WINDOW_HEIGHT - alien_height)/2);
	SDLA_DrawTexture(alien3, 0, WINDOW_HEIGHT - alien_height);
	SDLA_DrawTexture(alien4, WINDOW_WIDTH - alien_width, 0);
	SDLA_DrawTexture(alien5, WINDOW_WIDTH - alien_width, (WINDOW_HEIGHT - alien_height)/2);
	SDLA_DrawTexture(alien6, WINDOW_WIDTH - alien_width, WINDOW_HEIGHT - alien_height);
}
