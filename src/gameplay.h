#ifndef GAMEPLAY_H
#define GAMEPLAY_H


#include "SDLA.h"
#include "settings.h"
#include "menus.h"
#include "core.h"
#include "saving.h"


extern SDL_Renderer *renderer;

extern Menu current_menu;
extern Game game_state;
extern Saving saving_state;

extern int redraw_menu;

extern TTF_Font *font_medium;

extern SDL_Texture *wallpaper;
extern SDL_Texture *space_ship;
extern SDL_Texture *current_alien;
extern SDL_Texture *bonus_model;

extern SDL_Color White;
extern SDL_Color Yellow;
extern SDL_Color Red;

extern Data ship;
extern Data bonus;


void play(void);


#endif
