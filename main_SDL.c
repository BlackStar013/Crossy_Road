#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "mob.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


//void init_display(Map *map, player *joueur){
//    Line *line = map->head;
//    for (int i = 0; i<HEIGTH;i++){
//        for (int j = 0; j<LENGTH; j++){
//            if((i==joueur->y) && (j==joueur->x)){
//                printf(" ");
//            }else{
//                printf("%c", line->tiles[j].type);
//            }
//        }
//        printf("\n");
//        if(i!=LENGTH){
//            line=line->next;
//        }
//    }
//}

//void update_display(Map* map, player *joueur){
//    srand(time(NULL));
//    Line *line = map->head;
//    printf("\033[%dA\r", HEIGTH);
//    for (int i = 0; i<HEIGTH;i++){
//        for (int j = 0; j<LENGTH; j++){
//            if((i==joueur->y) && (j==joueur->x)){
//                printf(" ");
//            }else{
//                printf("\033[32m%c",line->tiles[j].type);
//            }
//        }
//        printf("\n");
//    }
//    fflush(stdout);
//}

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de everything : %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in image init: %s", IMG_GetError());
        exit(-1);
    }
    atexit(IMG_Quit);
    
    SDL_Window *window;
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (HEIGTH*32*2), (LENGTH*32*2), 0);
    if(!window){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la fenetre : %s", SDL_GetError());
        exit(-1);
    }

    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du renderer : %s", SDL_GetError());
        exit(-1);
    }

    SDL_Surface *spriteSurface = IMG_Load("bois.png");   // a remplacer par : sprites/Plaines.aseprite ou autre format du coup
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
        exit(-1);
    }

    SDL_Color const BACKGROUND_COLOR = {.r=0xD0, .g=0xFF, .b=0xFF, .a=SDL_ALPHA_OPAQUE};
    if(SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur pour la couleur de fond : %s", SDL_GetError());
    }

    if(SDL_RenderClear(renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
    }

    SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if(!spriteTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(spriteSurface);

    int spriteFullWidth;
    int spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 4;
    int offset = 0;
    int distance_x = 0;
    int distance_y = 0;

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    Map* place = create_empty_map();
    player *joueur = malloc(sizeof(player));
    joueur->x = LENGTH/2;
    joueur->y = HEIGTH-3;
    //init_display(place, joueur);
    srand(time(NULL));
    SDL_RenderPresent(renderer);
    while(!quit){
        //regarde si il y a des evenements clavier
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT){
            printf("je rentre ici\n");
            quit = SDL_TRUE;
        }else if(event.type == SDL_KEYDOWN){
            //if(event.key.keysym.scancode == SDL_SCANCODE_A){
            //    printf("aaaaaaaaaaaaaaaaaaa\n");
            //}
            if(event.key.keysym.sym == SDLK_z){
                distance_y -= 1;
                //moveup(joueur, place);
                //printf("zzzzzzzzzzzzz\nzzzzzzzzzzzzz\nzzzzzzzzzzzzz\n");
            }else if(event.key.keysym.sym == SDLK_q){
                distance_x -= 1;
                //movedown(joueur, place);
                //printf("qqqqqqqqqqqqq\nqqqqqqqqqqqqq\nqqqqqqqqqqqqq\n");
            }else if(event.key.keysym.sym == SDLK_s){
                distance_y += 1;
                //moveleft(joueur, place);
                //printf("sssssssssssss\nsssssssssssss\nsssssssssssss\n");
            }else if(event.key.keysym.sym == SDLK_d){
                distance_x += 1;
                //move_right(joueur, place);
                //printf("ddddddddddddd\nddddddddddddd\nddddddddddddd\n");
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        SDL_Color const BACKGROUND_COLOR = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = SDL_ALPHA_OPAQUE};
        if(SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur pour la couleur de fond : %s", SDL_GetError());
        }
        if(SDL_RenderClear(renderer)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        }
        SDL_Rect spriteRect = {.x = offset*spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
        SDL_Rect destRect = {.x = distance_x*10, .y = distance_y*10, .w = spriteWidth, .h = spriteHeight};
        if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        SDL_RenderPresent(renderer);
        }
        SDL_Delay(20);
    //update_display(place, joueur);
    }
    free(joueur);
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
