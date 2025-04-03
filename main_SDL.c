#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define ECHELLE 2

void display_map(Map *map, Player *joueur, SDL_Renderer *renderer){
    Line *line = map->head_map;
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            SDL_Surface *spriteSurface;
            SDL_Texture *spriteTexture;
            //if((i==joueur->y) && (j==joueur->x)){
            //    spriteSurface = IMG_Load("sprites/Kairon.png");
            //}else{
                if((line->tiles[j].type=='_') && (line->tiles[j].color=='g')){              //charge le sprite a afficher
                    spriteSurface = IMG_Load("sprites/Plaines.png");
                }else if(line->tiles[j].type=='_'){
                    spriteSurface = IMG_Load("sprites/Routes.png");
                }else if(line->tiles[j].type=='#'){
                    spriteSurface = IMG_Load("sprites/Rochers.png");
                }else if(line->tiles[j].type=='~'){
                    spriteSurface = IMG_Load("sprites/Eaumilieu.png");
                }else{
                    spriteSurface = IMG_Load("sprites/Planche.png");
                }
            //}
            if (!spriteSurface) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);                                                // créer la surface avec le sprite
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            SDL_FreeSurface(spriteSurface);
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {                             //creer la texture 
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = (spriteFullWidth / 2)*ECHELLE;
            int spriteHeight = (spriteFullHeight / 2)*ECHELLE;
            SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = 32*j*ECHELLE, .y = 32*i*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect))                                                   //finalement l'ajout à l'affichage
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            }
            if(i!=LENGTH){
            line=line->next;
        }
    }
}


int main(void){
    //globalement beaucoup d'initialisation diverses et variees
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
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (HEIGTH*32*ECHELLE), (LENGTH*32*ECHELLE), 0);
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

    SDL_Surface *spriteSurface = IMG_Load("sprites/Kairon.png");
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
        exit(-1);
    }

    //SDL_Color const BACKGROUND_COLOR = {.r=0x00, .g=0x00, .b=0x00, .a=SDL_ALPHA_OPAQUE};
    //if(SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
    //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur pour la couleur de fond : %s", SDL_GetError());
    //}

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

    int spriteWidth = (spriteFullWidth / 4);
    int spriteHeight = (spriteFullHeight / 4);
    int offset_x = 0;
    int offset_y = 0;
    Game *game = initalise_game();
    int distance_x = (LENGTH-1)/2;
    int distance_y = HEIGHT-3;

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    //Map* place = create_empty_map();
    //pour tester le chargement des diffrentes tuiles
    game->carte->head_map->next->next->next->next->next->next->tiles[5].type='#';
    game->carte->head_map->next->next->next->next->next->next->tiles[7].type='#';
    game->carte->head_map->next->next->next->next->next->next->tiles[9].type='#';
    for(int i=0; i<LENGTH; i++){
        game->carte->head_map->next->next->next->tiles[i].type='~';
    }
    //fin de la zone de changement de map

    //init_display(place, joueur);
    srand(time(NULL));
    display_map(game->carte, game->joueur, renderer);
    //affiche le joueur
    SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = {.x = distance_x*32*ECHELLE, .y = distance_y*32*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
    }
    
    //push l'affichage
    SDL_RenderPresent(renderer);

    while(!quit){

        //regarde si il y a des evenements clavier
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT){
            printf("je rentre ici\n");
            quit = SDL_TRUE;
        }else if(event.type == SDL_KEYDOWN){
            //printf("%d\n", offset);
            //if(event.key.keysym.scancode == SDL_SCANCODE_A){
            //    printf("aaaaaaaaaaaaaaaaaaa\n");
            //}
            if(event.key.keysym.sym == SDLK_z){
                //distance_y -= 1;
                move_up(game);
                offset_y = 1;
                offset_x += 1;
                offset_x %= 4;
                //printf("zzzzzzzzzzzzz\nzzzzzzzzzzzzz\nzzzzzzzzzzzzz\n");
            }else if(event.key.keysym.sym == SDLK_q){
                //distance_x -= 1;
                move_left(game);
                offset_y = 3;
                offset_x += 1;
                offset_x %= 4;
                //printf("qqqqqqqqqqqqq\nqqqqqqqqqqqqq\nqqqqqqqqqqqqq\n");
            }else if(event.key.keysym.sym == SDLK_s){
                //distance_y += 1;
                move_down(game);
                offset_y = 0;
                offset_x += 1;
                offset_x %= 4;
                //printf("sssssssssssss\nsssssssssssss\nsssssssssssss\n");
            }else if(event.key.keysym.sym == SDLK_d){
                //distance_x += 1;
                move_right(game);
                offset_y = 2;
                offset_x += 1;
                offset_x %= 4;
                //printf("ddddddddddddd\nddddddddddddd\nddddddddddddd\n");
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        //SDL_Color const BACKGROUND_COLOR = {.r = 0x00, .g = 0x00, .b = 0x00, .a = SDL_ALPHA_OPAQUE};
        //if(SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
        //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur pour la couleur de fond : %s", SDL_GetError());
        //}
        if(SDL_RenderClear(renderer)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        }
        SDL_Rect spriteRect = {.x = offset_x*spriteWidth, .y = offset_y*spriteWidth, .w = spriteWidth, .h = spriteHeight};
        //SDL_Rect destRect = {.x = (game->joueur->x+distance_x)*32*ECHELLE, .y = ((HEIGHT-game->joueur->y)+distance_y)*32*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
        SDL_Rect destRect = {.x = (distance_x+game->joueur->x)*32*ECHELLE, .y = (distance_y-game->joueur->y)*32*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
        display_map(game->carte, game->joueur, renderer);
        if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect))
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        SDL_RenderPresent(renderer);
        }
        SDL_Delay(20);
    //update_display(place, joueur);
    }

    destroy_game(game);
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
