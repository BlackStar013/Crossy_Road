#include "PrototypeRoutes.h"
#include "dico.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void init_display(void){
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            printf("#");
        }
        printf("\n");
    }
}

void update_display(map* place){
    srand(time(NULL));
    printf("\033[%dA\r", HEIGTH);
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            printf("\033[32m%c",((tile_map(j, i, place))->type));
        }
        printf("\n");
    }
    fflush(stdout);
}

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("ca marche po\n");
    }
    SDL_Window *win = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1, 1, 0);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    map* place = initialize_map_dico();
    init_display();
    srand(time(NULL));
    //int ch = getch();
    while(!quit){
        for(int i=0; i<LENGTH; i++){
            for(int j=0; j<HEIGTH; j++){
                if((i, j) not in dico){
                    printf("#");
                }else{
                    printf("%c", tile_map(j, i, place));
                }
            }
            printf("\n");
        }
        update_display(place);
        //regarde si il y a des evenements clavier
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT){
            printf("je rentre ici");
            quit = SDL_TRUE;
        }else if(event.type == SDL_KEYDOWN){
            //if(event.key.keysym.scancode == SDL_SCANCODE_A){
            //    printf("aaaaaaaaaaaaaaaaaaa\n");
            //}
            if(event.key.keysym.sym == SDLK_z){
                moveup(joueur, place);
                printf("zzzzzzzzzzzzz\nzzzzzzzzzzzzz\nzzzzzzzzzzzzz\n");
            }else if(event.key.keysym.sym == SDLK_q){
                movedown(joueur, place);
                printf("qqqqqqqqqqqqq\nqqqqqqqqqqqqq\nqqqqqqqqqqqqq\n");
            }else if(event.key.keysym.sym == SDLK_s){
                moveleft(joueur, place);
                printf("sssssssssssss\nsssssssssssss\nsssssssssssss\n");
            }else if(event.key.keysym.sym == SDLK_d){
                move_right(joueur, place);
                printf("ddddddddddddd\nddddddddddddd\nddddddddddddd\n");
            }else if(event.key.keysym.sym == SDLK_m){
                quit = SDL_TRUE;
            }
        }
        SDL_Delay(20);
        //forobstacles{
        //    applique traitement
        //}
        update_display(place);
        if(test_death_d(joueur, place)){
            //fin de partie
        }
        else{
            map_shift_d(place, score);
        }

        
        //place = initialize_map();
        //int i = rand()%16;
        //int j = rand()%16;
        //TYPE(tile_map(j, i, place)) = ' ';
        //update_display(place);
        //ch = getch();
        //printf("%d", ch);
        sleep(1);
    return 0;
    }
}