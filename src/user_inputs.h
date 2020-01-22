#ifndef INPUT_H
#define INPUT_H


#include "SDLA.h"
#include "settings.h"
#include "core.h"
#include "menus.h"
#include "saving.h"
#include "init.h"


extern char *Text_input;

extern const Uint8 *Keyboard_state;

extern Menu current_menu;
extern Game game_state;
extern Saving saving_state;

extern int redraw_menu;

extern int quit;
extern int loading_succeed;

extern SDL_Texture *alien_easy;
extern SDL_Texture *alien_medium;
extern SDL_Texture *alien_hard;
extern SDL_Texture *current_alien;

extern NODE *enemy_list;
extern NODE *enemy_shot_list;
extern NODE *player_shot_list;

extern Data ship;

extern Difficulty difficulty;
extern int wave_number;
extern int actual_enemy_speed;
extern float actual_shot_speed;
extern float actual_bonus_speed;
extern int actual_kill_reward;

extern int time_step;
extern int input_time;
extern int animation_time;
extern int bonus_time;

extern int bonus_spawned;
extern int bonus_gone;

extern int invincibility;
extern int drawing_ship;


// Handle user inputs.
void input_control();


// Returns 1 if the given key is actually pressed, and if so updates last_pressed_key, left_down and redraw_menu.
int key_pressed(SDL_Keycode key);

#endif
