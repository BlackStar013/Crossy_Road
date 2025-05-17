#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "mob.h"

SDL_Surface** initSurfaceArrayMap(void);
void freeSurfaceArrayMap(SDL_Surface** surfaceArrayMap);
SDL_Surface** initSurfaceArrayMob(void);
void freeSurfaceArrayMob(SDL_Surface** surfaceArrayMob);
SDL_Surface** initSurfaceExp(void);
void freeSurfaceExp(SDL_Surface** surfaceExp);
SDL_Surface** initSurfaceArrayProjectile(void);
void freeSurfaceArrayProjectile(SDL_Surface** surfaceArrayProjectile);
SDL_Surface** initSurfaceArrayPlayer(void);
void freeSurfaceArrayPlayer(SDL_Surface** surfaceArrayPlayer);
