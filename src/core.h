#ifndef CORE_H
#define CORE_H


#include <time.h>

#include "SDLA.h"
#include "settings.h"


typedef enum {EASY = 1, MEDIUM = 2, HARD = 3} Difficulty;


extern SDL_Renderer *renderer;

extern int wave_number;
extern Difficulty difficulty;
extern int actual_enemy_speed;
extern float actual_shot_speed;
extern float actual_bonus_speed;
extern int actual_kill_reward;


// Data structure: will be either the player's spaceship, an enemy, a shot, or a bonus.
// Param for the player: Hit Points.
// Param for an enemy: 1 -> the enemy can attack, else 0.
// Param for a shot: 1 -> shot going to the top, -1 -> to the bottom.
// Param for a bonus: 1 -> bonus going to the right, -1 -> to the left.
typedef struct
{
	float X;
	float Y;
	int Param;
} Data;


// Linked lists of Data.
typedef struct node
{
	Data Val;
	struct node* Next;
} NODE;


// Initialisation:


void init_ship(Data *ship);

// Initialize a bonus.
void init_bonus(Data *bonus);

// Create a wave of enemy.
NODE* wave_attack(void);


// Difficulty settings:


// Updates actual_enemy_speed, actual_shot_speed, actual_bonus_speed, and actual_kill_reward when difficulty or wave_number changes.
void update_balance(void);


// Deplacement:


// Move the player's spaceship.
void move_spaceship(Data *ship, int direction); // direction: -1 -> left, 1 -> right.

// Move the current bonus.
int move_bonus(Data *bonus);

// Move an enemy, with zigzag as a parameter.
void move_enemy(Data *enemy, int zigzag, int alea); // zigzag modulo (4 * actual_enemy_speed).

// Move a shot.
void move_shot(Data *shot);


// Drawing:


// Draws the given unit.
void draw_unit(Data *unit, SDL_Texture *unit_model);

// Draws and moves each enemy, and eventually make them attack. Doesn't change the order of enemy_list;
void draw_move_attack_enemies(NODE **enemy_shot_list, NODE **enemy_list, int zigzag, SDL_Texture *alien);

// Updates and draws the value of every shot in the list.
void draw_move_shots(NODE **shot_list, SDL_Color *color);


// Adding new data:


// Adds a data at the start of the list.
void add_on_top(NODE **list, float x, float y, int param);

// The player shoots.
void player_shoot(NODE **player_shot_list, Data *ship);

// This enemy shoots.
void enemy_shoot(NODE **enemy_shot_list, Data *enemy);


// Collisions:


// Check if a given data and shot collide.
int collision_data_shot(Data *data, Data *shot, int data_width, int data_length);

// Check if the given data is touching one of the shots. Doesn't change the order of shot_list.
int collision_data_shotlist(Data *data, NODE **shot_list, int data_width, int data_length);

// Check for collions between enemies (or shots) and shots. Doesn't change the order of data_list.
void collision_datalist_shotlist(NODE **data_list, NODE **shot_list, int data_width, int data_length, int reward, int *score);

// Check if one enemy goes too far to be killed.
int end_reached(NODE *enemy_list);


// Memory:


// Free a list from memory.
void free_list(NODE *list);


// Filename:


// Returns the filename associated with the given name. Will be used for scores, saves AND images.
char* get_filename(char *folder_name, char *name, char *file_type);


// Time measurement:

double time_elapsed(clock_t start);


#endif
