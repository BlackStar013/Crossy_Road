#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "ia.h"
#include "core.h"
#include "menu.h"
#include "mob.h"
#include "display.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void display_map(Map *map, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMap);
void display_joueur(Player *joueur, SDL_Renderer *renderer,int pix_defil, SDL_Surface** surfaceArrayPlayer);
void display_exp(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceExp);
void display_mob(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMob);
void display_projectile(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayProjectile);
void display_all(Game *game, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, TTF_Font* font);
