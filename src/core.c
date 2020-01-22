#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"


// Initialisation:

// Initialize the player's ship position.
void init_ship(Data *ship)
{
	ship -> X = (WINDOW_WIDTH - SHIP_SIZE) /2;
	ship -> Y = SHIP_YPOS;
	ship -> Param = PLAYER_HP;
}


// Initialize a bonus.
void init_bonus(Data *bonus)
{
	int coming_from = 2 * (rand() & 1) - 1;

	if (coming_from == 1)
		bonus -> X = -BONUS_SIZE;
	else
		bonus -> X = WINDOW_WIDTH;

	bonus -> Y = BONUS_YPOS;
	bonus -> Param = coming_from;
}


// Create a wave of enemy.
NODE* wave_attack(void)
{
	int incrX = WINDOW_WIDTH / (NB_ENNEMY_COLS + 1);
	int posY = 0.1 * WINDOW_HEIGHT;
	int gap = ENEMY_SIZE + 10;
	NODE *enemy_list = NULL;

	for (int i = 1; i <= NB_ENNEMY_COLS; ++i)
	{
		for (int j = 0; j < NB_ENEMY_ROWS - 1; ++j)
			add_on_top(&enemy_list, i * incrX, posY + j * gap, 0);
		add_on_top(&enemy_list, i * incrX, posY + (NB_ENEMY_ROWS - 1) * gap, 1);
	}

	return enemy_list;
}


// Difficulty setting:

// Updates actual_enemy_speed, actual_shot_speed, actual_bonus_speed, and actual_kill_reward when difficulty or wave_number changes.
void update_balance(void)
{
	float actual_difficulty_factor = 0.5 * difficulty * (1 + DIFFICULTY_STEP * (wave_number - 1)); // Difficulty: 1, 2, or 3.
	actual_enemy_speed = (int) 1 + STARTING_ENEMY_SPEED / actual_difficulty_factor;
	actual_shot_speed = STARTING_SHOT_SPEED * actual_difficulty_factor;
	actual_bonus_speed = STARTING_BONUS_SPEED * actual_difficulty_factor;
	actual_kill_reward = 0.5 * difficulty * KILL_REWARD;
}


// Deplacement:

// Move the player's spaceship.
void move_spaceship(Data *ship, int direction) // direction: -1 -> left, 1 -> right.
{
	float next_pos = ship -> X + direction * SHIP_JUMP;
	if ( next_pos >= 0 && next_pos <= (WINDOW_WIDTH - SHIP_SIZE) )
		ship -> X = next_pos;
}


// Move the current bonus.
int move_bonus(Data *bonus)
{
	float next_pos = bonus -> X + bonus -> Param * actual_bonus_speed;
	if (next_pos >= -BONUS_SIZE && next_pos <= WINDOW_WIDTH)
	{
		bonus -> X = next_pos;
		return 0;
	}
	else return 1;
}


// Move an enemy, with zigzag as a parameter.
void move_enemy(Data *enemy, int zigzag, int alea) // zigzag modulo (4 * actual_enemy_speed).
{
	if (zigzag == 0)
		enemy -> X -= ENEMY_JUMP + alea;
	else if (zigzag == actual_enemy_speed)
		enemy -> Y += ENEMY_JUMP + alea;
	else if (zigzag == 2 * actual_enemy_speed)
		enemy -> X += ENEMY_JUMP + alea;
	else if (zigzag == 3 * actual_enemy_speed)
		enemy -> Y += ENEMY_JUMP + alea;
	// else, enemy are waiting.
}


// Move a shot.
void move_shot(Data *shot)
{
	shot -> Y -= actual_shot_speed * shot -> Param; // goes up if param = 1, and down if param = -1
}


// Drawing:

// Draws the given unit.
void draw_unit(Data *unit, SDL_Texture *unit_model)
{
	SDLA_DrawTexture(unit_model, unit -> X, unit -> Y);
}


// Draws and moves each enemy, and eventually make them attack. Doesn't change the order of enemy_list;
void draw_move_attack_enemies(NODE **enemy_shot_list, NODE **enemy_list, int zigzag, SDL_Texture *alien)
{
	int alea = rand() % ENEMY_JUMP;
	NODE *current = *enemy_list;

	while (current != NULL)
	{
		Data *enemy = &(current -> Val);
		draw_unit(enemy, alien);
		move_enemy(enemy, zigzag, alea);

		if (enemy -> Param == 1) // Enemy may attack...
		{
			int precision = 100 / ENEMY_ATTACK_PROBABILITY;
			float proba = (float) (rand() % precision) / precision;
			if (proba < ENEMY_ATTACK_PROBABILITY) // Enemy attacks!
				enemy_shoot(enemy_shot_list, enemy);
		}
		current = current -> Next;
	}
}


// Updates and draws the value of every shot in the list.
void draw_move_shots(NODE **shot_list, SDL_Color *color)
{
	if (SDL_SetRenderDrawColor(renderer, color -> r, color -> g, color -> b, color -> a) != 0)
		SDLA_ExitWithError("Impossible de changer la couleur pour le rendu.\n");

	SDL_Rect current_rect = {0, 0, SHOT_WIDTH, SHOT_LENGTH};

	NODE *current = *shot_list, *prev = NULL, *temp = NULL;

	while (current != NULL)
	{
		Data *shot = &(current -> Val);
		move_shot(shot);

		if (shot -> Y < 0 || shot -> Y > WINDOW_HEIGHT) // shot has to be removed.
		{
			if (prev == NULL)
				*shot_list = current -> Next;
			else
				prev -> Next = current -> Next;

			temp = current;
			current = current -> Next; // prev still where it was.
			free(temp);
		}

		else
		{
			current_rect.x = shot -> X;
			current_rect.y = shot -> Y;

			if (SDL_RenderFillRect(renderer, &current_rect) != 0)
				SDLA_ExitWithError("Impossible de dessiner un rectangle.\n");

			prev = current;
			current = current -> Next;
		}
	}
}


// Adding new data:

// Adds a data at the start of the list.
void add_on_top(NODE **list, float x, float y, int param)
{
	NODE *New = (NODE*) calloc(1, sizeof(NODE));
	(New -> Val).X = x;
	(New -> Val).Y = y;
	(New -> Val).Param = param;
	New -> Next = *list;
	*list = New;
}


// The player shoots.
void player_shoot(NODE **player_shot_list, Data *ship)
{
	add_on_top(player_shot_list, ship -> X + SHIP_SIZE/2, SHIP_YPOS, 1);
}


// This enemy shoots.
void enemy_shoot(NODE **enemy_shot_list, Data *enemy)
{
	add_on_top(enemy_shot_list, enemy -> X + ENEMY_SIZE/2, enemy -> Y + ENEMY_SIZE, -1);
}


// Collisions:

// Check if a given data and shot collide.
int collision_data_shot(Data *data, Data *shot, int data_width, int data_length)
{
	float xmin_d = data -> X, xmax_d = data -> X + data_width, ymin_d = data -> Y, ymax_d = data -> Y + data_length; // data box coordinates.
	float xmin_s = shot -> X, xmax_s = shot -> X + SHOT_WIDTH, ymin_s = shot -> Y, ymax_s = shot -> Y + SHOT_LENGTH; // shot box coordinates.

	return (xmax_d >= xmin_s && xmin_d <= xmax_s && ymax_d >= ymin_s && ymin_d <= ymax_s);
}


// Check if the given data is touching one of the shots. Doesn't change the order of shot_list.
int collision_data_shotlist(Data *data, NODE **shot_list, int data_width, int data_length)
{
	NODE *current = *shot_list, *prev = NULL;

	while (current != NULL && !collision_data_shot(data, &(current -> Val), data_width, data_length))
	{
		prev = current;
		current = current -> Next;
	}

	if (current == NULL)
		return 0;

	else // Its a hit! The shot has to be removed of shot_list.
	{
		if (prev == NULL) // if current is at the top of the list.
			*shot_list = current -> Next;
		else
			prev -> Next = current -> Next;

		free(current);
		return 1;
	}
}


// Check for collions between enemies (or shots) and shots. Doesn't change the order of data_list.
void collision_datalist_shotlist(NODE **data_list, NODE **shot_list, int data_width, int data_length, int reward, int *score)
{
	NODE *current = *data_list, *prev = NULL, *next = NULL, *temp = NULL;
	while (current != NULL)
	{
		next = current -> Next;

		if (collision_data_shotlist(&(current -> Val), shot_list, data_width, data_length))
		{
			// Its a hit! Therefore if data_list is enemy_list, the next enemy can shoot. But because of how the enemy wave is created,
			// and given the fact that enemy_list has its order constant, the next element after an enemy in enemy_list is either
			// an enemy on top of it, or is at the bottom of the wave. But we cannot always allow an enemy to attack if it is the successor
			// of a killed one, because this one may have been in the middle of a column. So we have to check if the antecedent of the killed
			// one has the same X coordinate. Finally, it doesn't change anything for shots, as long as the first list is player_shot_list.
			if ( ( prev == NULL || (prev -> Val).X != (current -> Val).X ) && next != NULL )
				(next -> Val).Param = 1; // exploit

			// The data has to be removed.
			if (prev == NULL) // if current is at the top of the list.
				*data_list = next;
			else
				prev -> Next = next;

			temp = current;
			current = next; // prev still where it was.
			free(temp);
			*score += reward;
		}

		else // checking the next data.
		{
			prev = current;
			current = next;		
		}
	}
}


// Check if one enemy goes too far to be killed.
int end_reached(NODE *enemy_list)
{
	int maxY = SHIP_YPOS - ENEMY_SIZE;
	NODE *current = enemy_list;

	while ( current != NULL && (current -> Val).Y <= maxY )
		current = current -> Next;

	return (current != NULL);
}


// Memory:

// Free a list from memory.
void free_list(NODE *list)
{
	// int i = 0;
	while (list != NULL)
	{
		NODE *temp = list;
		list = list -> Next;
		free(temp);
		// ++i;
	}
	// printf("%d Data has been freed.\n", i);
}


// Filename:

// Returns the filename associated with the given name. Will be used for scores, saves AND images.
char* get_filename(char *folder_name, char *name, char *file_type) // filetype example: ".txt"
{
	int n = strlen(folder_name) + strlen(name) + 4;
	char *adress = (char*) calloc(n, sizeof(char));
	sprintf(adress, "%s%s%s", folder_name, name, file_type);
	return adress;
}


// Time measurement:

double time_elapsed(clock_t start)
{
	return (double) (clock() - start) / CLOCKS_PER_SEC;
}
