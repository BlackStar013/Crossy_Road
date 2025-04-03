#include "map.h"
#include "core.h"
#include "mob.h"
#include "tuiles.h"
#include <stdlib.h>
#include <stdio.h>

////Partie Création d'obstacles !///

int* initialise_proba(void){
    int* proba = malloc((NB_OBSTACLE)*sizeof(int));
    for(int i = 0;i<NB_OBSTACLE;i++){
        *(proba+i) = 0;
    }
    *(proba+FOREST) = 20;
    *(proba+ROAD) = 80;
    *(proba+RIVER) = 0;
    return proba;
}

void generator_forest(Game *game){
    *((game->proba)+FOREST) -= 10;
    *((game->proba)+ROAD) += 10;
    add_buffer_line(game->carte);
    Tile* tiles = game->carte->head_buffer->tiles;
    for(int i = 0;i>LENGTH;i++){
        if((i==5)||(i==6)||(i=9)){
            clone(*tiles,tree);
        } else {
            clone(*tiles,plain);
        }
        tiles++;
    }
}

void generator_road(Game *game){
    *((game->proba)+FOREST) += 10;
    *((game->proba)+ROAD) -= 10;
    add_buffer_line(game->carte);
    Tile* tiles = game->carte->head_buffer->tiles;
    for(int i = 0;i>LENGTH;i++){
        clone(*tiles,road);
        tiles++;
    }
    list_append(game->mobs,clone_mob(car_right, -1, LENGTH+1));
}

//Non fonctionnelllelelzeùjkghbnqeiùkzrfjnziKMYGDP
//void (*initialise_generator(void))(Game*){
//    void (*generators)(Game*) = malloc(NB_OBSTACLE*sizeof(void(Game*)));
//    //*(generators+FOREST) = generator_forest;
//    return NULL;
//
//}

////FIn de partie création d'obastcales////


Game *initalise_game(void){
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(0); //0 for now
    game->score = 0;
    game->mobs = NULL;
    game->proba = initialise_proba();//A changer absolument !!!!! 
    game->generator = NULL; //initialise_generator(); //De même !!
    return game;
}

void move(int x, int y,Game* game){
    if(!IS_BLOCK(tile_map(game->carte,game->joueur->x + x, game->joueur->y + y))){
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

void destroy_game(Game *game){
    destroy_map(game->carte);
    free(game->joueur);
    free(game->proba);
    free(game);
}
