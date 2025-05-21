#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "ia.h"
#include "core.h"
#include "menu.h"
#include "mob.h"
#include "SDLdisplay.h"
#include "display.h"
#include "save_load.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    // Load font
    TTF_Font* font;
    font = TTF_OpenFont("font/PixelPurl.ttf", 24); // Replace with the path to your font file
    if (font == NULL) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return 1;
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

    SDL_Surface** surfaceArrayMap = initSurfaceArrayMap();
    SDL_Surface** surfaceArrayMob = initSurfaceArrayMob();
    SDL_Surface** surfaceExp= initSurfaceExp();
    SDL_Surface** surfaceArrayProjectile = initSurfaceArrayProjectile();
    SDL_Surface** surfaceArrayPlayer = initSurfaceArrayPlayer();

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
    SDL_bool main_menu = SDL_TRUE;
    SDL_bool pause = SDL_FALSE;
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
    const int desired_fps = 64;
    const int frameDelay = 1000 / desired_fps;
    int fps = 0;
    int cursor = 0;
    int chara = AESIR;
    int highscore;
    bool ia = false;
    Game* game = initialise_game(chara,0);
    load(&(game->nb_exp), &highscore);
    printf("exp: %d,highscore: %d\n", game->nb_exp, highscore);
    Uint32 startTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 endframeTime;
    Uint32 elapsedTime;
    while(!quit){
        displayMainMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE));
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            quit = SDL_TRUE;
        }else if(event.type == SDL_KEYUP){
            if((event.key.keysym.sym == SDLK_z) || (event.key.keysym.sym == SDLK_UP)){
                cursor = (cursor-1)%NB_MMOPTION;
                if(cursor < 0){
                    cursor = NB_MMOPTION-1;
                }
            }else if((event.key.keysym.sym == SDLK_s) || (event.key.keysym.sym == SDLK_DOWN)){
                cursor = (cursor+1)%NB_MMOPTION;
            }else if((event.key.keysym.sym == SDLK_SPACE) || (event.key.keysym.sym == SDLK_RETURN)){
                if(cursor == START){
                    main_menu = SDL_FALSE;
                }
                if(cursor == CHANGECHARA){
                    charaMenu(&quit, &chara, renderer, &event, font);
                    if(chara == LEDE){
                        ia = true;
                    }
                    else{
                        ia = false;
                    }
                }
                if(cursor == MMOPTIONS){
                    //printf("maybe later");
                }
                if(cursor == MMQUIT){
                    quit = SDL_TRUE;
                }
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        }
        if(!main_menu){
            destroy_game(game);
            game = initialise_game(chara,0);
        }
        while(!quit && !main_menu){
            cursor = 0;
            currentTime = SDL_GetTicks();
            elapsedTime = currentTime-startTime;
            //printf("elapsedTime = %d", elapsedTime);
            if(elapsedTime < 1000){
                fps+=1;
            }
            else{
                //printf("fps: %d\n", fps);
                fps = 0;
                startTime = SDL_GetTicks();
            }
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
            if(test_death(game)){
                if(SDL_RenderClear(renderer)){
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
                }
                gameOverMenu(&main_menu, &quit, &game, renderer, &event, font, chara);
                //quit = SDL_TRUE;
            }
            if(SDL_RenderClear(renderer)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
            }
            display_all(game, renderer,pix_defil, surfaceArrayMap, surfaceArrayMob, surfaceArrayPlayer, surfaceArrayProjectile, surfaceExp,font);

            // une fois l'affichage fait, on regarde si le joueur se déplace
            if(ia){
                if(game->joueur->delai){
                    game->joueur->delai--;
                }else{ 

                    int directive = eval(game);
                    // printf("directive %d\n", directive);
                    if(directive == 0){
                        if(move_up(game)&&game->joueur->_y_dest>=3*32){
                            pix_defil += 32;
                            counter = 0;
                            switch_game(game);
                            game->mobs = destroy_liste_mob_from(-pix_defil/32-1, game->mobs);
                            game->exp = destroy_liste_mob_from(-pix_defil/32-1, game->exp);
                            game->projectile = destroy_liste_mob_from(-pix_defil/32-2*HEIGTH, game->projectile);
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
                    game->joueur->delai = DELAI;
                    // SDL_Delay(3000);
                }
            }else{
                SDL_PollEvent(&event);
                //regarde si il y a des evenements clavier
                if(event.type == SDL_QUIT){
                    printf("je rentre ici\n");
                    quit = SDL_TRUE;
                }else if(event.type == SDL_KEYUP){
                    // bouge le joueur dans la direction selon la touche pressée, ZQSD, ou quitte avec M
                    if((event.key.keysym.sym == SDLK_z) || (event.key.keysym.sym == SDLK_UP)){
                        // si le joueur avance de plus de 3 cases, on avance la carte, sinon c'est qu'il avait reculé donc on avance pas toute la map
                        if(move_up(game)&&game->joueur->_y_dest>=3*32){
                            pix_defil += 32;
                            counter = 0;
                            switch_game(game);
                            game->mobs = destroy_liste_mob_from(-pix_defil/32-1, game->mobs);
                            game->exp = destroy_liste_mob_from(-pix_defil/32-1, game->exp);
                            game->projectile = destroy_liste_mob_from(-pix_defil/32-2*HEIGTH, game->projectile);
                        }
                        //sert à changement l'image du joueur quand il se déplace
                        game->joueur->offset_y = 1;
                        game->joueur->offset_x += 1;
                        game->joueur->offset_x %= 4;
                    }else if((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_LEFT)){
                        move_left(game);
                        game->joueur->offset_y = 3;
                        game->joueur->offset_x += 1;
                        game->joueur->offset_x %= 4;
                    }else if((event.key.keysym.sym == SDLK_s) || (event.key.keysym.sym == SDLK_DOWN)){
                        move_down(game);
                        game->joueur->offset_y = 0;
                        game->joueur->offset_x += 1;
                        game->joueur->offset_x %= 4;
                    }else if((event.key.keysym.sym == SDLK_d) || (event.key.keysym.sym == SDLK_RIGHT)){
                        move_right(game);
                        game->joueur->offset_y = 2;
                        game->joueur->offset_x += 1;
                        game->joueur->offset_x %= 4;
                    }else if(event.key.keysym.sym == SDLK_SPACE){
                        use_power(game);
                    }else if(event.key.keysym.sym == SDLK_TAB){
                        pauseMenu(&main_menu, &quit, renderer, &event, font, game, pix_defil, surfaceArrayMap, surfaceArrayMob, surfaceArrayPlayer, surfaceArrayProjectile, surfaceExp);
                    }else if(event.key.keysym.sym == SDLK_m){
                        quit = SDL_TRUE;
                    }
                }
            }
            endframeTime = SDL_GetTicks();
            if (frameDelay > (endframeTime-currentTime)) {
                SDL_Delay(frameDelay - (endframeTime-currentTime));
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
        if((game->score > highscore) && (!IA)){
            save(game->nb_exp, game->score);
        }
        else{
            save(game->nb_exp, highscore);
        }
    }
    destroy_game(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    freeSurfaceArrayMap(surfaceArrayMap);
    freeSurfaceArrayMob(surfaceArrayMob);
    freeSurfaceArrayPlayer(surfaceArrayPlayer);
    freeSurfaceArrayProjectile(surfaceArrayProjectile);
    freeSurfaceExp(surfaceExp);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
