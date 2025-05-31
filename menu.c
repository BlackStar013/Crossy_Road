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

void gameOverMenu(SDL_bool* main_menu, SDL_bool* quit, Game** game, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, int chara){
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
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor-1)%NB_GOOPTION;
                if(cursor < 0){
                    cursor = NB_GOOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor+1)%NB_GOOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                Mix_PlayChannel(-1, soundArray[SELECT], 0);
                if(cursor == RETRY){
                    destroy_game(*game);;
                    *game = initialise_game(chara,0);
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
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor-1)%NB_PMOPTION;
                if(cursor < 0){
                    cursor = NB_PMOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor+1)%NB_PMOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                Mix_PlayChannel(-1, soundArray[SELECT], 0);
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

void displayCharaMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Texture* titleTexture = strToTexture("CHARACTERS", font, renderer);

    SDL_Texture* textTextures[] = {strToTexture("KAIRON", font, renderer), strToTexture("SNITCH", font, renderer), strToTexture("AESIR", font, renderer), strToTexture("KABI", font, renderer), strToTexture("OOGWAAY", font, renderer), strToTexture("JOLINAR", font, renderer), strToTexture("RANTON", font, renderer), strToTexture("LEDE", font, renderer)};

    //SDL_Rect backRect = {0, 0, width, heigth};
    //SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120, heigth / 2 - 180, 240, 96};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    if (0<=cursor && cursor <=3){
        SDL_Rect curRect = {width / 2 - 100 - 130, heigth / 2+(cursor*50)-20, 2*32, 2*32};
        SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);
    } else if (4<=cursor && cursor<=7){
        SDL_Rect curRect = {width / 2 + 100 - 130, heigth / 2+((cursor-4)*50)-20, 2*32, 2*32};
        SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);
    }

    SDL_Rect rect1 = {width / 2 - 100 - 80, heigth / 2, 160, 32};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - 100 - 80, heigth / 2 + 50, 160, 32};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 100 - 80, heigth / 2 + 100, 160, 32};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - 100 - 80, heigth / 2 + 150, 160, 32};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);

    SDL_Rect rect5 = {width / 2 + 100 - 80, heigth / 2, 160, 32};
    SDL_RenderCopy(renderer, textTextures[4], NULL, &rect5);
    SDL_Rect rect6 = {width / 2 + 100 - 80, heigth / 2 + 50, 160, 32};
    SDL_RenderCopy(renderer, textTextures[5], NULL, &rect6);
    SDL_Rect rect7 = {width / 2 + 100 - 80, heigth / 2 + 100, 160, 32};
    SDL_RenderCopy(renderer, textTextures[6], NULL, &rect7);
    SDL_Rect rect8 = {width / 2 + 100 - 80, heigth / 2 + 150, 160, 32};
    SDL_RenderCopy(renderer, textTextures[7], NULL, &rect8);

    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<8; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_FreeSurface(cursorSurface);
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

void charaMenu(SDL_bool* quit, int* pchara, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Mix_Chunk** soundArray, SDL_Surface** surfaceArrayPlayer, Game* game){
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
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor-1)%NB_PERSO;
                if(cursor < 0){
                    cursor = NB_PERSO-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor+1)%NB_PERSO;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                Mix_PlayChannel(-1, soundArray[SELECT], 0);
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
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor-1)%NB_OPTOPTION;
                if(cursor < 0){
                    cursor = NB_OPTOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                Mix_PlayChannel(-1, soundArray[MOVEM], 0);
                cursor = (cursor+1)%NB_OPTOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                Mix_PlayChannel(-1, soundArray[SELECT], 0);
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
