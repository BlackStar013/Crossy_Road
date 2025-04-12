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

void display_map(Map *map, SDL_Renderer *renderer){
    //parcours toute la map ligne par ligne et charge le sprite correspondant
    Line *line = map->head_map;
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            SDL_Surface *spriteSurface;
            if((line->tiles[j].type=='_') && (line->tiles[j].color=='g')){              //charge le sprite a afficher
                spriteSurface = IMG_Load("sprites/Plaines.png");
            }else if(line->tiles[j].type=='_'){
                spriteSurface = IMG_Load("sprites/Routes.png");
            }else if(line->tiles[j].type=='#'){
                spriteSurface = IMG_Load("sprites/Rochers.png");
            }else{   //(line->tiles[j].type=='~')
                spriteSurface = IMG_Load("sprites/Eaumilieu.png");
            }
            // creer la surface pour afficher
            if (!spriteSurface) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            //creer la textire avec la surface et le sprite
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) { 
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            // spriteWidth correspond à la largeur du fichier, ici je me sers de la taille des cases simples pour l'échelle, donc pas de division ni produit
            int spriteWidth = spriteFullWidth;   //sinon (spriteFullWidth / nbSpritesInSprite)*ECHELLE
            int spriteHeight = spriteFullHeight;
            // recupère dans le fichier la position depuis en haut à gauche (d'ou le (0, 0)pour x et y) et la taille calculée au dessus
            SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
            // x et y correspondent à la position où on va l'afficher, et w et h sa taille
            SDL_Rect destRect = {.x = 32*j*ECHELLE, .y = 32*i*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        }
        if(i!=LENGTH){
            line=line->next;
        }
    }
}

void display_joueur(Player *joueur, SDL_Renderer *renderer){
    // affiche le joueur selon le même principe que ci-dessus
    SDL_Surface *spriteSurface = IMG_Load("sprites/Kairon.png");
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
    int spriteWidth = (spriteFullWidth / 4 ); //ici pour le joueur, on a 4 lignes et 4 colonnes pour les 4 orientations et 4 "gestes" d'où le /4
    int spriteHeight = (spriteFullHeight / 4 );
    SDL_Rect spriteRect = {.x = joueur->offset_x*spriteWidth, .y = joueur->offset_y*spriteWidth, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = {.x = (joueur->_x)*ECHELLE, .y = (HEIGHT*32-joueur->_y-32)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
    }
    SDL_DestroyTexture(spriteTexture);
    SDL_FreeSurface(spriteSurface);
}

void display_mob(Liste_Mob *liste, SDL_Renderer *renderer){
    // affiche les mobs un par un selon le même principe que ci-ci-dessus-dessus
    while(liste!=NULL){
        if(liste->mob->id_mob==CAR_RIGHT || liste->mob->id_mob==CAR_LEFT){
            SDL_Surface *spriteSurface = IMG_Load("sprites/Chariot.png");
            if (!spriteSurface){
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
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = (spriteFullWidth / 2); // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = (spriteFullHeight / 2);
            SDL_Rect spriteRect = {.x = 0, .y = liste->mob->id_mob*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGHT*32-liste->mob->_y-32)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        }else if(liste->mob->id_mob==PLANK_RIGHT || liste->mob->id_mob==PLANK_LEFT){
            SDL_Surface *spriteSurface = IMG_Load("sprites/Planche.png");
            if (!spriteSurface){
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
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight;
            SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGHT*32-liste->mob->_y-32)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        }
        liste = liste->next;
    }
}

void display_all(Game *game, SDL_Renderer *renderer){
    // affiche toute la carte dans l'ordre, map puis mobs puis joueur
    if(SDL_RenderClear(renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
    }
    display_map(game->carte, renderer);
    display_mob(game->mobs, renderer);
    display_joueur(game->joueur, renderer);
    SDL_RenderPresent(renderer);
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
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (LENGHT*32*ECHELLE), (HEIGHT*32*ECHELLE), 0);
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

    // permet de choisir un arrière plan, mais pas utile là puis on recouvre toute la fenetre
    //SDL_Color const BACKGROUND_COLOR = {.r=0x00, .g=0x00, .b=0x00, .a=SDL_ALPHA_OPAQUE};
    //if(SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
    //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur pour la couleur de fond : %s", SDL_GetError());
    //}

    //vide la fenetre
    if(SDL_RenderClear(renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
    }

    Game *game = initalise_game();
    // Line *line = game->carte->head_map;
    // while(line!=NULL){
    //     for(int i=0; i<HEIGHT; i++){
    //         printf("%c", line->tiles[i].type);
    //     }
    //     printf("\n");
    //     line=line->next;
    // }
    update_game(game);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    //pour tester le chargement des diffrentes tuiles
    // for(int i=0; i<LENGTH; i++){
    //     game->carte->head_map->next->next->next->tiles[i].type='~';
    //     game->carte->head_map->next->next->next->tiles[i].is_danger=true;
    // }
    // game->carte->head_map->next->next->next->tiles[7].type='_';
    // game->carte->head_map->next->next->next->tiles[7].is_danger=false;
    //game->mobs = list_append(game->mobs, clone_mob(car_left, 3, 5, 10));
    //fin de la zone de changement de map

    //a servi pour vérifier que la map s'affichait bien
    // Line *line = game->carte->head_map;
    // while(line!=NULL){
    //     for(int i=0; i<HEIGHT; i++){
    //         printf("%c", line->tiles[i].type);
    //     }
    //     printf("\n");
    //     line=line->next;
    // }
    // printf("\n");
    while(!quit){
        // printf("score :%d\n", game->score);
        update_game(game);
        // test la mort, pour le moment juste quitte le jeu
        //if(test_death(game)){
        //    if(SDL_RenderClear(renderer)){
        //        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        //    }
        //    quit = SDL_TRUE;
        //}
        if(SDL_RenderClear(renderer)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        }    
        display_all(game, renderer);

        // une fois l'affichage fait, on regarde si le joueur se déplace
        SDL_PollEvent(&event);
        //regarde si il y a des evenements clavier
        if(event.type == SDL_QUIT){
            printf("je rentre ici\n");
            quit = SDL_TRUE;
        }else if(event.type == SDL_KEYUP){
            // bouge le joueur dans la direction selon la touche pressée, ZQSD, ou quitte avec M
            if(event.key.keysym.sym == SDLK_z){
                move_up(game);
                // si le joueur avance de plus de 3 cases, on avance la carte, sinon c'est qu'il avait reculé donc on avance pas toute la map
                if(game->joueur->_y>=3*32){
                    switch_game(game);
                }
                //sert à changement l'image du joueur quand il se déplace
                game->joueur->offset_y = 1;
                game->joueur->offset_x += 1;
                game->joueur->offset_x %= 4;
            }else if(event.key.keysym.sym == SDLK_q){
                move_left(game);
                game->joueur->offset_y = 3;
                game->joueur->offset_x += 1;
                game->joueur->offset_x %= 4;
            }else if(event.key.keysym.sym == SDLK_s){
                move_down(game);
                game->joueur->offset_y = 0;
                game->joueur->offset_x += 1;
                game->joueur->offset_x %= 4;
            }else if(event.key.keysym.sym == SDLK_d){
                move_right(game);
                game->joueur->offset_y = 2;
                game->joueur->offset_x += 1;
                game->joueur->offset_x %= 4;
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        }
        //SDL_Delay(50);
    }
    destroy_game(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


