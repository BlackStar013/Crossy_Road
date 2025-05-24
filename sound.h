#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum Sounds {
    MOVEM,
    SELECT,
    NB_SOUND
};

Mix_Chunk** initSoundArray(void);
void freeSoundArray(Mix_Chunk** soundArray);