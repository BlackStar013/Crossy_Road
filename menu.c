#include "menu.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Texture* strToTexture(char* str, TTF_Font* font, SDL_Renderer* renderer){
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, textColor);
    if (textSurface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Unable to render text surface! SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
}

void displayMainMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth){
    //printf("%d %d\n", width, heigth);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Surface* titleSurface = IMG_Load("sprites/titre.png");
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Surface* backSurface = IMG_Load("sprites/menu.png");
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    SDL_Texture* textTextures[] = {strToTexture("START", font, renderer), strToTexture("SELECT CHARACTER", font, renderer), strToTexture("OPTIONS", font, renderer), strToTexture("QUIT", font, renderer)};

    SDL_Rect backRect = {0, 0, width, heigth};
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 150*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*5)/2, (23*8)/2, 80, (23*4)/2};

    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50*ECHELLE)-40*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*5)/2)*ECHELLE, heigth / 2 - 20*ECHELLE, 23*5*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*8)/2)*ECHELLE, heigth / 2 +30*ECHELLE, 23*8*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 80*ECHELLE, heigth / 2 + 80*ECHELLE, 160*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 130*ECHELLE, 23*4*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<4; i++){
        SDL_DestroyTexture(textTextures[i]);
    }
    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(backTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

void gameOverMenu(SDL_bool* main_menu, SDL_bool* quit, Game** game, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int chara, bool sound){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    while(!quit2){
        displayGameOverMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE));
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor-1)%NB_GOOPTION;
                if(cursor < 0){
                    cursor = NB_GOOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor+1)%NB_GOOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[SELECT], 0);
                }
                if(cursor == RETRY){
                    int exp = (*game)->nb_exp;
                    int shortmode = (*game)->short_mode;
                    destroy_game(*game);;
                    *game = initialise_game(chara,exp,shortmode);
                    (*game)->nb_exp = exp;
                    *main_menu = SDL_FALSE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOGOMAINMENU){
                    *main_menu = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOQUIT){
                    *quit = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
}

void displayGameOverMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth){
    //printf("%d %d\n", width, heigth);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Surface* titleSurface = IMG_Load("sprites/game_over.png");
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Texture* textTextures[] = {strToTexture("RETRY", font, renderer), strToTexture("MAIN MENU", font, renderer), strToTexture("QUIT", font, renderer)};

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 150*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*5)/2, (23*9)/2, (23*4)/2};
    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50)*ECHELLE-20*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*5)/2)*ECHELLE, heigth / 2, (23*5)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*9)/2)*ECHELLE, heigth / 2 + 50*ECHELLE, (23*9)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 100*ECHELLE, (23*4)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<3; i++){
        SDL_DestroyTexture(textTextures[i]);
    }
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

void pauseMenu(SDL_bool* main_menu, SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, Mix_Chunk** soundArray, int* shortmode, bool* sound, bool* reset_save){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Surface* backSurface = IMG_Load("sprites/filtrepause.png");
    while(!quit2){
        displayPauseMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE), game, pix_defil, surfaceArrayMap, surfaceArrayMob, surfaceArrayPlayer, surfaceArrayProjectile, surfaceExp, cursorSurface, backSurface);
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor-1)%NB_PMOPTION;
                if(cursor < 0){
                    cursor = NB_PMOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor+1)%NB_PMOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[SELECT], 0);
                }
                if(cursor == RESUME){
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOMAINMENU){
                    *main_menu = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == PMOPTIONS){
                    optionsMenu(quit, renderer, pevent, font, soundArray, shortmode, sound, reset_save);
                }
                if(cursor == PMQUIT){
                    *quit = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
    SDL_FreeSurface(backSurface);
    SDL_FreeSurface(cursorSurface);
}

void displayPauseMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, SDL_Surface* cursorSurface, SDL_Surface* backSurface){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Texture* titleTexture = strToTexture("PAUSE", font, renderer);

    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    display_map(game->carte, renderer,pix_defil, surfaceArrayMap);
    display_exp(game->exp, renderer,pix_defil, surfaceExp);
    display_mob(game->mobs, renderer,pix_defil, surfaceArrayMob);
    display_projectile(game->projectile, renderer,pix_defil, surfaceArrayProjectile);
    display_joueur(game->joueur, renderer,pix_defil, surfaceArrayPlayer);

    SDL_Texture* textTextures[] = {strToTexture("RESUME", font, renderer), strToTexture("MAIN MENU", font, renderer), strToTexture("OPTIONS", font, renderer), strToTexture("QUIT", font, renderer)};
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_Rect backRect = {0, 0, width, heigth};
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 180*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*6)/2, (23*9)/2, 80, (23*4)/2};

    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50*ECHELLE)-40*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*6)/2)*ECHELLE, heigth / 2 - 20*ECHELLE, 23*6*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*9)/2)*ECHELLE, heigth / 2 +30*ECHELLE, 23*9*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 80*ECHELLE, heigth / 2 + 80*ECHELLE, 160*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 130*ECHELLE, 23*4*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<4; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    SDL_DestroyTexture(backTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_DestroyTexture(cursorTexture);
}

void displayCharaMenu2(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, SDL_Surface** surfaceArrayPlayer, Game* game, int* unlocked){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    int selection[] = {0,0,0,0,0,0,0,0};
    selection[cursor] = 1;
    
    SDL_Surface* cadrePersoSurface = IMG_Load("sprites/cadre_perso_menu.png");
    SDL_Texture* cadrePersoTexture = SDL_CreateTextureFromSurface(renderer, cadrePersoSurface);
    int spriteFullWidth;
    int spriteFullHeight;
    if (SDL_QueryTexture(cadrePersoTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Rect spriteCadreRects[8];
    for (int i = 0; i<NB_PERSO; i++){
        spriteCadreRects[i].x = selection[i]*spriteFullWidth/2;
        spriteCadreRects[i].y = 0;
        spriteCadreRects[i].w = spriteFullWidth/2;
        spriteCadreRects[i].h = spriteFullHeight;
    }

    SDL_Rect spritePersoRect = {0,0,32,32};
    
    SDL_Texture* titleTexture = strToTexture("CHARACTERS", font, renderer);

    SDL_Texture* textTextures[] = {strToTexture("DEDE", font, renderer), strToTexture("AESIR", font, renderer), strToTexture("KAIRON", font, renderer), strToTexture("RANTON", font, renderer), strToTexture("JOLINAR", font, renderer), strToTexture("SNITCH", font, renderer), strToTexture("OOGWAAY", font, renderer), strToTexture("KABI", font, renderer)};
    SDL_Texture* lvlTextures[] = {strToTexture("Lv.0", font, renderer), strToTexture("Lv.1", font, renderer), strToTexture("Lv.2", font, renderer), strToTexture("Lv.3", font, renderer)};
    //SDL_Rect backRect = {0, 0, width, heigth};
    //SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120*0.75*ECHELLE, heigth / 2 - 180*0.75*ECHELLE, 240*0.75*ECHELLE, 96*0.75*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    SDL_Surface* spriteLockedSurface = IMG_Load("sprites/Cadenas.png");

    for(int i = 0; i < 4; i++){
        SDL_Rect rectC = {width / 2 - (100 + 80 + 5)*0.75*ECHELLE - 32*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE, 32*ECHELLE, 32*ECHELLE};
        SDL_RenderCopy(renderer, cadrePersoTexture, &spriteCadreRects[i], &rectC);

        if (unlocked[i] != 0){
            SDL_Surface *spriteSurface = surfaceArrayPlayer[i];
            SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
            SDL_RenderCopy(renderer, spriteTexture, &spritePersoRect, &rectC);
            SDL_DestroyTexture(spriteTexture);
        }
        else{
            SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteLockedSurface);
            SDL_RenderCopy(renderer, spriteTexture, NULL, &rectC);
            SDL_DestroyTexture(spriteTexture);
        }

        SDL_Rect rect = {width / 2 - (100 + 80)*0.75*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE + 2*ECHELLE, 160*0.75*ECHELLE, 32*0.70*0.75*ECHELLE};
        SDL_RenderCopy(renderer, textTextures[i], NULL, &rect);

        SDL_Rect rectL = {width / 2 - (100 + 80)*0.75*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE + 32*0.75*0.75*ECHELLE + 2*ECHELLE, 40*0.75*ECHELLE, 32*0.45*0.75*ECHELLE};
        SDL_RenderCopy(renderer, lvlTextures[unlocked[i]], NULL, &rectL);
    }

    for(int i = 0; i < 4; i++){
        SDL_Rect rectC = {width / 2 + (100 - 80 - 5)*0.75*ECHELLE - 32*ECHELLE + 25*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE, 32*ECHELLE, 32*ECHELLE};
        SDL_RenderCopy(renderer, cadrePersoTexture, &spriteCadreRects[i+4], &rectC);

        if (unlocked[i+4] != 0){
            SDL_Surface *spriteSurface = surfaceArrayPlayer[i+4];
            SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
            SDL_RenderCopy(renderer, spriteTexture, &spritePersoRect, &rectC);
            SDL_DestroyTexture(spriteTexture);
        }
        else{
            SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteLockedSurface);
            SDL_RenderCopy(renderer, spriteTexture, NULL, &rectC);
            SDL_DestroyTexture(spriteTexture);
        }

        SDL_Rect rect = {width / 2 + (100 - 80)*0.75*ECHELLE + 25*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE + 2*ECHELLE, 160*0.75*ECHELLE, 32*0.70*0.75*ECHELLE};
        SDL_RenderCopy(renderer, textTextures[i+4], NULL, &rect);

        SDL_Rect rectL = {width / 2 + (100 - 80)*0.75*ECHELLE + 25*ECHELLE, heigth / 2 + i*50*0.75*ECHELLE + 32*0.75*0.75*ECHELLE + 2*ECHELLE, 40*0.75*ECHELLE, 32*0.45*0.75*ECHELLE};
        SDL_RenderCopy(renderer, lvlTextures[unlocked[i+4]], NULL, &rectL);
    }
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<8; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_FreeSurface(spriteLockedSurface);
    SDL_FreeSurface(cadrePersoSurface);
    SDL_DestroyTexture(cadrePersoTexture);
}

void charaMenu(SDL_bool* quit, int* pchara, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, SDL_Surface** surfaceArrayPlayer, Game* game, bool sound){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    int* unlocked = malloc(sizeof(int)*8);
    updateLocks(unlocked, game->nb_exp);
    while(!quit2){
        displayCharaMenu2(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE), surfaceArrayPlayer, game, unlocked);
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor-1)%NB_PERSO;
                if(cursor < 0){
                    cursor = NB_PERSO-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor+1)%NB_PERSO;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[SELECT], 0);
                }
                if(cursor == KAIRON && unlocked[cursor] != 0){
                    *pchara = KAIRON;
                    quit2 = SDL_TRUE;
                }if(cursor == SNITCH && unlocked[cursor] != 0){
                    *pchara = SNITCH;
                    quit2 = SDL_TRUE;
                }if(cursor == AESIR && unlocked[cursor] != 0){
                    *pchara = AESIR;
                    quit2 = SDL_TRUE;
                }if(cursor == KABI && unlocked[cursor] != 0){
                    *pchara = KABI;
                    quit2 = SDL_TRUE;
                }if(cursor == OOGWAAY && unlocked[cursor] != 0){
                    *pchara = OOGWAAY;
                    quit2 = SDL_TRUE;
                }if(cursor == JOLINAR && unlocked[cursor] != 0){
                    *pchara = JOLINAR;
                    quit2 = SDL_TRUE;
                }if(cursor == RANTON && unlocked[cursor] != 0){
                    *pchara = RANTON;
                    quit2 = SDL_TRUE;
                }if(cursor == LEDE && unlocked[cursor] != 0){
                    *pchara = LEDE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_d){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[SELECT], 0);
                }
                descMenu(quit, renderer, pevent, font, soundArray, cursor, unlocked[cursor], sound);
            }else if(pevent->key.keysym.sym == SDLK_TAB){
                quit2 = SDL_TRUE;
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
    free(unlocked);
}

void optionsMenu(SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int* shortmode, bool* sound, bool* reset_save){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Surface* backSurface = IMG_Load("sprites/filtrepause.png");
    while(!quit2){
        displayOptionsMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE), cursorSurface, backSurface);
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor-1)%NB_OPTOPTION;
                if(cursor < 0){
                    cursor = NB_OPTOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                cursor = (cursor+1)%NB_OPTOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(*sound){
                    Mix_PlayChannel(-1, soundArray[SELECT], 0);
                }
                if(cursor == SOUND){
                    *sound = !*sound;
                }
                if(cursor == SHORTMODE){
                    *shortmode = (*shortmode+1)%2;
                }
                if(cursor == SEE_HIGHSCORE){

                }
                if(cursor == RESET_SAVE){
                    *reset_save = true;
                }
                if(cursor == EXIT){
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_m){
                quit2 = SDL_TRUE;
            }
        }
    }
    SDL_FreeSurface(backSurface);
    SDL_FreeSurface(cursorSurface);
}

void displayOptionsMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, SDL_Surface* cursorSurface, SDL_Surface* backSurface){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Texture* titleTexture = strToTexture("OPTIONS", font, renderer);

    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    SDL_Texture* textTextures[] = {strToTexture("SOUND", font, renderer), strToTexture("SHORTMODE", font, renderer), strToTexture("HIGHSCORE", font, renderer), strToTexture("RESET SAVE", font, renderer), strToTexture("EXIT", font, renderer)};
    
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    //SDL_Rect backRect = {0, 0, width, heigth};
    //SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
    
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 180*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*5)/2, (23*9)/2, (23*9)/2, (23*10)/2, (23*4)/2};

    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*40*ECHELLE) - 55*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - decals[0]*ECHELLE, heigth / 2 - 35*ECHELLE, 2*decals[0]*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - decals[1]*ECHELLE, heigth / 2 +5*ECHELLE, 2*decals[1]*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - decals[2]*ECHELLE, heigth / 2 + 45*ECHELLE, 2*decals[2]*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - decals[3]*ECHELLE, heigth / 2 + 85*ECHELLE, 2*decals[3]*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);
    SDL_Rect rect5 = {width / 2 - decals[4]*ECHELLE, heigth / 2 + 125*ECHELLE, 2*decals[4]*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[4], NULL, &rect5);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<5; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    SDL_DestroyTexture(backTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_DestroyTexture(cursorTexture);
}

int count_char_occurrences(char* str, char ch){
    int count = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == ch) {
            count++;
        }
    }
    return count;
}

void displayTutorial(SDL_Renderer* renderer, TTF_Font* font, int width, int heigth){

}

void displayDescMenu(int chara, int lvl, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth){

    char* texts[] = {"\nRegardez la belle I.A, elle est belle hein ? Eh bah elle vous rapportera pas d'exp, nan.\nVous etes punis. Allez, au coin. Zou. Zou j'ai dis.",
        "D'aucun dites que vous etes le protagonistes, assez classique et plat mais bien aimant...\nC'est bien pour ca que vous ne montrez pas vos pouvoirs !\nPar contre... Je me dis que contre un peu d'argent, peut-etre que je vous\ndenoncerais a notre paladin, en disant que c'est vous, le veritable antagoniste du jeu !\n( Pouvoir en DLC payant, arrivant en cas de soutiens du jeu )",
        "Un demon ? J'aime pas les demons. \"Blabla je suis paladin\", j'ai quand meme pas confiance.\nAu moins vous pouvez utilisez votre bel armure... que vous avez vole n'est-ce pas ?",
        "\n\nOh, tiens, l'experience vous a rendu plus fort, vous pouvez l'utilisez deux fois cette armure.\nEh bien dis donc...",
        "\n\n3 utilisations d'armures ? Wow, on dirait presque un protagoniste classique.\nPeut-etre que vous etes vraiment gentil au final.",
        "Oh, un druide ? Je suis fan ! Et vous adorez les champignons ? Vous dites que\nc'est au moins deux fois plus efficace que l'experience de la vie ? Eh bien, c'est une\nlecon a ne pas donner a la jeunesse, mais je connais un professeur de SIC\nqui serait bien d'accord.",
        "\n\nC'est normal que vous attiriez les champignons ? Euh... Prenez une douche.",
        "\n\nAttendez, maintenant ca aide aussi pour le score ? J'etais d'accord au debut,\n mais la je crois que je vais devoir appeler la police concernant votre consommation...",
        "Non, non, arretez, vous n'etes pas une porte ! Nan, stoppez ceci ! En plus ca n'a rien\na voir avec vos pouvoir, vous immobilisez juste une ligne entiere... Alors pourquoi cette obsession ?\n",
        "\n\n2 ligne maintenant ? Et plus souvent ? Wow, vous etes un bon. Genre vraiment bon.\n...Bon, vous pouvez me repondre ? J'ai l'impression de parler a une porte la... ",
        "\n\nComment ca le scenario entier est lie a une porte ? Qu'est-ce que tu me fumes !? T'es potes avec le\ndruide aux champis ? Il y a meme pas de porte dans le jeu ! Ouais je brise le 4eme mur, mais ca m'a enerve !\nNon j'expliquerais pas la capacite, vous allez bien devinez en jouant. Bordel.",
        "Wow, depuis quand vous pouvez disparaitre pour vous rendre invisible et donc invincible ?\nC'est parce que vous etes un chat ou un voleur ? En fait ne me repondez pas...",
        "\n\nWow, vous durez plus longtemps maintenant ! ( titre ) En tout cas, ca doit etre grace\na toute les fois que vous avez du fuir in extremis les gens que vous avez enerve...",
        "\n\nOui, oui, vous savez maintenant le faire plus souvent. (titre).\nC'est bien beau, mais, lachez cette langue de mimique, c'est degeulasse.",
        "Une tortue qui saute ? Genre, vraiment, une vielle tortue qui saute, et qui fait des arts martiaux.\nWow. J'aurais tout vu.",
        "\n\n...Une tortue, qui saute, et qui se bourre la gueule. La c'est meme plus un vieux moine classique,\nil s'enfile des tonnaux de bieres ? Et le pire, c'est que ca l'a ameliore ! Il est vraiment plus fort !\nEh ben, dis donc, ne faites pas lire ce jeu a vos enfants.",
        "\n\n...Comment ca il a participe au genocide de l'entierete de la garde de toute une île ? Genre, comme ca ?\nJe le croyais mignon moi a la base. Euh... J'aimerais bien m'eloigner, mais maintenant il n'a meme plus besoin\nd'attendre pour sauter, dites a ma femme et mes enfants que je les aimes !",
        "On dirait pas comme ca, mais elle fait a peu pres 1m20. Et pourtant, c'est pas un chaton mignon,\nles fleches qu'elle lance tuent des cohortes, c'est vraiment vrai. Petit, mais costaud.",
        "\n\nEn plus de savoir lancer des fleches plus vite, elle peut cartographier ! Bon, ca sert a rien,\ndonc c'est pas implemente, mais, c'est pas juste une tueuse sanglante, meme si elle en a les capacites",
        "\n\nBah la on peut la considerer comme une machine de guerre. Je vous conseil d'etre de son côte."};
    
    int index[] = {0,1,2,5,8,11,14,17};
    SDL_Texture* textTextures[] = {strToTexture("DEDE", font, renderer), strToTexture("AESIR", font, renderer), strToTexture("KAIRON", font, renderer), strToTexture("RANTON", font, renderer), strToTexture("JOLINAR", font, renderer), strToTexture("SNITCH", font, renderer), strToTexture("OOGWAAY", font, renderer), strToTexture("KABI", font, renderer)};
    
    SDL_Texture* exitTexture = strToTexture("PRESS ENTER TO EXIT", font, renderer);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Rect titleRect = {width / 2 - 120*0.75*ECHELLE, heigth / 2 - 180*0.75*ECHELLE, 240*0.75*ECHELLE, 96*0.75*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[chara], NULL, &titleRect);

    SDL_Rect rect1 = {width / 2 - 120*0.75*ECHELLE, heigth / 2 + 180*0.75*ECHELLE, 240*0.75*ECHELLE, 50*0.75*ECHELLE};
    SDL_RenderCopy(renderer, exitTexture, NULL, &rect1);

    char message[2000] = "";
    for(int i = 0; i<lvl; i++){
        strcat(message, texts[index[chara]+i]);
    }
    
    SDL_Color textColor = {255, 255, 255, 255};

    char* line = strtok(message, "\n");  // Split message by newline
    int y_offset = heigth / 2 - 35 * ECHELLE;

    while (line != NULL) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, line, textColor);
        SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect rect2 = {width / 2 - 180 * ECHELLE, y_offset, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, messageTexture, NULL, &rect2);

        y_offset += textSurface->h * ECHELLE;

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(messageTexture);

        line = strtok(NULL, "\n");  // Move to next line
    }
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<8; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    SDL_DestroyTexture(exitTexture);
}

void Tutorial(){

}

void descMenu(SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int chara, int lvl, bool sound){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    while(!quit2){
        displayDescMenu(chara, lvl, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE));
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN) || (pevent->key.keysym.sym == SDLK_TAB)){
                if(sound){
                    Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                }
                quit2 = SDL_TRUE;
            }else if(pevent->key.keysym.sym == SDLK_m){
                quit2 = SDL_TRUE;
            }
        }
    } 
}
