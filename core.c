#include "map.h"
#include "core.h"
#include "mob.h"
#include "tuiles.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

////Partie Création d'obstacles !///

int* initialise_proba(void){
    // explicite 
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
    // explicite
    *((game->proba)+FOREST) -= 10;
    *((game->proba)+ROAD) += 10;
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
    // explicite
    *((game->proba)+FOREST) += 5;
    *((game->proba)+ROAD) -= 5;
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
    // calcule l'obstacle a généré selon les probas
    int nb = rand()%100;
    int obstacle = -1; 
    while(nb>0){
        obstacle++;
        nb -= game->proba[obstacle];
    }
    switch (obstacle) {
        case FOREST:
        generator_forest(game);
        break;

        case ROAD:
        generator_road(game);
        break;

    //     //case RIVER:
    //     //generator_river(game);

    }

}



Game *initalise_game(void){
    // explicite
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(0); //0 for now
    game->score = -1;
    game->mobs = NULL;
    game->proba = initialise_proba();
    return game;
}

void switch_game(Game* game){
    // quand le joueur avance suffisemment, on avance la map, et les mob, et incrément le score
    switch_map(game->carte);
    switch_mob(game->mobs);
    move_down(game);
    game->score++;
}


void move(int x, int y, Game* game){
    if(!IS_BLOCK(tile_map(game->carte, game->joueur->x + x, game->joueur->y + y))){
        game->joueur->x += x;
        game->joueur->y += y;
    }
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
    // test si le joueur rentre dans un mon, avance sur une case danger
    if(a_death_mob_collide(game->mobs, game->joueur)){
        return true;
    }else if(a_mob_collide(game->mobs, game->joueur)){
        return false;
    }else{
        return IS_DANGER(tile_map(game->carte, game->joueur->x, game->joueur->y));
    }
}

void update_game(Game* game){
    // avance les mobs de leur path et génére un obstacle dans le buffer
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
    // Kaboum
    destroy_map(game->carte);
    free(game->carte);
    destroy_liste_mob(game->mobs);
    free(game->joueur);
    free(game->proba);
    free(game);
}
