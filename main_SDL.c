#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "ia.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define ECHELLE 2
#define IA true

void display_map(Map *map, SDL_Renderer *renderer,int pix_defil){
    //parcours toute la map ligne par ligne et charge le sprite correspondant
    Line *line = map->head_map;
    for (int i = 0; i<HEIGTH+map->tail_buffer_lenght;i++){
        for (int j = 0; j<LENGTH; j++){
            SDL_Surface *spriteSurface;
            if(line->tiles[j].type==_PLAIN){              //charge le sprite a afficher
                spriteSurface = IMG_Load("sprites/Plaines.png");
            }else if(line->tiles[j].type==_ROAD){
                spriteSurface = IMG_Load("sprites/Routes.png");
            }else if(line->tiles[j].type==_PLAIN_BLOCK){
                spriteSurface = IMG_Load("sprites/Plaines.png");
            }else if(line->tiles[j].type==_ROAD_UP){
                spriteSurface = IMG_Load("sprites/RoutesLimiteHaut.png");
            }else if(line->tiles[j].type==_ROAD_DOWN){
                spriteSurface = IMG_Load("sprites/RouteLlimiteBas.png");
            }else if(line->tiles[j].type==_ROAD_MIDDLE){
                spriteSurface = IMG_Load("sprites/RouteCentre.png");
            }else if(line->tiles[j].type==_WALL){
                spriteSurface = IMG_Load("sprites/Rochers.png");
            }else if(line->tiles[j].type==_WATER){   //(line->tiles[j].type=='~')
                spriteSurface = IMG_Load("sprites/Eaudoublebords.png");
            }else if(line->tiles[j].type==_WATER_UP){   //(line->tiles[j].type=='~')
                spriteSurface = IMG_Load("sprites/Eaubordhaut.png");
            }else if(line->tiles[j].type==_WATER_DOWN){   //(line->tiles[j].type=='~')
                spriteSurface = IMG_Load("sprites/Eaubordbas.png");
            }else{ //if(line->tiles[j].type==_WATER_MIDDLE){   //(line->tiles[j].type=='~')
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
            SDL_Rect destRect = {.x = 32*j*ECHELLE, .y = (32*(i)-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE}; //
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        }
        if(line!=NULL){
            line=line->next;
        }
    }
}

void display_joueur(Player *joueur, SDL_Renderer *renderer,int pix_defil){
    // affiche le joueur selon le même principe que ci-dessus
    SDL_Surface *spriteSurface = IMG_Load("sprites/Snitch.png");
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
    SDL_Rect destRect = {.x = (joueur->_x)*ECHELLE, .y = (HEIGTH*32-joueur->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
    }
    SDL_DestroyTexture(spriteTexture);
    SDL_FreeSurface(spriteSurface);
}

void display_mob(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil){
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
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y = (liste->mob->id_mob-CAR_RIGHT)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
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
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        } else if(liste->mob->id_mob==CANON_RIGHT || liste->mob->id_mob==CANON_LEFT){
            SDL_Surface *spriteSurface = IMG_Load("sprites/Canon.png");
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
            int spriteWidth = spriteFullWidth/3; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->id_mob-CANON_RIGHT)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        } else if(liste->mob->id_mob==CANON_BALL_RIGHT || liste->mob->id_mob==CANON_BALL_LEFT){
            SDL_Surface *spriteSurface = IMG_Load("sprites/Boulet.png");
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
            int spriteWidth = spriteFullWidth/3; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->id_mob-CANON_BALL_RIGHT)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
            SDL_FreeSurface(spriteSurface);
        }
        liste = liste->next;
    }
}


void display_all(Game *game, SDL_Renderer *renderer, int pix_defil){
    // affiche toute la carte dans l'ordre, map puis mobs puis joueur
    // if(SDL_RenderClear(renderer)){
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
    // }
    display_map(game->carte, renderer,pix_defil);
    display_mob(game->mobs, renderer,pix_defil);
    display_joueur(game->joueur, renderer,pix_defil);
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
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE), 0);
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

    Game *game = initialise_game();
    printf("%d %d\n", game->joueur->_x/32, game->joueur->_y/32);
    // Line *line = game->carte->head_map;
    // while(line!=NULL){
    //     for(int i=0; i<HEIGHT; i++){
    //         printf("%c", line->tiles[i].type);
    //     }
    //     printf("\n");
    //     line=line->next;
    // }
    int pix_defil = 0;
    int counter = 0;
    //update_game(game);

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
        if(pix_defil){
            if(pix_defil>13*32){
                pix_defil -= 32;
            }else if(pix_defil>10*32){
                pix_defil -= 16;
            }else if(pix_defil>5*32){
                pix_defil -= 8;
            }else{
                pix_defil -= 4;
            }
        }else{
            counter += 1;
        }
        if(!pix_defil){
            destroy_tail_buffer(game->carte);
        }else if(!pix_defil%32){
            destroy_last_buffer(game->carte);
        }
        // test la mort, pour le moment juste quitte le jeu
        //if(test_death(game)){
        //    if(SDL_RenderClear(renderer)){
        //         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        //      }
        //   quit = SDL_TRUE;
        // }
        //if(SDL_RenderClear(renderer)){
        //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        //}    
        display_all(game, renderer,pix_defil);
        
        //si le joueur est une IA
        if(IA){
            if(game->joueur->delai){
                game->joueur->delai--;
            }else{
                eval(game);
                for(int i=0; i<5; i++){
                    printf("directive :%d; score : %d\n", i, *(game->joueur->score+i));
                }
                printf("\n");
                SDL_Delay(100);
                int directive = max_score(game->joueur->score);
                if(directive == 0){
                    if(move_up(game)&&game->joueur->_y_dest>=3*32){
                        pix_defil += 32;
                        counter = 0;
                        switch_game(game);
                        game->mobs = destroy_liste_mob_from(-pix_defil/32-1, game->mobs);
                    }
                    //sert à changement l'image du joueur quand il se déplace
                    game->joueur->offset_y = 1;
                    game->joueur->offset_x += 1;
                    game->joueur->offset_x %= 4;
                }else if(directive == 1){
                    move_right(game);
                    game->joueur->offset_y = 2;
                    game->joueur->offset_x += 1;
                    game->joueur->offset_x %= 4;
                }else if(directive == 2){
                    move_down(game);
                    game->joueur->offset_y = 0;
                    game->joueur->offset_x += 1;
                    game->joueur->offset_x %= 4;
                }else if(directive == 3){
                    move_left(game);
                    game->joueur->offset_y = 3;
                    game->joueur->offset_x += 1;
                    game->joueur->offset_x %= 4;
                }
                game->joueur->delai = 25;
            }
        }else{
            // une fois l'affichage fait, on regarde si le joueur se déplace
            SDL_PollEvent(&event);
            //regarde si il y a des evenements clavier
            if(event.type == SDL_QUIT){
                printf("je rentre ici\n");
                quit = SDL_TRUE;
            }else if(event.type == SDL_KEYUP){
                // bouge le joueur dans la direction selon la touche pressée, ZQSD, ou quitte avec M
                if(event.key.keysym.sym == SDLK_z){
                    // si le joueur avance de plus de 3 cases, on avance la carte, sinon c'est qu'il avait reculé donc on avance pas toute la map
                    if(move_up(game)&&game->joueur->_y_dest>=3*32){
                        pix_defil += 32;
                        counter = 0;
                        switch_game(game);
                        game->mobs = destroy_liste_mob_from(-pix_defil/32-1, game->mobs);
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
        }
        // update_game(game);
        // // test la mort, pour le moment juste quitte le jeu
        // if(test_death(game)){
        //     if(SDL_RenderClear(renderer)){
        //         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        //     }
        //     quit = SDL_TRUE;
        // }
        // if(SDL_RenderClear(renderer)){
        //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
        // }    
        //display_all(game, renderer, defil, baaaaaa);
        //if(baaaaaa){
        //    for(int defil=0; defil<8; defil++){
        //        switch_game(game, defil);
        //        update_game(game);
        //        display_all(game, renderer, defil, baaaaaa);
        //        //SDL_Delay(1000);
        //    }
        //    baaaaaa = false;    
        //}
        //SDL_Delay(50);
    }
    destroy_game(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
