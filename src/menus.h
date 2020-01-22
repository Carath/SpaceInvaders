#ifndef MENUS_H
#define MENUS_H


#include "SDLA.h"
#include "settings.h"
#include "core.h" // for get_filename...
#include "saving.h" // for Result.


typedef enum {IN_GAME, MAIN_MENU, PAUSE_MENU, DIFFICULTY_MENU, COMMANDS_MENU, SCORES_MENU, GAMEOVER_MENU, NAME_MENU} Menu;

typedef enum {GAME_NOT_STARTED, GAME_ON, GAME_PAUSED, GAME_OVER} Game;
// current_menu == IN_GAME if and only if game_state == GAME_ON.

typedef enum {NOT_SAVING, SAVING_SCORE, SAVING_GAME, LOADING_GAME} Saving;


extern SDL_Renderer *renderer;
extern char *Text_input;

extern SDL_Color White;

extern TTF_Font *font_small;
extern TTF_Font *font_medium;
extern TTF_Font *font_big;

extern SDL_Texture *game_over;
extern SDL_Texture *alien_easy;
extern SDL_Texture *alien_medium;
extern SDL_Texture *alien_hard;
extern SDL_Texture *current_alien;
extern SDL_Texture *alien0;
extern SDL_Texture *alien1;
extern SDL_Texture *alien2;
extern SDL_Texture *alien3;
extern SDL_Texture *alien4;
extern SDL_Texture *alien5;
extern SDL_Texture *alien6;

extern Menu current_menu;
extern Saving saving_state;

extern int redraw_menu;

extern int loading_succeed;

extern int score;
extern int score_steps_number;

extern int drawing_ship;


// Render the current menu, if menu_rendred = 1.
void render_menu(void);


// Figure which menu is to be drawn.
void draw_menu(void);


// Afficher le cadre du menu.
void wallpaper_menu(void);


#endif
