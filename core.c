#include "map.h"
#include "core.h"
#include "mob.h"
#include "tuiles.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

////Partie Création d'obstacles !///

int* initialise_proba(void){
    int* proba = malloc((NB_OBSTACLE)*sizeof(int));
    for(int i = 0; i<NB_OBSTACLE; i++){
        *(proba+i) = 0;
    }
    *(proba+FOREST) = 50;
    *(proba+ROAD) = 50;
    *(proba+RIVER) = 0;
    return proba;
}

void generator_forest(Game *game){
    //*((game->proba)+FOREST) -= 10;
    //*((game->proba)+ROAD) += 10;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if((i==5)||(i==6)||(i==9)){
            clone(&line->tiles[i], wall);
        }else{
            clone(&line->tiles[i], plain);
        }
    }
}

void generator_road(Game *game){
    //*((game->proba)+FOREST) += 5;
    //*((game->proba)+ROAD) -= 5;
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], road);
    }
    game->mobs = list_append(game->mobs, clone_mob(car_right, -car_right.length, LENGTH));
    game->mobs = list_append(game->mobs, clone_mob(car_right, 5, LENGTH));
    game->mobs = list_append(game->mobs, clone_mob(car_right, 9, LENGTH));
}

void generator_obstacle(Game*game){
    int nb = rand()%100;
    int obstacle = -1; 
    //printf("avant nb, obstacle : %d %d\n", nb, obstacle);
    while(nb>0){
        obstacle++;
        //printf("pendant avant nb, obstacle : %d %d\n", nb, obstacle);
        nb -= game->proba[obstacle];
        //printf("%d, pendant apres obstacle : %d\n", nb, obstacle);
    }
    //printf("apres nb, obstacle : %d %d\n", nb, obstacle);
    //printf("bonjour\n");
    switch (obstacle) {
        case FOREST:
        //printf("comment\n");
        generator_forest(game);
        break;

        case ROAD:
        //printf("ca va\n");
        generator_road(game);
        break;

    //     //case RIVER:
    //     //generator_river(game);

    }

}



Game *initalise_game(void){
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(0); //0 for now
    game->score = -1;
    game->mobs = NULL;
    game->proba = initialise_proba();
    return game;
}

void switch_game(Game* game){
    switch_map(game->carte);
    switch_mob(game->mobs);
    move_down(game);
    game->score++;
}


void move(int x, int y, Game* game){
    //printf("%d\n", IS_BLOCK(tile_map(game->carte, game->joueur->x + x, game->joueur->y + y)));
    //sleep(3);
    //printf("%d %d\n", game->joueur->x, game->joueur->y);
    if(!IS_BLOCK(tile_map(game->carte, game->joueur->x + x, game->joueur->y + y))){
        game->joueur->x += x;
        game->joueur->y += y;
    }
    //printf("%d %d\n", game->joueur->x, game->joueur->y);
    //sleep(3);
    //if(test_death(game)){
    //    printf("vous êtes mort\n");
    //}else{
    //    printf("cool raoul\n");
    //}
    //printf("%c %c\n", tile_map(game->carte, game->joueur->x, game->joueur->y).type, tile_map(game->carte, game->joueur->x, game->joueur->y).color);
}

void move_up(Game* game){
    move(0,1,game);
}

void move_right(Game* game){
    move(1,0,game);
}

void move_left(Game* game){
    move(-1,0,game);
}

void move_down(Game* game){
    move(0,-1,game);
}

bool test_death(Game* game){
    if(a_death_mob_collide(game->mobs, game->joueur)){
        return true;
    } else if(a_mob_collide(game->mobs, game->joueur)){
        return false;
    } else {
        return IS_DANGER(tile_map(game->carte, game->joueur->x, game->joueur->y));
    }
}

void update_game(Game* game){
    move_all_mob(game->mobs);
    Liste_Mob* cur = game->mobs;
    while(cur!=NULL){
        if(cur->mob->move_x){
            if(mob_collide(cur->mob, game->joueur)){
                game->joueur += cur->mob->move_x;
            }
        }
        cur = cur->next;
    }
    if(game->carte->head_buffer==game->carte->head_map){
        generator_obstacle(game);
    }
}

void destroy_game(Game *game){
    destroy_map(game->carte);
    free(game->joueur);
    free(game->proba);
    free(game);
}
