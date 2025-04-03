//#include <SDL2/SDL.h>
//#include <SDL2/SDL_timer.h>
//#include <SDL2/SDL_image.h>
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

void init_display(Map *map, Player *joueur){
    Line *line = map->head_map;
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

void update_display(Map* map, Player *joueur){
    srand(time(NULL));
    Line *line = map->head_map;
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

int main(void){
    struct termios old_tio = init_terminal();
    Map* place = create_empty_map();
    Player *joueur = malloc(sizeof(Player));
    joueur->x = LENGTH/2;
    joueur->y = HEIGTH-3;
    init_display(place, joueur);
    srand(time(NULL));
    char ch;
    bool quit = false;
    while(!quit){
        read(STDIN_FILENO, &ch, 1);
        //printf("Key pressed: %c\n", ch);
        if (ch == 'm') {
            quit = true;
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
    }
    free(joueur);
    restore_terminal(&old_tio);
    return 0;
}
