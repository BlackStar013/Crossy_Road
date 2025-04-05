//#include <SDL2/SDL.h>
//#include <SDL2/SDL_timer.h>
//#include <SDL2/SDL_image.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
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
            if((HEIGHT-i-1==joueur->y) && (j==joueur->x)){
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
            if((HEIGHT-i-1==joueur->y) && (j==joueur->x)){
                printf(" ");
            }else{
                printf("\033[32m%c",line->tiles[j].type);
            }
        }
        printf("\n");
        if(i!=LENGTH){
            line=line->next;
        }
    }
    //printf("%d %d\n", joueur->x, joueur->y);
    fflush(stdout);
}

int main(void){
    struct termios old_tio = init_terminal();
    Game *game = initalise_game();
    game->carte->head_map->next->next->next->next->next->tiles[5].type='#';
    game->carte->head_map->next->next->next->next->next->tiles[5].is_block = true;
    game->carte->head_map->next->next->next->next->next->tiles[7].type='#';
    game->carte->head_map->next->next->next->next->next->tiles[7].is_block = true;
    game->carte->head_map->next->next->next->next->next->tiles[9].type='#';
    game->carte->head_map->next->next->next->next->next->tiles[9].is_block = true;
    for(int i=0; i<LENGTH; i++){
        game->carte->head_map->next->next->next->tiles[i].type='~';
        game->carte->head_map->next->next->next->tiles[i].is_danger=true;
    }
    update_game(game);
    //Map* place = create_empty_map();
    //Player *joueur = malloc(sizeof(Player));
    //joueur->x = LENGTH/2;
    //joueur->y = HEIGTH-3;
    init_display(game->carte, game->joueur);
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
            move_up(game);
            if(game->joueur->y>=3){
                switch_game(game);
            }
            //joueur->y--;
            //update_display(place, joueur);
        }
        if (ch == 'q') {
            move_left(game);
            //joueur->x--;
            //update_display(place, joueur);
        }
        if (ch == 's') {
            move_down(game);
            //joueur->y++;
            //update_display(place, joueur);
        }
        if (ch == 'd') {
            move_right(game);
            //joueur->x++;
            //update_display(place, joueur);
        }
    //printf("%d %d\n", game->joueur->x, game->joueur->y);
    //sleep(3);
    update_game(game);
    update_display(game->carte, game->joueur);
    }
    //free(joueur);
    destroy_game(game);
    restore_terminal(&old_tio);
    return 0;
}
