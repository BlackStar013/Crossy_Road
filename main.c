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
            if((HEIGHT-i-1==game->joueur->y) && (j==game->joueur->x)){
                printf(" ");
            }else{
                Liste_Mob *liste = game->mobs;
                bool trouve = false;
                while((!trouve) && (liste!=NULL)){
                    if((HEIGHT-i-1==liste->mob->y) && (j==liste->mob->x)){
                        // for(int k=0; k<liste->mob->length){
                        //     printf("%d", liste->mob->id_mob);

                        // }
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
            if((HEIGHT-i-1==game->joueur->y) && (j==game->joueur->x)){
                printf(" ");
            }else{
                Liste_Mob *liste = game->mobs;
                bool trouve = false;
                while((!trouve) && (liste!=NULL)){
                    if((HEIGHT-i-1==liste->mob->y) && (j==liste->mob->x)){
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
    update_game(game);
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
        update_game(game);
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
            if(game->joueur->y>=3){
                switch_game(game);
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
