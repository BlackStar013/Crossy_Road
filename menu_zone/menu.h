#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _menu{
    int curpos;
    int nb_options;
    char* title_menu;
    char** menu_options_titles;
    bool (**menu_options)();
} Menu;

bool run_menu(Menu* menu, SDL_Renderer* renderer);
bool game_screenshot_taken(void);
void display_screenshot(SDL_Renderer* renderer);
void display_background(SDL_Renderer* renderer);
void display_options(Menu* menu, SDL_Renderer* renderer);