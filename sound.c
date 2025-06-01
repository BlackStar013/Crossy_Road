#include "sound.h"

Mix_Chunk** initSoundArray(void){
    //initialise un array contenant les sons et musiques du jeu.
    Mix_Chunk** soundsArray = malloc(sizeof(Mix_Chunk*)*NB_SOUND);
    for(int i = 0; i<NB_SOUND; i++){
        if(i==MOVEM){              
            soundsArray[i] = Mix_LoadWAV("sounds/menu-button-88360.wav");
        }
        if(i==SELECT){              
            soundsArray[i] = Mix_LoadWAV("sounds/mixkit-select-click-1109.wav");
        }
        if(i==STARTGAME){
            soundsArray[i] = Mix_LoadWAV("sounds/game-start-6104.wav");
        }
        if(i==CANNON){
            soundsArray[i] = Mix_LoadWAV("sounds/loud-shot-193248.wav");
        }
        if (!soundsArray[i]) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }
    return soundsArray;
}

 Mix_Music** initMusicArray(void){
    Mix_Music** musicArray = malloc(sizeof(Mix_Chunk*)*NB_MUSIC);
    for(int i = 0; i<NB_MUSIC; i++){
        if(i==MUS_PLAINS){
            musicArray[i] = Mix_LoadMUS("sounds/plains.mp3");
        }
        if(i==MUS_MANOR){
            musicArray[i] = Mix_LoadMUS("sounds/manor.mp3");
        }
        if(i==MUS_CAVE){
            musicArray[i] = Mix_LoadMUS("sounds/cave.mp3");
        }
        if(i==MUS_MONSTER){
            musicArray[i] = Mix_LoadMUS("sounds/monster.mp3");
        }
        if(i==MUS_GAMEOVER){
            musicArray[i] = Mix_LoadMUS("sounds/game_over.mp3");
        }
        if (!musicArray[i]) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }
    return musicArray;
 }

void freeSoundArray(Mix_Chunk** soundArray){
    for(int i = 0; i<NB_SOUND; i++){
        Mix_FreeChunk(soundArray[i]);
    }
    free(soundArray);
}

void freeMusicArray(Mix_Music** musicArray){
    for(int i = 0; i<NB_MUSIC; i++){
        Mix_FreeMusic(musicArray[i]);
    }
    free(musicArray);
}
