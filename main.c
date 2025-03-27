#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "mob.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>


struct termios init_terminal(void){
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    return old_tio;
}

void restore_terminal(struct termios* settings){
    tcsetattr(STDIN_FILENO, TCSANOW, settings);
}

void init_display(Map *map, player *joueur){
    Line *line = map->head;
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            if((i==joueur->y) && (j==joueur->x)){
                printf(" ");
            }else{
                printf("%c", line->tiles[j].type);
            }
        }
        printf("\n");
        if(i!=LENGTH){
            line=line->next;
        }
    }
}

void update_display(Map* map, player *joueur){
    srand(time(NULL));
    Line *line = map->head;
    printf("\033[%dA\r", HEIGTH);
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            if((i==joueur->y) && (j==joueur->x)){
                printf(" ");
            }else{
                printf("\033[32m%c",line->tiles[j].type);
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

int main(int argc, char *argv[]){
    struct termios old_tio = init_terminal();
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("ca marche po\n");
    }
    
    //SDL_Window *win = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    Map* place = create_empty_map();
    player *joueur = malloc(sizeof(player));
    joueur->x = LENGTH/2;
    joueur->y = HEIGTH-3;
    init_display(place, joueur);
    srand(time(NULL));
    char ch;
    while(!quit){
        read(STDIN_FILENO, &ch, 1);
        //printf("Key pressed: %c\n", ch);
        if (ch == 'm') {
            quit = SDL_TRUE;
        }
        if (ch == 'z') {
            joueur->y--;
            update_display(place, joueur);
        }
        if (ch == 'q') {
            joueur->x--;
            update_display(place, joueur);
        }
        if (ch == 's') {
            joueur->y++;
            update_display(place, joueur);
        }
        if (ch == 'd') {
            joueur->x++;
            update_display(place, joueur);
        }
        update_display(place, joueur);
        sleep(0.5);
    }
    free(joueur);
    restore_terminal(&old_tio);

        //update_display(place);
    //    for(int i=0; i<LENGTH; i++){
    //        for(int j=0; j<HEIGTH; j++){
    //            if((i, j) not in dico){
    //                printf("#");
    //            }else{
    //                printf("%c", tile_map(j, i, place));
    //            }
    //        }
    //        printf("\n");
    //    }
    //    update_display(place);
    //    //regarde si il y a des evenements clavier
        //SDL_WaitEvent(&event);
        //if(event.type == SDL_QUIT){
        //    printf("je rentre ici\n");
        //    quit = SDL_TRUE;
        //}else if(event.type == SDL_KEYDOWN){
        //    //if(event.key.keysym.scancode == SDL_SCANCODE_A){
        //    //    printf("aaaaaaaaaaaaaaaaaaa\n");
        //    //}
        //    if(event.key.keysym.sym == SDLK_z){
        //        //moveup(joueur, place);
        //        printf("zzzzzzzzzzzzz\nzzzzzzzzzzzzz\nzzzzzzzzzzzzz\n");
        //    }else if(event.key.keysym.sym == SDLK_q){
        //        //movedown(joueur, place);
        //        printf("qqqqqqqqqqqqq\nqqqqqqqqqqqqq\nqqqqqqqqqqqqq\n");
        //    }else if(event.key.keysym.sym == SDLK_s){
        //        //moveleft(joueur, place);
        //        printf("sssssssssssss\nsssssssssssss\nsssssssssssss\n");
        //    }else if(event.key.keysym.sym == SDLK_d){
        //        //move_right(joueur, place);
        //        printf("ddddddddddddd\nddddddddddddd\nddddddddddddd\n");
        //    }else if(event.key.keysym.sym == SDLK_m){
        //        quit = SDL_TRUE;
        //    }
        //}
        //SDL_Delay(20);
        //fflush(stdout);
    //    //forobstacles{
    //    //    applique traitement
    //    //}
    //    update_display(place);
    //    if(test_death_d(joueur, place)){
    //        //fin de partie
    //    }
    //    else{
    //        map_shift_d(place, score);
    //    }
//
    //    
    //    //place = initialize_map();
    //    //int i = rand()%16;
    //    //int j = rand()%16;
    //    //TYPE(tile_map(j, i, place)) = ' ';
    //    //update_display(place);
    //    //ch = getch();
    //    //printf("%d", ch);
    //    sleep(1);
    
    return 0;
}
