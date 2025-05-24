#include "sound.h"

Mix_Chunk** initSoundArray(void){
    Mix_Chunk** soundsArray = malloc(sizeof(Mix_Chunk*)*NB_SOUND);
    for(int i = 0; i<NB_SOUND; i++){
        if(i==MOVEM){              
            soundsArray[i] = Mix_LoadWAV("sounds/menu-button-88360.wav");
        }
        if(i==SELECT){              
            soundsArray[i] = Mix_LoadWAV("sounds/mixkit-select-click-1109.wav");
        }
        if (!soundsArray[i]) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
        //Mix_PlayChannel(-1, soundsArray[i], 0);
    }
    return soundsArray;
}

void freeSoundArray(Mix_Chunk** soundArray){
    for(int i = 0; i<NB_SOUND; i++){
        Mix_FreeChunk(soundArray[i]);
    }
    free(soundArray);
}