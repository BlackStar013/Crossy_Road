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

void init_display(Game *game){
    Line *line = game->carte->head_map;
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            if((HEIGHT-i-1==game->joueur->_y*32) && (j==game->joueur->_x*32)){
                printf(" ");
            }else{
                Liste_Mob *liste = game->mobs;
                bool trouve = false;
                while((!trouve) && (liste!=NULL)){
                    if(((HEIGHT-i-1)*32==liste->mob->_y) && (j*32==liste->mob->_x)){
                        printf("%d", liste->mob->id_mob);
                        trouve = true;
                    }else{
                        liste=liste->next;
                    }
                }
                if(!trouve){
                    printf("%c", line->tiles[j].type);
                }
            }
        }
        printf("\n");
        if(i!=LENGTH){
            line=line->next;
        }
    }
}

void update_display(Game *game){
    srand(time(NULL));
    Line *line = game->carte->head_map;
    printf("\033[%dA\r", HEIGTH);
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            if((HEIGHT-i-1==game->joueur->_y*32) && (j==game->joueur->_x*32)){
                printf(" ");
            }else{
                Liste_Mob *liste = game->mobs;
                bool trouve = false;
                while((!trouve) && (liste!=NULL)){
                    if(((HEIGHT-i-1)*32==liste->mob->_y) && (j*32==liste->mob->_x)){
                        printf("%d", liste->mob->id_mob);
                        trouve = true;
                    }else{
                        liste=liste->next;
                    }
                }
                if(!trouve){
                    printf("%c", line->tiles[j].type);
                }
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
    update_game(game,0);
    game->mobs = list_append(game->mobs, clone_mob(car_right, 3, 5, 10));
    //print_liste(game->mobs);
    //sleep(1);
    //Map* place = create_empty_map();
    //Player *joueur = malloc(sizeof(Player));
    //joueur->x = LENGTH/2;
    //joueur->y = HEIGTH-3;
    init_display(game);
    srand(time(NULL));
    char ch;
    bool quit = false;
    while(!quit){
        update_game(game,0);
        if(test_death(game)){
            quit = true;
        }
        update_display(game);
        read(STDIN_FILENO, &ch, 1);
        //printf("Key pressed: %c\n", ch);
        if(ch == 'm'){
            quit = true;
        }else if(ch == 'z'){
            move_up(game);
            if(game->joueur->_y>=3*32){
                switch_game(game,0);
            }
            //joueur->y--;
            //update_display(place, joueur);
        }else if(ch == 'q'){
            move_left(game);
            //joueur->x--;
            //update_display(place, joueur);
        }else if(ch == 's'){
            move_down(game);
            //joueur->y++;
            //update_display(place, joueur);
        }else if(ch == 'd'){
            move_right(game);
            //joueur->x++;
            //update_display(place, joueur);
        }
    //printf("%d %d\n", game->joueur->x, game->joueur->y);
    //sleep(3);
    }
    //free(joueur);
    //destroy_game(game);
    restore_terminal(&old_tio);
    return 0;
}
