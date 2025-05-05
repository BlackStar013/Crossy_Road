#include "menu.h"

//the returned bool serves to indicate whether or not the program layer below should kill itself
bool run_menu(Menu* menu, SDL_Renderer* renderer){
    //never clears the renderer to continue to display the game in the background
    if (game_screenshot_taken()){
        display_screenshot(renderer);        
    }

    SDL_Event event;
    bool kys = false;
    SDL_bool quit = SDL_FALSE;
    while(!quit){
        display_background(renderer);
        display_options(menu, renderer);

        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            quit = SDL_TRUE;
        }
        else if(event.type == SDL_KEYUP){
            // bouge le joueur dans la direction selon la touche pressée, ZQSD, ou quitte avec M
            if(event.key.keysym.sym == SDLK_z){
                menu->curpos = (menu->curpos+1)%(menu->nb_options);
            }else if(event.key.keysym.sym == SDLK_s){
                menu->curpos = (menu->curpos+1)%(menu->nb_options);
            }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
                kys = (menu->menu_options)[curpos]();
                if (kys){
                    quit = SDL_TRUE;
                }
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        }
    }
    return kys;
}

void display_background(SDL_Renderer *renderer){
    // affiche le joueur selon le même principe que ci-dessus
    SDL_Surface *spriteSurface = IMG_Load("sprites/menu.png");
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
        exit(-1);
    }
    SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if(!spriteTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
        exit(-1);
    }
    int spriteFullWidth;
    int spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = spriteFullWidth;
    int spriteHeight = spriteFullHeight;

    int displayedWidth = spriteWidth; //Temp value
    int displayedHeight = spriteHeight; //Temp value

    // recupère dans le fichier la position depuis en haut à gauche (d'ou le (0, 0)pour x et y) et la taille calculée au dessus
    SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
    // x et y correspondent à la position où on va l'afficher, et w et h sa taille
    SDL_Rect destRect = {.x = 0, .y = 0, .w = displayedWidth, .h = displayedHeight};
    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
    }

    SDL_DestroyTexture(spriteTexture);
    SDL_FreeSurface(spriteSurface);
}
