#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "sound.h"
#include "save_load.h"
#include "SDLdisplay.h"
#include "display.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define ECHELLE 2

enum MAINMENU{
    START,
    CHANGECHARA,
    MMOPTIONS,
    MMQUIT,
    NB_MMOPTION
};

enum PAUSEMENU{
    RESUME,
    GOMAINMENU,
    PMOPTIONS,
    PMQUIT,
    NB_PMOPTION
};

enum GAMEOVERMENU{
    RETRY,
    GOGOMAINMENU,
    GOQUIT,
    NB_GOOPTION
};

enum OPTIONSMENU{
    SOUND,
    SHORTMODE,
    SEE_HIGHSCORE,
    RESET_SAVE,
    EXIT,
    NB_OPTOPTION
};

SDL_Texture* strToTexture(char* str, TTF_Font* font, SDL_Renderer* renderer);
void displayMainMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth);
void displayPauseMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, SDL_Surface* cursorSurface, SDL_Surface* backSurface);
void displayCharaMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth);
void displayCharaMenu2(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, SDL_Surface** surfaceArrayPlayer, Game* game, int* unlocked);
void displayGameOverMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth);
void gameOverMenu(SDL_bool* main_menu, SDL_bool* quit, Game** game, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int chara);
void pauseMenu(SDL_bool* main_menu, SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, Mix_Chunk** soundArray, int* shortmode, bool* sound, bool* reset_save);
void charaMenu(SDL_bool* quit, int* pchara, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, SDL_Surface** surfaceArrayPlayer, Game* game);
void optionsMenu(SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int* shortmode, bool* sound, bool* reset_save);
void displayOptionsMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, SDL_Surface* cursorSurface, SDL_Surface* backSurface);
