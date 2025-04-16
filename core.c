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
//    temp modif pour test
//    *(proba+FOREST) = 20;
//    *(proba+ROAD) = 60;
//    *(proba+RIVER) = 20;
    *(proba+FOREST) = 20;
    *(proba+ROAD) = 20;
    *(proba+RIVER) = 0;
    *(proba+BUSHES) = 60;
    return proba;
}

void generator_forest(Game *game){
    // explicite
    int proba_change = (rand()%4)*5 + 10;
    if(*((game->proba)+FOREST) <= proba_change){
        *((game->proba)+ROAD) += (2*(*((game->proba)+FOREST)))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+FOREST))/3+1 ;
        *((game->proba)+FOREST) = 0 ;
    } else {
        *((game->proba)+ROAD) += 2*proba_change/3 ;
        *((game->proba)+FOREST) -= proba_change ;
        *((game->proba)+RIVER) += proba_change/3+1 ;
    }
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], plain);
    }
    int nb_wall = rand()%6;
    for(int i = 0; i<nb_wall; i++){
        clone(&line->tiles[rand()%LENGTH], wall);
    }

}

void add_road(Game *game, Mob car, int pos, int speed, int nb_car){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], road);
    }
    int init_x = -car.length+(rand()%3+1)*pos;
    for(int i = 0; i < nb_car; i++){
        game->mobs = list_append(game->mobs, clone_mob(car, init_x+(HEIGHT/nb_car)*i, HEIGHT+pos,speed));
    }
}

void generator_road(Game *game){
    // explicite
    int nb_line = rand()%2+1;
    for(int i = 0 ; i < nb_line ; i++){
        int direction = rand()%2;
        int level = rand()%3;
        int speed = (level) + 1;
        int nb_car = rand()%4 + 1;
        if(nb_car > 1){
            nb_car -= (2 - level)/2;
        }
        if(direction == 0){
            add_road(game, car_right,i,speed,nb_car);
        } else {
            add_road(game,car_left,i,speed,nb_car);
        }
    }
    int proba_change = 5 + (nb_line-1)*(rand()%6+5);
    if(*((game->proba)+ROAD) <= proba_change){
        *((game->proba)+FOREST) += (2*(*((game->proba)+ROAD)))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+ROAD))/3+1 ;
        *((game->proba)+ROAD) = 0 ;
    } else {
        *((game->proba)+ROAD) -= proba_change ;
        *((game->proba)+FOREST) += 2*proba_change/3 ;
        *((game->proba)+RIVER) += proba_change/3+1 ;
    }
    
}

void add_plank(Game *game, Mob plank, int pos, int speed){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], water);
    }
    int proba_plank = 50;
    int plank_lenght = 0;
    int cons = 0;
    for(int i = 0; i < LENGHT; i++){
        int dice = rand()%100+1;
        if(dice<proba_plank){
            game->mobs = list_append(game->mobs, clone_mob(plank, -1+i, HEIGHT+pos,speed));
            if(plank_lenght<3){
                proba_plank += 10;
            } else if (plank_lenght<5){
                proba_plank -= 10;
            } else {
                proba_plank -= 20;
            }
            cons = 0;
            plank_lenght++;
        } else {
            if(plank_lenght == 0){
                proba_plank += 10*(cons/2);
            }else if(plank_lenght<3){
                proba_plank = 50;
            } else if (plank_lenght<5){
                proba_plank = 30;
            } else {
                proba_plank = 10;
            }
            plank_lenght = 0;
            cons++;
        }
    }
}

void generator_river(Game *game){
    // explicite
    int nb_line = rand()%(4)+1;
    int direction = rand()%2;
    for(int i = 0 ; i < nb_line ; i++){
        int level = rand()%3;
        int speed = (level) + 1;
        if(direction == i%2){
            add_plank(game, plank_right,i,speed);
        } else {
            add_plank(game,plank_left,i,speed);
        }
    }
    int proba_change = 5 + (nb_line-1)*(rand()%6+5);
    if(*((game->proba)+RIVER) <= proba_change){
        *((game->proba)+FOREST) += (2*(*((game->proba)+ROAD)))/3 ;
        *((game->proba)+ROAD) += (*((game->proba)+ROAD))/3+1 ;
        *((game->proba)+RIVER) = 0 ;
    } else {
        *((game->proba)+RIVER) -= proba_change ;
        *((game->proba)+FOREST) += 2*proba_change/3 ;
        *((game->proba)+ROAD) += proba_change/3+1 ;
    }
    
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

        case RIVER:
        generator_river(game);
        break;

        case BUSHES:
        generator_bushes(game);
        break;
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
    if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x/32 + x, game->joueur->_y/32 + y))){
        game->joueur->_x += x*32;
        game->joueur->_y += y*32;
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
        return IS_DANGER(tile_map(game->carte, game->joueur->_x/32, game->joueur->_y/32));
    }
}

void update_game(Game* game){
    // avance les mobs de leur path et génére un obstacle dans le buffer
    move_all_mob(game->mobs,game->joueur);
    Liste_Mob* cur = game->mobs;
    while(cur!=NULL){
        if(cur->mob->move_x){
            if(mob_collide(cur->mob, game->joueur)){
                game->joueur->_x += cur->mob->move_x;
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

void generator_bushes(Game *game){
    //les buissons
    int proba_change = (rand()%4)*5;
    if(*((game->proba)+BUSHES) <= proba_change){
        *((game->proba)+FOREST) += (2*(*((game->proba)+BUSHES)))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+BUSHES))/3+1 ;
        *((game->proba)+BUSHES) = 0 ;
    } else {
        *((game->proba)+FOREST) += 2*proba_change/3 ;
        *((game->proba)+BUSHES) -= proba_change ;
        *((game->proba)+RIVER) += proba_change/3+1 ;
    }
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], plain);
    }
    int nb_regular = rand()%5;
    for(int i = 0; i<nb_regular; i++){
        clone(&line->tiles[rand()%LENGTH], tree);
    }
    int nb_aggressive = rand()%3;
    for(int i = 0; i<nb_aggressive; i++){
        game->mobs = list_append(game->mobs, clone_mob(bush, (rand()%LENGTH)*32, HEIGHT,1));
    }
}
