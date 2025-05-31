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
    CANNON,
    NB_SOUND
};

enum Musics {
    MUS_PLAINS,
    MUS_MANOR,
    MUS_CAVE,
    MUS_MONSTER,
    NB_MUSIC
};

Mix_Chunk** initSoundArray(void);
Mix_Music** initMusicArray(void);
void freeSoundArray(Mix_Chunk** soundArray);
void freeMusicArray(Mix_Music** musicArray);
