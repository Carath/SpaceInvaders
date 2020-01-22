#ifndef SAVING_H
#define SAVING_H


#include "settings.h"
#include "core.h"


// Results of a finished game.
typedef struct
{
	char Nom[MAX_NAME_LENGTH];
	int Score;
	
} Result;


extern char *Text_input;

extern int score;
extern int wave_number;

extern int time_step;
extern int input_time;
extern int animation_time;
extern int bonus_time;

extern int bonus_spawned;
extern int bonus_gone;

extern int invincibility;
extern int zigzag;

extern NODE *enemy_list;
extern NODE *enemy_shot_list;
extern NODE *player_shot_list;

extern Data ship;
extern Data bonus;

extern Difficulty difficulty;


// Saving results of a finished game:


// Returns a Result array from a text file.
Result* get_array(char *filename);


// Check if the given score is in the top 10. Return 1 if so.
int in_high_score(void);


// Sauvegarde les scores dans le fichier texte.
void save_scores(char *filename, char *pseudo);


// Saving the state of a un unfinished game:


// Save a Data list in the text file.
void save_list(FILE *F, NODE *list);


// Save the state of a game in a text file. Erases previous content.
void save_game_state(void);


// Loading the state of a saved game:


// Get a Data list from a text file.
void load_list(FILE *F, NODE **list);


// Get the state of an unfinished game from a text file. Returns 1 if the file is found, 0 otherwise.
int load_data(void);


#endif
