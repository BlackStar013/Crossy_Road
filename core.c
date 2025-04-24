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
    *(proba+FOREST) = 100;
    *(proba+ROAD) = 0;
    *(proba+RIVER) = 0;
    *(proba+CANON) = 0;
    return proba;
}

int check_percent(Game* game, int init, int finish ){
    // /!\ <= à finish !!
    int tot = 0;
    for(int i = init;i<=finish;i++){
        tot += game->proba[i];
    }
    return 100 - tot;
}

void proba_change_forest(Game* game){
    int proba_change = (rand()%4)*5 + 10;
    if(*((game->proba)+FOREST) <= proba_change){
        *((game->proba)+ROAD) += ((*((game->proba)+FOREST)))/2 ;
        *((game->proba)+CANON) += ((*((game->proba)+FOREST)))/4 ;
        *((game->proba)+RIVER) += (*((game->proba)+FOREST))/4 ;
        *((game->proba)+FOREST) = 0 ;
    } else {
        *((game->proba)+ROAD) += proba_change/2 ;
        *((game->proba)+FOREST) -= proba_change ;
        *((game->proba)+RIVER) += proba_change/4 ;
        *((game->proba+CANON)) += proba_change/4;
    }
    *((game->proba)+RIVER) += check_percent(game, FOREST, CANON);

}

void generator_forest(Game *game){
    // explicite
    proba_change_forest(game);
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

void add_road(Game *game, Mob car, Tile road_, int pos, int speed, int nb_car){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], road_);
    }
    int init_x = -car.length+(rand()%3+1)*pos;
    for(int i = 0; i < nb_car; i++){
        game->mobs = list_append(game->mobs, clone_mob(car, init_x+(HEIGTH/nb_car)*i, HEIGTH+pos,speed));
    }
}

void proba_change_road(Game *game, int nb_line){
    int proba_change = 6 + (nb_line-1)*(rand()%8+6);
    if(*((game->proba)+ROAD) <= proba_change){
        *((game->proba)+FOREST) += ((*((game->proba)+ROAD)))/3 ;
        *((game->proba)+CANON) += ((*((game->proba)+ROAD)))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+ROAD))/3 ;
        *((game->proba)+ROAD) = 0 ;
    } else {
        *((game->proba)+ROAD) -= proba_change ;
        *((game->proba)+FOREST) += proba_change/3 ;
        *((game->proba+CANON)) += proba_change/3;
        *((game->proba)+RIVER) += proba_change/3 ;
    }
    *((game->proba)+CANON) += check_percent(game, FOREST, CANON);
}

void generator_road(Game *game){
    // explicite
    int nb_line = rand()%3+1;
    Tile tile;
    if(nb_line == 1){
        tile = road;
    } else {
        tile = road_down;
    }
    for(int i = 0 ; i < nb_line ; i++){
        int direction = rand()%2;
        int speed = rand()%5 +1;
        int nb_car;
        if(speed==6){
            nb_car = rand()%4 + 2;
        } else if(speed==1){
            nb_car = rand()%3 +1;
        } else {
            nb_car = rand()%4 + 1;
        }
        if(direction == 0){
            add_road(game, car_right,tile,i,speed,nb_car);
        } else {
            add_road(game,car_left,tile,i,speed,nb_car);
        }
        if(i==nb_line - 2){
            tile = road_up;
        } else {
            tile = road_middle;
        }
    }
    proba_change_road(game, nb_line);
    
}

void add_plank(Game *game, Mob plank,Tile tile, int pos, int speed){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], tile);
    }
    int proba_plank = 50;
    int plank_lenght = 0;
    int cons = 0;
    for(int i = 0; i < LENGTH; i++){
        int dice = rand()%100+1;
        if(dice<proba_plank){
            game->mobs = list_append(game->mobs, clone_mob(plank, -1+i, (HEIGTH+pos),speed));
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

void proba_change_river(Game* game,int nb_line){
    int proba_change = 10 + (nb_line-1)*(rand()%20+10);
    if(*((game->proba)+RIVER) <= proba_change){
        *((game->proba)+FOREST) += ((*((game->proba)+RIVER)))/2 ;
        *((game->proba)+ROAD) += (*((game->proba)+RIVER))/4 ;
        *((game->proba)+CANON) += (*((game->proba)+RIVER))/4 ;
        *((game->proba)+RIVER) = 0 ;
    } else {
        *((game->proba)+RIVER) -= proba_change ;
        *((game->proba)+FOREST) += proba_change/4 ;
        *((game->proba)+ROAD) += proba_change/2 ;
        *((game->proba)+CANON) += proba_change/4 ;
    }
    *((game->proba)+ROAD) += check_percent(game, FOREST, CANON);

}

void generator_river(Game *game){
    // explicite
    int nb_line = rand()%(4)+1;
    Tile tile;
    if(nb_line == 1){
        tile = water;
    } else {
        tile = water_down;
    }
    int direction = rand()%2;
    for(int i = 0 ; i < nb_line ; i++){
        int speed = rand()%3+1;
        if(direction == i%2){
            add_plank(game, plank_right,tile,i,speed);
        } else {
            add_plank(game,plank_left,tile,i,speed);
        }
        if(i==nb_line-2){
            tile = water_up;
        }else{
            tile = water_middle;
        }
    }
    proba_change_river(game,nb_line);
    
}

void add_canon(Game *game, Mob canon,Mob canon_ball, int pos, int speed,int where_can){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if(i==where_can){
            clone(&line->tiles[i], plain_block);
        } else {
            clone(&line->tiles[i], plain);
        }
    }
    int delay = (rand()%100 * speed)/100;
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed);
    canon_->delay = delay;
    ball->delay = delay;
    game->mobs = list_append(game->mobs, canon_);
    game->mobs = list_append(game->mobs, ball);
}

void proba_change_canon(Game* game, int nb_line){
    int proba_change = 5 + (nb_line-1)*(rand()%10+10);
    if(*((game->proba)+CANON) <= proba_change){
        *((game->proba)+FOREST) += ((*((game->proba)+CANON)))/3 ;
        *((game->proba)+ROAD) += (*((game->proba)+CANON))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+CANON))/3 ;
        *((game->proba)+CANON) = 0 ;
    } else {
        *((game->proba)+CANON) -= proba_change ;
        *((game->proba)+FOREST) += proba_change/3 ;
        *((game->proba)+ROAD) += proba_change/3 ;
        *((game->proba)+RIVER) += proba_change/3 ;   
    }
    *((game->proba)+ROAD) += check_percent(game, FOREST, CANON);

}

void generator_canon(Game *game){
    // explicite
    int nb_line = 1;//zzzrand()%(4)+1;
    for(int i = 0 ; i < nb_line ; i++){
        int speed = rand()%240+64;
        int direction = rand()%2;
        if(direction == 0){
            add_canon(game, canon_left,canon_ball_left,i, speed,LENGTH-1);
        } else {
            add_canon(game, canon_right,canon_ball_right,i, speed,0);
        }
    }
    proba_change_canon(game, nb_line);
    
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

        case CANON:
        generator_canon(game);
        break;

        default:
        generator_forest(game);

    }

}

void initialise_map(Game* game){
    for(int i=0;i<8;i++){
        update_game(game);
        move_up(game);
        switch_game(game);
        game->score--;
    }
}

Game *initialise_game(void){
    // explicite
    srand( time( NULL ) );
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(0); //0 for now
    game->score = -1;
    game->mobs = NULL;
    game->proba = initialise_proba();
    initialise_map(game);
    return game;
}

void switch_game(Game* game){
    // quand le joueur avance suffisamment, on avance la map, et les mob, et incrément le score
    //printf("%d\n",game->score+1);
    switch_map(game->carte);
    game->score++;
    switch_mob(game->mobs);
    game->joueur->_y -= 32;
    game->joueur->_y_dest -= 32;
}


bool move(int x, int y, Game* game){
    //printf("(%d,%d)\n",game->joueur->_x_dest/32,game->joueur->_y_dest/32);
    //printf("%d\n",tile_map(game->carte, game->joueur->_x_dest/32 + x, (game->joueur->_y_dest)/32 + y).type);
    if(tile_map(game->carte, game->joueur->_x_dest/32 + x, (game->joueur->_y_dest)/32 + y).type != water.type){
         if(game->joueur->_x_dest%32 > 16){
                  game->joueur->_x_dest =  (game->joueur->_x_dest/32+1)*32;
             } else {
                  game->joueur->_x_dest = (game->joueur->_x_dest/32)*32;
            }
            if(game->joueur->_y_dest%32 > 16){
                game->joueur->_y_dest =  (game->joueur->_y_dest/32+1)*32 ;
            } else {
                game->joueur->_y_dest =  (game->joueur->_y_dest/32)*32 ;
            }
     }
    if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 + x, (game->joueur->_y_dest)/32 + y))){
            game->joueur->_x_dest +=  x*32;
            game->joueur->_y_dest +=  y*32;
            return true ;
    } else {
        return false ;
    }
}

bool move_up(Game* game){
    return move(0,1,game);
}

bool move_right(Game* game){
    return move(1,0,game);
}

bool move_left(Game* game){
    return move(-1,0,game);
}

bool move_down(Game* game){
    return move(0,-1,game);
}

bool test_death(Game* game){
    // test si le joueur rentre dans un mon, avance sur une case danger
    if(a_death_mob_collide(game->mobs, game->joueur)){
        return true;
    }else if(a_mob_collide(game->mobs, game->joueur)){
        return false;
    }else{
        return IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, game->joueur->_y_dest/32));
    }
}

void print_percent(Game* game){
    int tot = 0;
    for(int i = 0;i<NB_OBSTACLE;i++){
        tot += game->proba[i];
    }
    printf("%d\n",tot);
}

void update_game(Game* game){
    // avance les mobs de leur path et génére un obstacle dans le buffer
    move_all_mob(game->mobs,game->joueur);
    game->joueur->_x = (game->joueur->_x + game->joueur->_x_dest)/2;
    game->joueur->_y = (game->joueur->_y + game->joueur->_y_dest)/2;
    //SDL_Delay(1000);
    Liste_Mob* cur = game->mobs;
    while(cur!=NULL){
        if(cur->mob->move_x){
            if(mob_collide(cur->mob, game->joueur)){
                if(cur->mob->delay==0){
                    game->joueur->_x += cur->mob->move_x;
                    game->joueur->_x_dest += cur->mob->move_x;
                }
            }
        }
        cur = cur->next;
    }
    if(game->carte->head_buffer==game->carte->head_map){
        generator_obstacle(game);
        //print_percent(game);
    }
}


void destroy_game(Game *game){
    // Kaboum
    destroy_map(game->carte);
    destroy_liste_mob(game->mobs);
    free(game->joueur);
    free(game->proba);
    free(game);
}
