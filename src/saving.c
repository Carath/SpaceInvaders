#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saving.h"


// Saving results of a finished game:


// Returns a Result array from a text file.
Result* get_array(char *filename)
{
	FILE *F = fopen(filename, "r");

	if (F == NULL)
		;//printf("Erreur d'ouverture du fichier %s\n\n", filename);

	Result *tab = (Result*) calloc(NB_MAX_SCORES, sizeof(Result));

	if (tab == NULL)
		;//printf("\nImpossible dallouer.\n");

	else
	{
		char buffer[MAX_NAME_LENGTH];
		int i = 0;

		while (i < NB_MAX_SCORES && fgets(buffer, MAX_NAME_LENGTH, F) != NULL)
		{
			buffer[strcspn(buffer, "\n")] = '\0'; // on débarasse du retour chariot.
			strcpy(tab[i].Nom, buffer);

			if (fgets(buffer, MAX_NAME_LENGTH, F) == NULL)
				printf("Impossible de remplir le 'buffer'.\n\n");

			tab[i].Score = atoi(buffer); // conversion en entier
			++i;
		}
	}

	fclose(F);
	return tab;
}


// Check if the given score is in the top 10. Return 1 if so.
int in_high_score(void)
{
	char *filename = get_filename(SCORES_FOLDER, BEST_SCORES_FILENAME, ".txt");
	Result *tab = get_array(filename);
	int score_10 = tab[NB_MAX_SCORES-1].Score;
	free(tab);
	free(filename);
	return score > score_10;
}


// Sauvegarde les scores dans le fichier texte.
void save_scores(char *filename, char *pseudo)
{
	Result *tab = get_array(filename);
	int i = 0;

	while (i < NB_MAX_SCORES && tab[i].Score >= score)
		++i;

	if (i != NB_MAX_SCORES) // un meilleur score!
	{
		for (int j = NB_MAX_SCORES-1; j>=i; --j) // décalage des derniers.
		{
			strcpy(tab[j].Nom, tab[j-1].Nom);
			tab[j].Score = tab[j-1].Score;
		}
		strcpy(tab[i].Nom, pseudo); // et on place le nouveau vainqueur.
		tab[i].Score = score;
	}

	FILE *F = fopen(filename, "w");

	if (F == NULL)
		;//printf("Erreur d'ouverture du fichier %s\n\n", filename);

	else
	{
		for (int i = 0; i < NB_MAX_SCORES; ++i)
			fprintf(F, "%s\n%d\n", tab[i].Nom, tab[i].Score); // écriture dans le fichier.
		//printf("Sauvegarde des scores reussie.\n\n");
	}

	fclose(F);
	free(tab);
}

//------------------------------------------------------------
// Saving the state of a un unfinished game:


// Save a Data list in the text file.
void save_list(FILE *F, NODE *list)
{
	NODE *current = list;
	while (current != NULL)
	{
		Data *data = &(current -> Val);
		fprintf(F, "%.1f %.1f %d\n", data -> X, data -> Y, data -> Param);
		current = current -> Next;
	}
}


// Save the state of a game in a text file. Erases previous content.
void save_game_state(void)
{
	char *filename = get_filename(SAVES_FOLDER, Text_input, ".txt");
	FILE *F = fopen(filename, "w");

	if (F == NULL)
		;//printf("Erreur d'ouverture du fichier %s\n\n", filename);

	else
	{
		fprintf(F, "%d\n%.1f\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%.1f\n%d\n%d\n\n", score, ship.X, ship.Param, wave_number,
			difficulty, zigzag, input_time - time_step, invincibility, animation_time - time_step, bonus_spawned, bonus_gone,
			bonus.X, bonus.Param, bonus_time - time_step);

		save_list(F, enemy_list);
		fprintf(F, "#\n\n");
		save_list(F, enemy_shot_list);
		fprintf(F, "#\n\n");
		save_list(F, player_shot_list);
		fprintf(F, "#\n\n");

		// printf("Sauvegarde de la partie reussie.\n");
	}

	fclose(F);
	free(filename);
}

//------------------------------------------------------------
// Loading the state of a saved game:


// Get a Data list from a text file.
void load_list(FILE *F, NODE **list)
{
	NODE *current = NULL;
	char check;

	while (1)
	{
		Data data;

		if (fscanf(F, "%f %f %d", &(data.X), &(data.Y), &(data.Param)) != 3)
			; // Don't break here!

		if (fscanf(F, "%c", &check) != 1)
			;// printf("Caractere darret non trouve.\n\n");

		if (check == '#') // end of this list.
			break;
		else
			add_on_top(&current, data.X, data.Y, data.Param);
	}

	free_list(*list);
	*list = current;
}


// Get the state of an unfinished game from a text file. Returns 1 if the file is found, 0 otherwise.
int load_data(void)
{
	char *filename = get_filename(SAVES_FOLDER, Text_input, ".txt");
	FILE *F = fopen(filename, "r");

	if (F == NULL)
	{
		//printf("Erreur d'ouverture du fichier %s\n\n", filename);
		free(filename);
		return 0;
	}

	else
	{
		int loaded_difficulty;

		if (fscanf(F, "%d\n%f\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%f\n%d\n%d", &score, &(ship.X), &(ship.Param), &wave_number,
			&loaded_difficulty, &zigzag, &input_time, &invincibility, &animation_time, &bonus_spawned, &bonus_gone, &(bonus.X),
			&(bonus.Param), &bonus_time) != 14)
		{
			printf("Impossible de retrouver les données de la partie.\n\n");
			return 0;
		}

		difficulty = loaded_difficulty; // for type-checking compliance.

		load_list(F, &enemy_list);
		load_list(F, &enemy_shot_list);
		load_list(F, &player_shot_list);

		free(filename);
		fclose(F);
		// printf("Partie chargee.\n");
		return 1;
	}
}
