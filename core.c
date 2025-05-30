#include "map.h"
#include "core.h"
#include "mob.h"
#include "tuiles.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

////Partie Création d'obstacles !///

void add_exp(int x, int y, Game *game){
    //speed = 0 -> Pas sur rivière
    //+ -> sur rivière normale de droite à gauche
    //- sur rivière normale de gauche à droite
    // Pour autre type de rivière, voir Yovhann.
    if(game->joueur->character_id == RANTON){
        game->exp = list_append(game->exp,clone_mob(champi, x, y, 4, 4));
    } else {
        game->exp = list_append(game->exp,clone_mob(xp, x, y, 4, 4));
    }
}


void proba_change_game(Game* game, int obstacle){
    for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *(game->proba+obstacle) = 100;
}

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
    int nb_wall = rand()%(LENGTH/2 - 2);
    for(int i = 0; i<nb_wall; i++){
        if(rand()%3){
            clone(&line->tiles[rand()%LENGTH], wall);
        } else {
            clone(&line->tiles[rand()%LENGTH], tree);
        }
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
        game->mobs = list_append(game->mobs, clone_mob(car, init_x+(LENGTH/nb_car)*i, HEIGTH+pos,speed,0));
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
        if(rand()%4){
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
        } else {
            int direction = rand()%2;
            int speed = rand()%6 +2;
            int nb_bus = rand()%2 +1;
            if(direction == 0){
                add_road(game, bus_right,tile,i,speed,nb_bus);
            } else {
                add_road(game,bus_left,tile,i,speed,nb_bus);
            }
            if(i==nb_line - 2){
                tile = road_up;
            } else {
                tile = road_middle;
            }
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
            game->mobs = list_append(game->mobs, clone_mob(plank, -1+i, (HEIGTH+pos),speed,0));
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
            if(where_can){
                clone(&line->tiles[where_can], plain_block_right);
            } else {
                clone(&line->tiles[where_can], plain_block_left);
            };
        } else {
            clone(&line->tiles[i], plain);
        }
    }
    int delay = (rand()%100 * speed)/100;
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed,(LENGTH*32)/32);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed,(LENGTH*32)/32);
    ball->memory__x = where_can*32;
    canon_->delay = delay;
    ball->delay = delay;
    game->mobs = list_append(game->mobs, canon_);
    game->projectile = list_append(game->projectile, ball);
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
        int speed = rand()%128+128;
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
    int nb = rand()%100+1;
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

        case BUSHES:
        generator_bushes(game);
        break;

        case FAST_RIVER:
        generator_fast_river(game);
        break;

        case BLOCKED_CANON:
        generator_blocked_canon(game);
        break;

        case MAGIC_ROAD:
        generator_magic_road(game);
        break;
                
        case ENTRANCE:
        generator_entrance(game);
        break;

        case FIRE:
        generator_fire(game);
        break;

        case CAVE:
        generator_cave(game);
        break;

        case CULTIST:
        generator_cultist(game);
        break;
                
        case MAGE:
        generator_mage(game);
        break;

        case TRANSITION_CAVE:
        generator_transition_cave(game);
        break;

        case STONE:
        generator_pebble(game);
        break;
        case BABEL:
        generator_babel(game);
        break;

        case TRANSITION_FINAL:
        generator_transition_heart(game);
        break;

        case ACID_RIVER:
        generator_acid_river(game);
        break;

        case FLESH_MAW:
        generator_f_maw(game);
        break;

        case FLESH_SPIKES:
        generator_f_spike(game);
        break;

        case BONESHOOTER:
        generator_boneshooter(game);
        break;

        case BARRAGE:
        generator_barrage(game);
        break;
        
        case SENTINEL_EYE:
        generator_sentinel_eye(game);
        break;

        case LABYRINTH:
        generator_labyrinth(game);
        break;
    }

}

///////////////////////////////////////////
//générateurs avancés
///////////////////////////////////////////


void proba_change_entrance(Game* game){
    for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *((game->proba)+FIRE) = 100;

}

void generator_entrance(Game *game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],plain);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==LENGTH/2 || i==((LENGTH/2)-1)) {
            clone(&line->tiles[i],transition);
        }
        else {
            clone(&line->tiles[i],low_wall);
        }
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],planks);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],planks);
    }
    proba_change_entrance(game);
}

void proba_change_fire(Game* game){
    //Pour l'instant fire ne donne que fire
}

void generator_fire(Game *game){
    //feu
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    int nb_Block = rand()%3;
    for(int i = 0; i<nb_Block; i++){
        clone(&line->tiles[rand()%LENGTH], brokenplanks);
    }
    int nb_flames = rand()%4+1;
    for(int i = 0; i<nb_flames; i++){
        int pattern=rand()%4;
        int pos;
        switch (pattern){
        case 0:
            pos=rand()%(LENGTH-2);
            game->projectile = list_append(game->projectile, clone_mob(fire_clock, pos, HEIGTH,(rand()%3)+1,16));
            break;
        case 1:
            pos=rand()%(LENGTH-3);
            game->projectile = list_append(game->projectile, clone_mob(fire_clock, pos, HEIGTH,(rand()%3)+1,32));
            break;
        case 2:
            pos=rand()%(LENGTH-2);
            game->projectile = list_append(game->projectile, clone_mob(fire_counter, pos, HEIGTH,(rand()%3)+1,16));
            break;
        case 3:
            pos=rand()%(LENGTH-3);
            game->projectile = list_append(game->projectile, clone_mob(fire_counter, pos, HEIGTH,(rand()%3)+1,32));
            break;
        default:
            break;
        }
    proba_change_fire(game);
        
    }
}


void proba_change_transition_cave(Game* game){
        for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *((game->proba)+CULTIST) = 100;
}


void generator_transition_cave(Game * game){
    Line* line;

    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],planks);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==LENGTH/2 || i==((LENGTH/2)-1) ||i==LENGTH/2+1 || i==((LENGTH/2)-2)) {
            clone(&line->tiles[i],Porte);
        }
        else {
            clone(&line->tiles[i],MurManoir);
        }
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],cave);
    }
    proba_change_transition_cave(game);



}


void proba_change_cave(Game* game){
    int proba_change = (rand()%4)*5 + 10;
    if(*((game->proba)+CAVE) <= proba_change){
        *((game->proba)+CULTIST) += (2*(*((game->proba)+CAVE)))/5+1 ;
        *((game->proba)+BABEL) += ((*((game->proba)+CAVE)))/5 ;
        *((game->proba)+STONE) += ((*((game->proba)+CAVE)))/5 ;
        *((game->proba)+MAGE) += (*((game->proba)+CAVE))/5 ;
        *((game->proba)+CAVE) = 0 ;
    } else {
        *((game->proba)+CULTIST) += 2*proba_change/5+1 ;
        *((game->proba)+CAVE) -= proba_change ;
        *((game->proba)+MAGE) += proba_change/5 ;
        *((game->proba)+STONE) += proba_change/5 ;
        *((game->proba)+MAGE) += proba_change/5 ;
    }
    *((game->proba)+CAVE) += check_percent(game, CAVE, BABEL);
}


void generator_cave(Game *game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    int nb_wall = rand()%6+2;
    for(int i = 0; i<nb_wall; i++){
        int place=rand()%LENGTH;
        if (place!=0 && place!=LENGTH-1){
            if (!line->tiles[place+1].is_block || !line->tiles[place-1].is_block){
                clone(&line->tiles[place], stalagmite);
            }
        }
    }
    proba_change_cave(game);
}

void proba_change_cultist(Game* game){
    *((game->proba)+CAVE) += ((*((game->proba)+CULTIST))/3) ;
    *((game->proba)+STONE) += ((*((game->proba)+CULTIST))/3) ;
    *((game->proba)+MAGE) += ((*((game->proba)+CULTIST))/3)+1;
    *((game->proba)+CULTIST) = 0 ;
    *((game->proba)+CAVE) += check_percent(game, CAVE, BABEL);

    *((game->proba)+CAVE) += ((*((game->proba)+BABEL))/3) ;
    *((game->proba)+STONE) += ((*((game->proba)+BABEL))/3) ;
    *((game->proba)+MAGE) += ((*((game->proba)+BABEL))/3)+1;
    *((game->proba)+BABEL) = 0 ;
    *((game->proba)+CAVE) += check_percent(game, CAVE, BABEL);

}



void generator_cultist(Game *game){
    //les cultistes avec des dagues
    int type=rand()%3;
    int sens=rand()%2;
    int nb_lines;
    int lat_size;
    if (type==2) {
        lat_size=6;
        }
    else {
        lat_size=5;
    }
    if (type==1) {
        nb_lines=4;
        }
    else {
        nb_lines=3;
    }
    int pos_cult=rand()%(LENGTH-lat_size);
    int nb_cultists=rand()%2+1;
    for (int i=0;i<nb_lines;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int j = 0; j<LENGTH; j++){
            if (((j>=pos_cult && j<=(pos_cult+lat_size-2))&&(i==0||i==(nb_lines-1)||j==(pos_cult+lat_size-2)||j==pos_cult))) {
                clone(&line->tiles[j], cave);
            }
            else {
                clone(&line->tiles[j], stalagmite);
            }
        }
    }
    int speed=(rand()%3)+1;
    switch (type) {
        case 0:
        if (sens==0){
            game->mobs = list_append(game->mobs, clone_mob(cult_clock1, pos_cult+2, HEIGTH+nb_lines-1,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter1, pos_cult+1, HEIGTH+nb_lines-1,speed,16));
        }
        break;
        case 2:
        if (sens==0){
            game->mobs = list_append(game->mobs, clone_mob(cult_clock2, pos_cult+3, HEIGTH+nb_lines-1,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter2, pos_cult+1, HEIGTH+nb_lines-1,speed,16));
        }
        break;
        case 1:
        if (sens==0){
            game->mobs = list_append(game->mobs, clone_mob(cult_clock3, pos_cult+2, HEIGTH+nb_lines-1,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter3, pos_cult+1, HEIGTH+nb_lines-1,speed,16));
        }
        break;
    }
    if (nb_cultists==2) {
        Mob *Double;
        switch (type) {
            case 0:
                if (sens==0){
                    Double=clone_mob(cult_clock1, pos_cult+1, HEIGTH,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter1, pos_cult+2, HEIGTH,speed,16);
                }
                break;
            case 2:
                if (sens==0){
                    Double=clone_mob(cult_clock2, pos_cult+1, HEIGTH,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter2, pos_cult+3, HEIGTH,speed,16);
                }
                break;
            case 1:
                if (sens==0){
                    Double=clone_mob(cult_clock3, pos_cult+1, HEIGTH,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter3, pos_cult+2, HEIGTH,speed,16);
                }
                break;
        }
        Double->action_value=2;
        game->mobs=list_append(game->mobs,Double);
    }
    proba_change_cultist(game);
}


void proba_change_mage(Game* game){
    int proba_change = (rand()%4)*5+10;
    if(*((game->proba)+MAGE) <= proba_change){
        *((game->proba)+CULTIST) += (*((game->proba)+MAGE))/4 ;
        *((game->proba)+CAVE) += (*((game->proba)+MAGE))/4 ;
        *((game->proba)+STONE) += (*((game->proba)+MAGE))/4+1;
        *((game->proba)+BABEL) += (*((game->proba)+MAGE))/4;
        *((game->proba)+MAGE) = 0 ;
    } else {
        *((game->proba)+CULTIST) += proba_change/2 ;
        *((game->proba)+BABEL) += proba_change/2 ;
        *((game->proba)+STONE) += proba_change/2 ;
        *((game->proba)+MAGE) -= proba_change ;
        *((game->proba)+CAVE) += proba_change/2+1 ;
    }
    *((game->proba)+MAGE) += check_percent(game, CAVE, BABEL);
}

void generator_mage(Game *game){
    add_buffer_line(game->carte);
    int pos_x=rand()%LENGTH;
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==pos_x){
            clone(&line->tiles[i], base);
        }else {
            clone(&line->tiles[i], channel);
        }
    }
    int speed=3;
    Mob *c=clone_mob(mage, pos_x, HEIGTH,speed,0);
    c->offset_x+=3;
    game->mobs = list_append(game->mobs, c);
    Mob *arrow=clone_mob(curse_arrow, pos_x, HEIGTH,speed,0);
    if (speed==2){
        arrow->action_value=16;
    }
    if (speed==3){
        arrow->action_value=12;
    }
    if (speed==1){
        arrow->action_value=24;
    }
    game->projectile = list_append(game->projectile, arrow);
    proba_change_mage(game);
}

void add_pebble(Game *game, Mob pebble, Tile cave_, int pos, int speed, int nb_pebble){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave_);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave_);
    }
    int init_x = -pebble.length+(rand()%3+1)*pos;
    for(int i = 0; i < nb_pebble; i++){
        game->mobs = list_append(game->mobs, clone_mob(pebble, init_x+(LENGTH/nb_pebble)*i, HEIGTH+pos,speed,0));
    }
}

void proba_change_pebble(Game *game, int nb_line){
    int proba_change = 10 + (nb_line-1)*(rand()%8+10);
    if(*((game->proba)+STONE) <= proba_change){
        *((game->proba)+CAVE) += ((*((game->proba)+STONE)))/4 ;
        *((game->proba)+MAGE) += ((*((game->proba)+STONE)))/4 ;
        *((game->proba)+CULTIST) += (*((game->proba)+STONE))/4 ;
        *((game->proba)+BABEL) += (*((game->proba)+STONE))/4 ;
        *((game->proba)+STONE) = 0 ;
    } else {
        *((game->proba)+STONE) -= proba_change ;
        *((game->proba)+CAVE) += proba_change/4 ;
        *((game->proba+MAGE)) += proba_change/4;
        *((game->proba)+CULTIST) += proba_change/4 ;
        *((game->proba)+BABEL) += proba_change/4 ;
    }
    *((game->proba)+STONE) += check_percent(game, CAVE, BABEL);
}

void generator_pebble(Game *game){
    // explicite
    int nb_line = rand()%2+1;
    Tile tile=cave;
    for(int i = 0 ; i < 2*nb_line ; i+=2){
        int direction = rand()%2;
        int speed = rand()%5 +1;
        int nb_pebble;
        if(speed==6){
            nb_pebble = rand()%4 + 2;
        } else if(speed==1){
            nb_pebble = rand()%3 +1;
        } else {
            nb_pebble = rand()%4 + 1;
        }
        if(direction == 0){
            add_pebble(game, pebble_right,tile,i,speed,nb_pebble);
        } else {
            add_pebble(game,pebble_left,tile,i,speed,nb_pebble);
        }
    }
    proba_change_pebble(game, nb_line);
}

void proba_change_babel(Game* game){
    *((game->proba)+STONE) += ((*((game->proba)+CAVE))/2) ;
    *((game->proba)+MAGE) += ((*((game->proba)+CAVE))/2)+1;
    *((game->proba)+STONE) += ((*((game->proba)+CULTIST))/2) ;
    *((game->proba)+MAGE) += ((*((game->proba)+CULTIST))/2)+1;
    *((game->proba)+STONE) += ((*((game->proba)+BABEL))/2) ;
    *((game->proba)+MAGE) += ((*((game->proba)+BABEL))/2)+1;
    *((game->proba)+CULTIST) = 0 ;
    *((game->proba)+BABEL) = 0 ;
    *((game->proba)+CAVE) = 0 ;
    *((game->proba)+STONE) += check_percent(game, CAVE, BABEL);

}

void generator_babel(Game *game){
    int type=rand()%4;
    int sens=rand()%2;
    int pos_b=rand()%(LENGTH-5);
    for (int i=0;i<4;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int j = 0; j<LENGTH; j++){
            clone(&line->tiles[j], cave);
            if(j<pos_b ||j>pos_b+4){
                clone(&line->tiles[j], stalagmite);
            }
            else {
                switch (type){
                case 0:
                    if (i==1){
                        if (j==1+pos_b ||j==3+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    if (i==3){
                        if (j==0+pos_b ||j==2+pos_b||j==4+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    break;
                case 1:
                    if (i==1){
                        if (j==2+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    if (i==3){
                        if (j==0+pos_b ||j==4+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    // if (i==4){
                    //     if (j==1+pos_b ||j==3+pos_b){
                    //         clone(&line->tiles[j], stalagmite);
                    //     }
                    // }
                    break;
                case 2:
                    if (i==0){
                        if (j==0+pos_b ||j==4+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    // if (i==4){
                    //     if (j==1+pos_b ||j==2+pos_b||j==3+pos_b){
                    //         clone(&line->tiles[j], stalagmite);
                    //     }
                    // }
                    break;
                case 3:
                    if (i==2){
                        if (j==0+pos_b ||j==4+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    if (i==3){
                        if (j==1+pos_b ||j==3+pos_b){
                            clone(&line->tiles[j], stalagmite);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    int speed=(rand()%3)+1;
    switch (type) {
            case 0:
                if (sens==0){
                    game->mobs = list_append(game->mobs, clone_mob(babel11, pos_b+2, HEIGTH+1,speed,16));
                }
                else{
                    game->mobs = list_append(game->mobs, clone_mob(babel12, pos_b+3, HEIGTH+2,speed,16));
                }
                break;
            case 1:
                if (sens==0){
                    game->mobs = list_append(game->mobs, clone_mob(babel21, pos_b+1, HEIGTH+2,speed,16));
                }
                else{
                    game->mobs = list_append(game->mobs, clone_mob(babel22, pos_b+3, HEIGTH+2,speed,16));
                }
                break;
            case 2:
                if (sens==0){
                    game->mobs = list_append(game->mobs, clone_mob(babel31, pos_b+2, HEIGTH+2,speed,16));
                }
                else{
                    game->mobs = list_append(game->mobs, clone_mob(babel32, pos_b+1, HEIGTH+1,speed,16));
                }
                break;
            case 3:
                if (sens==0){
                    game->mobs = list_append(game->mobs, clone_mob(babel41, pos_b+1, HEIGTH+1,speed,16));
                }
                else{
                    game->mobs = list_append(game->mobs, clone_mob(babel42, pos_b+3, HEIGTH+1,speed,16));
                }
                break;
            default:
                break;
    }
    proba_change_babel(game);
}
//NEW_GENERATOR//

void proba_change_bushes(Game* game){
    int proba_change = (rand()%4)*5 + 10;
    if(*((game->proba)+BUSHES) <= proba_change){
        *((game->proba)+MAGIC_ROAD) += ((*((game->proba)+BUSHES)))/2 ;
        *((game->proba)+BLOCKED_CANON) += ((*((game->proba)+BUSHES)))/4 ;
        *((game->proba)+FAST_RIVER) += (*((game->proba)+BUSHES))/4 ;
        *((game->proba)+BUSHES) = 0 ;
    } else {
        *((game->proba)+MAGIC_ROAD) += proba_change/2 ;
        *((game->proba)+BUSHES) -= proba_change ;
        *((game->proba)+FAST_RIVER) += proba_change/4 ;
        *((game->proba+BLOCKED_CANON)) += proba_change/4;
    }
    *((game->proba)+FAST_RIVER) += check_percent(game, BUSHES, MAGIC_ROAD);
}

void generator_bushes(Game *game){
    //les buissons
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], plain);
    }
     int nb_wall = rand()%(LENGTH/2 - 2);
     int nb_max_bush = 2;
     int pos_bush[3] = {-1,-1,-1};
    for(int i = 0; i<nb_wall; i++){
        int pos = rand()%LENGTH;
        if(rand()%3){
            clone(&line->tiles[pos], tree);
        } else if (rand()%3){
            clone(&line->tiles[pos], ruins);
        } else if (nb_max_bush) {
            game->mobs = list_append(game->mobs, clone_mob(bush, pos, HEIGTH,4,128));
            pos_bush[2-nb_max_bush] = pos;
            nb_max_bush--;
        } else {
            clone(&line->tiles[pos], wall);
        }
        for(int i =0;pos_bush[i]!=-1;i++){
             clone(&line->tiles[pos_bush[i]], plain);
        }
    }
    proba_change_bushes(game);
}


void add_magic_road(Game *game, Mob car, Tile road_, int pos, int speed, int max_delay, int nb_car){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], road_);
    }
    int init_x = -car.length+(rand()%3+1)*pos;
    for(int i = 0; i < nb_car; i++){
        game->mobs = list_append(game->mobs, clone_mob(car, init_x+(LENGTH/nb_car)*i, HEIGTH+pos,speed,max_delay));
    }
}

void proba_change_magic_road(Game *game, int nb_line){
    int proba_change = 6 + (nb_line-1)*(rand()%8+6);
    if(*((game->proba)+MAGIC_ROAD) <= proba_change){
        *((game->proba)+BUSHES) += ((*((game->proba)+MAGIC_ROAD)))/3 ;
        *((game->proba)+BLOCKED_CANON) += ((*((game->proba)+MAGIC_ROAD)))/3 ;
        *((game->proba)+FAST_RIVER) += (*((game->proba)+MAGIC_ROAD))/3 ;
        *((game->proba)+MAGIC_ROAD) = 0 ;
    } else {
        *((game->proba)+MAGIC_ROAD) -= proba_change ;
        *((game->proba)+BUSHES) += proba_change/3 ;
        *((game->proba+BLOCKED_CANON)) += proba_change/3;
        *((game->proba)+FAST_RIVER) += proba_change/3 ;
    }
    *((game->proba)+BLOCKED_CANON) += check_percent(game, BUSHES, MAGIC_ROAD);
}

void generator_magic_road(Game *game){
    // explicite
    int nb_line = rand()%3+1;
    Tile tile;
    if(nb_line == 1){
        tile = road;
    } else {
        tile = road_down;
    }
    for(int i = 0 ; i < nb_line ; i++){
        if(rand()%4){
            int direction = rand()%2;
            int speed = rand()%5;
            int wait = rand()%32;
            int nb_car;
            if(speed==6){
                nb_car = rand()%4 + 1;
            } else if(speed==0){
                if(rand()%4){
                    nb_car = 1;
                } else {
                    nb_car = 2;
                }
            } else {
                nb_car = rand()%3 + 1;
            }
            if(direction == 0){
                add_magic_road(game, magic_car_right,tile,i,speed,wait,nb_car);
            } else {
                add_magic_road(game,magic_car_left,tile,i,speed,wait,nb_car);
            }
            if(i==nb_line - 2){
                tile = road_up;
            } else {
                tile = road_middle;
            }
        } else {
            int direction = rand()%2;
            int speed = rand()%6 +1;
            int wait = rand()%96 +32;
            int nb_bus = rand()%2 + 1;
            if(direction == 0){
                add_magic_road(game, big_bus_right,tile,i,speed,wait,nb_bus);
            } else {
                add_magic_road(game,big_bus_left,tile,i,speed,wait,nb_bus);
            }
            if(i==nb_line - 2){
                tile = road_up;
            } else {
                tile = road_middle;
            }
        }   
    }
    proba_change_magic_road(game, nb_line);
}


void add_fast_plank(Game *game, Mob plank,Tile tile, int pos, int speed){
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
            game->mobs = list_append(game->mobs, clone_mob(plank, -1+i, (HEIGTH+pos),speed,0));
            if(plank_lenght<3){
                proba_plank += 10;
            } else if (plank_lenght<5){
                proba_plank -= 15;
            } else {
                proba_plank -= 30;
            }
            cons = 0;
            plank_lenght++;
        } else {
            if(plank_lenght == 0){
                proba_plank += 10*(cons/2);
            }else if(plank_lenght<3){
                proba_plank = 30;
            } else if (plank_lenght<5){
                proba_plank = 10;
            } else {
                proba_plank = 0;
            }
            plank_lenght = 0;
            cons++;
        }
    }
}

void proba_change_fast_river(Game* game,int nb_line){
    int proba_change = 10 + (nb_line-1)*(rand()%20+10);
    if(*((game->proba)+FAST_RIVER) <= proba_change){
        *((game->proba)+BUSHES) += ((*((game->proba)+FAST_RIVER)))/2 ;
        *((game->proba)+MAGIC_ROAD) += (*((game->proba)+FAST_RIVER))/4 ;
        *((game->proba)+BLOCKED_CANON) += (*((game->proba)+FAST_RIVER))/4 ;
        *((game->proba)+FAST_RIVER) = 0 ;
    } else {
        *((game->proba)+FAST_RIVER) -= proba_change ;
        *((game->proba)+BUSHES) += proba_change/4 ;
        *((game->proba)+MAGIC_ROAD) += proba_change/2 ;
        *((game->proba)+BLOCKED_CANON) += proba_change/4 ;
    }
    *((game->proba)+MAGIC_ROAD) += check_percent(game, BUSHES, MAGIC_ROAD);

}

void generator_fast_river(Game *game){
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
        int speed = rand()%4;
        if(!speed){
            if(rand()%3){
                speed++;
            }
        }
        if(direction == i%2){
            add_fast_plank(game, plank_right,tile,i,speed);
        } else {
            add_fast_plank(game,plank_left,tile,i,speed);
        }
        if(i==nb_line-2){
            tile = water_up;
        }else{
            tile = water_middle;
        }
    }
    proba_change_fast_river(game,nb_line);
    
}

void add_blocked_canon(Game *game, Mob canon,Mob canon_ball, int pos, int speed,int where_can){
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], plain);
    }
    int nb_wall = rand()%(LENGTH/3 - 2);
    for(int i = 0; i<nb_wall; i++){
        if(rand()%4){
            clone(&line->tiles[rand()%(LENGTH)], ruins);
        } else {
            clone(&line->tiles[rand()%LENGTH], tree);
        }
    }
    if(where_can){
        clone(&line->tiles[where_can], plain_block_right);
    } else {
        clone(&line->tiles[where_can], plain_block_left);
    }
    int delay = (rand()%100 * speed)/100;
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed,(LENGTH*32)/32);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed,(LENGTH*32)/32);
    ball->memory__x = where_can*32;
    canon_->delay = delay;
    ball->delay = delay;
    game->mobs = list_append(game->mobs, canon_);
    game->projectile = list_append(game->projectile, ball);
}

void proba_change_blocked_canon(Game* game, int nb_line){
    int proba_change = 5 + (nb_line-1)*(rand()%10+10);
    if(*((game->proba)+BLOCKED_CANON) <= proba_change){
        *((game->proba)+BUSHES) += ((*((game->proba)+BLOCKED_CANON)))/3 ;
        *((game->proba)+MAGIC_ROAD) += (*((game->proba)+BLOCKED_CANON))/3 ;
        *((game->proba)+FAST_RIVER) += (*((game->proba)+BLOCKED_CANON))/3 ;
        *((game->proba)+BLOCKED_CANON) = 0 ;
    } else {
        *((game->proba)+BLOCKED_CANON) -= proba_change ;
        *((game->proba)+BUSHES) += proba_change/3 ;
        *((game->proba)+MAGIC_ROAD) += proba_change/3 ;
        *((game->proba)+FAST_RIVER) += proba_change/3 ;   
    }
    *((game->proba)+BUSHES) += check_percent(game, BUSHES, MAGIC_ROAD);

}

void generator_blocked_canon(Game *game){
    // explicite
    int nb_line = 1;//zzzrand()%(4)+1;
    for(int i = 0 ; i < nb_line ; i++){
        int speed = rand()%192+64;
        int direction = rand()%2;
        if(direction == 0){
            add_blocked_canon(game, canon_left,canon_ball_left,i, speed,LENGTH-1);
        } else {
            add_blocked_canon(game, canon_right,canon_ball_right,i, speed,0);
        }
    }
    proba_change_blocked_canon(game, nb_line);
    
}


void proba_change_transition_heart(Game* game){
        for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *((game->proba)+FLESH_MAW) = 100;
}

void generator_transition_heart(Game*game){
    Line* line;

    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i],transition_finale);
    }
    proba_change_transition_heart(game);
}

void proba_change_maw(Game* game){
    int proba_change = rand()%10+15;
    if(*((game->proba)+FLESH_MAW) <= proba_change){
        *((game->proba)+FLESH_SPIKES)+= ((*((game->proba)+FLESH_MAW)))/5 ;
        *((game->proba)+BONESHOOTER) += (*((game->proba)+FLESH_MAW))/5 ;
        *((game->proba)+ACID_RIVER) += (*((game->proba)+FLESH_MAW))/5 ;
        *((game->proba)+BARRAGE)+= ((*((game->proba)+FLESH_MAW)))/5 ;
        *((game->proba)+SENTINEL_EYE) += (*((game->proba)+FLESH_MAW))/5 ;
        *((game->proba)+FLESH_MAW) = 0 ;
    } else {
        *((game->proba)+FLESH_MAW) -= proba_change ;
        *((game->proba)+FLESH_SPIKES)+= proba_change/5 ;
        *((game->proba)+BONESHOOTER) += proba_change/5 ;
        *((game->proba)+ACID_RIVER) += proba_change/5 ;
        *((game->proba)+BARRAGE)+= proba_change/5 ;
        *((game->proba)+SENTINEL_EYE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}

void generator_f_maw(Game*game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int nb_danger = rand()%(LENGTH/2)+3;
    for(int i = 0; i<nb_danger; i++){
        clone(&line->tiles[rand()%LENGTH], Maw);
    }
    proba_change_maw(game);
}

void proba_change_spikes(Game* game){
    int proba_change = rand()%10+10;
    if(*((game->proba)+FLESH_SPIKES) <= proba_change){
        *((game->proba)+FLESH_MAW)+= ((*((game->proba)+FLESH_SPIKES)))/5 ;
        *((game->proba)+BONESHOOTER) += (*((game->proba)+FLESH_SPIKES))/5 ;
        *((game->proba)+ACID_RIVER) += (*((game->proba)+FLESH_SPIKES))/5 ;
        *((game->proba)+BARRAGE)+= ((*((game->proba)+FLESH_SPIKES)))/5 ;
        *((game->proba)+SENTINEL_EYE) += (*((game->proba)+FLESH_SPIKES))/5 ;
        *((game->proba)+FLESH_SPIKES) = 0 ;
    } else {
        *((game->proba)+FLESH_SPIKES) -= proba_change ;
        *((game->proba)+FLESH_MAW)+= proba_change/5 ;
        *((game->proba)+BONESHOOTER) += proba_change/5 ;
        *((game->proba)+ACID_RIVER) += proba_change/5 ;
        *((game->proba)+BARRAGE)+= proba_change/5 ;
        *((game->proba)+SENTINEL_EYE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}

void generator_f_spike(Game*game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int nb_spikes = rand()%(LENGTH/2)+5;
    for(int i = 0; i<nb_spikes; i++){
        int pos = rand()%LENGTH;
        if (rand()%3){
            game->mobs = list_append(game->mobs, clone_mob(spike, pos, HEIGTH,(rand()%3+3)*6,rand()%5+3));
        } else {
            game->mobs = list_append(game->mobs, clone_mob(spike_trap, pos, HEIGTH,(rand()%3+3)*6,0));
        }
    }
    int nb_shifts = rand()%3+1;
    int speed=rand()%4+1;
    if (rand()%2){
        for(int i = 0; i <nb_shifts; i++){
            game->mobs = list_append(game->mobs, clone_mob(shift_left, (LENGTH/nb_shifts)*i, HEIGTH,speed,0));
        }
    } else {
        for(int i = 0; i <nb_shifts; i++){
            game->mobs = list_append(game->mobs, clone_mob(shift_right, (LENGTH/nb_shifts)*i, HEIGTH,speed,0));
        }
    }
    proba_change_spikes(game);
}

void proba_change_shooter(Game* game,int nb_lines){
    int proba_change = (rand()%10+5)*nb_lines;
    if(*((game->proba)+BONESHOOTER) <= proba_change){
        *((game->proba)+FLESH_MAW)+= ((*((game->proba)+BONESHOOTER)))/5 ;
        *((game->proba)+FLESH_SPIKES) += (*((game->proba)+BONESHOOTER))/5 ;
        *((game->proba)+ACID_RIVER) += (*((game->proba)+BONESHOOTER))/5 ;
        *((game->proba)+BARRAGE)+= ((*((game->proba)+BONESHOOTER)))/5 ;
        *((game->proba)+SENTINEL_EYE) += (*((game->proba)+BONESHOOTER))/5 ;
        *((game->proba)+BONESHOOTER) = 0 ;
    } else {
        *((game->proba)+BONESHOOTER) -= proba_change ;
        *((game->proba)+FLESH_MAW)+= proba_change/5 ;
        *((game->proba)+FLESH_SPIKES) += proba_change/5 ;
        *((game->proba)+ACID_RIVER) += proba_change/5 ;
        *((game->proba)+BARRAGE)+= proba_change/5 ;
        *((game->proba)+SENTINEL_EYE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}


void add_shooter(Game *game, int pos){
    add_buffer_line(game->carte);
    Mob *mob;
    Line *line = game->carte->head_buffer;
    int x = rand()%(LENGTH-8)+3;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int nb_danger = 4;
    for(int i = 0; i<nb_danger; i++){
        clone(&line->tiles[rand()%LENGTH], Maw);
    }
    if (rand()%3){ 
        game->mobs = list_append(game->mobs, clone_mob(shooter, x, HEIGTH+pos,rand()%4,10+8*x));
        game->projectile=list_append(game->projectile, clone_mob(bone_arrow_left, x, HEIGTH+pos,rand()%4,10));
    }
    else { 
        game->mobs = list_append(game->mobs, clone_mob(shooter_trap_left, x, HEIGTH+pos,rand()%3,0));
        game->projectile=list_append(game->projectile, clone_mob(bone_arrow_left_trap, x, HEIGTH+pos,rand()%2,0));
    }
    clone(&line->tiles[x], Shooter_block_left);
    clone(&line->tiles[x+1], Shooter_block_right);
    if (rand()%3){ 
        mob=clone_mob(shooter, x+1, HEIGTH+pos,rand()%4,0);
        mob->offset_y=1;
        game->mobs = list_append(game->mobs, mob);
        mob=clone_mob(bone_arrow_right, x+1, HEIGTH+pos,rand()%4,10+8*(LENGTH-x-1));
        mob->offset_y=1;
        game->projectile=list_append(game->projectile, mob);
    }
    else { 
        mob=clone_mob(shooter_trap_right, x+1, HEIGTH+pos,rand()%2,0);
        mob->offset_y=1;
        game->mobs = list_append(game->mobs, mob);
        mob=clone_mob(bone_arrow_right_trap, x+1, HEIGTH+pos,rand()%2,0);
        mob->offset_y=1;
        game->projectile=list_append(game->projectile, mob);
    }
    
}
void generator_boneshooter(Game*game){
    int nb_line = rand()%3+1;
    for(int i = 0 ; i < nb_line ; i++){
        add_shooter(game, i);
    }
    proba_change_shooter(game, nb_line);
}

void proba_change_acid(Game* game,int nb_line){
    int proba_change = 10 + (nb_line-1)*(rand()%20+10);
    if(*((game->proba)+ACID_RIVER) <= proba_change){
        *((game->proba)+FLESH_MAW)+= ((*((game->proba)+ACID_RIVER)))/5 ;
        *((game->proba)+FLESH_SPIKES) += (*((game->proba)+ACID_RIVER))/5 ;
        *((game->proba)+BONESHOOTER) += (*((game->proba)+ACID_RIVER))/5 ;
        *((game->proba)+BARRAGE)+= ((*((game->proba)+ACID_RIVER)))/5 ;
        *((game->proba)+SENTINEL_EYE) += (*((game->proba)+ACID_RIVER))/5 ;
        *((game->proba)+ACID_RIVER) = 0 ;
    } else {
        *((game->proba)+ACID_RIVER) -= proba_change ;
        *((game->proba)+FLESH_MAW)+= proba_change/5 ;
        *((game->proba)+FLESH_SPIKES) += proba_change/5 ;
        *((game->proba)+BONESHOOTER) += proba_change/5 ;
        *((game->proba)+BARRAGE)+= proba_change/5 ;
        *((game->proba)+SENTINEL_EYE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}

void add_bone(Game *game, Mob plank,Tile tile, int pos, int speed){
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
            game->mobs = list_append(game->mobs, clone_mob(plank, -1+i, (HEIGTH+pos),speed,0));
            if(plank_lenght<3){
                proba_plank -= 10;
            } else if (plank_lenght<5){
                proba_plank -= 20;
            } else {
                proba_plank -= 30;
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

void generator_acid_river(Game*game){
    int nb_line = rand()%(4)+1;
    Tile tile;
    if(nb_line == 1){
        tile = Acid;
    } else {
        tile = Acid_down;
    }
    int direction = rand()%2;
    for(int i = 0 ; i < nb_line ; i++){
        int speed = rand()%3+1;
        if(direction == i%2){
            add_bone(game, bone_right,tile,i,speed);
        } else {
            add_bone(game,bone_left,tile,i,speed);
        }
        if(i==nb_line-2){
            tile = Acid_up;
        }else{
            tile = Acid_middle;
        }
    }
    proba_change_acid(game,nb_line);
    
}

void proba_change_barrage(Game* game){
    int proba_change = 15 + (rand()%20);
    if(*((game->proba)+BARRAGE) <= proba_change){
        *((game->proba)+FLESH_MAW)+= ((*((game->proba)+BARRAGE)))/5 ;
        *((game->proba)+FLESH_SPIKES) += (*((game->proba)+BARRAGE))/5 ;
        *((game->proba)+BONESHOOTER) += (*((game->proba)+BARRAGE))/5 ;
        *((game->proba)+ACID_RIVER)+= ((*((game->proba)+BARRAGE)))/5 ;
        *((game->proba)+SENTINEL_EYE) += (*((game->proba)+BARRAGE))/5 ;
        *((game->proba)+BARRAGE) = 0 ;
    } else {
        *((game->proba)+BARRAGE) -= proba_change ;
        *((game->proba)+FLESH_MAW)+= proba_change/5 ;
        *((game->proba)+FLESH_SPIKES) += proba_change/5 ;
        *((game->proba)+BONESHOOTER) += proba_change/5 ;
        *((game->proba)+ACID_RIVER)+= proba_change/5 ;
        *((game->proba)+SENTINEL_EYE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}

void generator_barrage(Game*game){
    Line* line;
    int cycle=0;
    int r;
    for (int i=0;i<7;i++){
        r=rand()%3;
        switch (r) {
            case 0:
            add_shooter(game,cycle);
            break;
            case 1:
                add_buffer_line(game->carte);
                line = game->carte->head_buffer;
                for(int i = 0; i<LENGTH; i++){
                    clone(&line->tiles[i], Flesh);
                }
                int nb_spikes = rand()%(LENGTH/2)+5;
                for(int i = 0; i<nb_spikes; i++){
                    int pos = rand()%LENGTH;
                    if (rand()%3){
                        game->mobs = list_append(game->mobs, clone_mob(spike, pos, HEIGTH+cycle,(rand()%3+3)*6,rand()%5+3));
                    } else {
                        game->mobs = list_append(game->mobs, clone_mob(spike_trap, pos, HEIGTH+cycle,(rand()%3+3)*6,0));
                    }
                }
            break;
            case 2:
                add_buffer_line(game->carte);
                line = game->carte->head_buffer;
                for(int i = 0; i<LENGTH; i++){
                    clone(&line->tiles[i], Flesh);
                }
                int nb_danger = rand()%(LENGTH/2)+3;
                for(int i = 0; i<nb_danger; i++){
                    clone(&line->tiles[rand()%LENGTH], Maw);
                }
            break;
        }
        cycle++;
    }
    int nb=rand()%3;
    for (int i=0;i<nb;i++){
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Flesh);
        }
        int nb_danger = rand()%(LENGTH/2)+1;
        for(int i = 0; i<nb_danger; i++){
            clone(&line->tiles[rand()%LENGTH], Maw);
        }    
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Flesh);
        }
        nb_danger = rand()%(LENGTH/2)+1;
        for(int i = 0; i<nb_danger; i++){
            clone(&line->tiles[rand()%LENGTH], Maw);
        }    
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Flesh);
        }
        nb_danger=2;
        int x;
        int speed;
        Mob* Proj;
        if (nb_danger==1){
            speed=rand()%3+1;
            x=rand()%(LENGTH-1);
            game->mobs=list_append(game->mobs,clone_mob(eye_wall,x,HEIGTH+2,speed*16,0));
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
        }else {
            x=rand()%(LENGTH/2-1);
            speed=rand()%3+1;
            game->mobs=list_append(game->mobs,clone_mob(eye_wall,x,HEIGTH+2,speed*16,0));
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            x=rand()%(LENGTH/2-1)+LENGTH/2;
            speed=rand()%3+1;
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            game->mobs=list_append(game->mobs,clone_mob(eye_wall,x,HEIGTH+2,speed*16,0));
        }
    }
    proba_change_barrage(game);
}

void proba_change_eye(Game* game){
    int proba_change = 10 + rand()%30;
    if(*((game->proba)+SENTINEL_EYE) <= proba_change){
        *((game->proba)+FLESH_MAW)+= ((*((game->proba)+SENTINEL_EYE)))/5 ;
        *((game->proba)+FLESH_SPIKES) += (*((game->proba)+SENTINEL_EYE))/5 ;
        *((game->proba)+BONESHOOTER) += (*((game->proba)+SENTINEL_EYE))/5 ;
        *((game->proba)+ACID_RIVER)+= ((*((game->proba)+SENTINEL_EYE)))/5 ;
        *((game->proba)+BARRAGE) += (*((game->proba)+SENTINEL_EYE))/5 ;
        *((game->proba)+SENTINEL_EYE) = 0 ;
    } else {
        *((game->proba)+SENTINEL_EYE) -= proba_change ;
        *((game->proba)+FLESH_MAW)+= proba_change/5 ;
        *((game->proba)+FLESH_SPIKES) += proba_change/5 ;
        *((game->proba)+BONESHOOTER) += proba_change/5 ;
        *((game->proba)+ACID_RIVER)+= proba_change/5 ;
        *((game->proba)+BARRAGE) += proba_change/5 ;
    }
    *((game->proba)+LABYRINTH) += check_percent(game, ACID_RIVER, LABYRINTH);

}

void generator_sentinel_eye(Game*game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int nb_danger = rand()%(LENGTH/2)+1;
    for(int i = 0; i<nb_danger; i++){
        clone(&line->tiles[rand()%LENGTH], Maw);
    } 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int x = rand()%(LENGTH-2);
    int speed=(rand()%3+5)*4;
    game->mobs=list_append(game->mobs,clone_mob(eye,x,HEIGTH+1,speed,0));
    game->projectile=list_append(game->projectile,clone_mob(eye_blast,x,HEIGTH+1,speed,0));
    clone(&line->tiles[x], Maw);         
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    nb_danger = rand()%(LENGTH/2)+1;
    for(int i = 0; i<nb_danger; i++){
        clone(&line->tiles[rand()%LENGTH], Maw);
    }    
    proba_change_eye(game);
}

void proba_change_labyrinth(Game* game){
    *((game->proba)+FLESH_MAW)+= ((*((game->proba)+LABYRINTH))) ;
    *((game->proba)+LABYRINTH) = 0 ;
    *((game->proba)+FLESH_SPIKES) += check_percent(game, ACID_RIVER, LABYRINTH);
}
void generator_labyrinth(Game*game){
    if (LENGTH!=12){
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Flesh);
        }
    } else {
            add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], Flesh);
    }
    int nb_spikes = rand()%(LENGTH/2);
    for(int i = 0; i<nb_spikes; i++){
            int pos = rand()%LENGTH;
            if (rand()%3){
                game->mobs = list_append(game->mobs, clone_mob(spike, pos, HEIGTH,(rand()%3+3)*6,rand()%5+1));
            } else {
                game->mobs = list_append(game->mobs, clone_mob(spike_trap, pos, HEIGTH,(rand()%3+3)*6,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==1||i==2){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+1,30,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==1||i==3||i==4||i==5){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+2,30,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==3||i==5||i==7||i==8||i==9||i==10){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+3,30,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==1||i==2||i==3||i==5||i==6||i==7||i==10){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+4,30,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==10||i==9||i==8){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+5,30,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Flesh);
        }
    }
    proba_change_labyrinth(game);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void initialise_map(Game* game){
    for(int i=0;i<HEIGTH-4;i++){
        update_game(game);
        move_up(game);
        switch_game(game);
        game->score--;
    }
}

Game *initialise_game(int character_id, int tot_exp){
    // explicite
    srand( time( NULL ) );
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(character_id,tot_exp); //0 for now
    game->score = 1;
    game->nb_exp = 0;
    game->exp = NULL;
    game->exp = list_append(game->exp, clone_mob(cible, -80085, 80085, 0, 0));
    game->mobs = NULL;
    game->projectile = NULL;
    game->projectile = list_append(game->projectile, clone_mob(arrow_kabi, -4242, 6969, 0, 0));
    game->proba = initialise_proba();
    game->biome = CROSSY_ROAD;
    game->until_next = 10;
    initialise_map(game);
    return game;
}

void switch_game(Game* game){
    // quand le joueur avance suffisamment, on avance la map, et les mob, et incrément le score
    //printf("%d\n",game->score+1);
    switch_map(game->carte);
    game->score++;
    switch_mob(game->exp);
    switch_mob(game->mobs);
    switch_mob(game->projectile);
    game->joueur->_y -= 32;
    game->joueur->_y_dest -= 32;
    if(game->joueur->character_delay){
        game->joueur->character_delay--;
    }
}


bool move_player(int x, int y, Game* game){
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
    game->joueur->offset_y = 1;
    game->joueur->offset_x += 1;
    game->joueur->offset_x %= 4;
    return move_player(0,1,game);
}

bool move_right(Game* game){
    game->joueur->offset_y = 2;
    game->joueur->offset_x += 1;
    game->joueur->offset_x %= 4;
    return move_player(1,0,game);
}

bool move_left(Game* game){
    game->joueur->offset_y = 3;
    game->joueur->offset_x += 1;
    game->joueur->offset_x %= 4;
    return move_player(-1,0,game);
}

bool move_down(Game* game){
    game->joueur->offset_y = 0;
    game->joueur->offset_x += 1;
    game->joueur->offset_x %= 4;
    return move_player(0,-1,game);
}


bool use_power(Game* game){
    if(game->joueur->character_delay){
        return false;
    } 
    switch(game->joueur->character_id){
        case SNITCH:
            game->joueur->inv_frame = game->joueur->character_value;
            game->joueur->offset_sup += 4;
            game->joueur->nb_frame_to_change = game->joueur->inv_frame;
            game->joueur->character_delay = game->joueur->character_max_delay;
            return true;
            break;
        case KABI:
            if(game->projectile->mob->action_value){
                return false;
            } else {
                game->joueur->offset_sup += 4;
                game->joueur->nb_frame_to_change = 8;
                game->joueur->character_delay = game->joueur->character_max_delay;
                game->projectile->mob->action_value = 1;
                game->projectile->mob->_x = game->joueur->_x;
                game->projectile->mob->_y = game->joueur->_y;
                game->projectile->mob->offset_y = game->joueur->offset_y;
                return true;
            }
            break;

        case OOGWAAY:
            switch(game->joueur->offset_y){
                case 0:
                    if(move_player(0, -2, game)){
                        game->joueur->nb_frame_to_change = 0;
                        game->joueur->character_delay = game->joueur->character_max_delay;
                        return true;
                    } 
                    return false;
                    break;

                 case 1:
                    if(move_player(0, 2, game)){
                        game->joueur->nb_frame_to_change = 0;
                        game->joueur->character_delay = game->joueur->character_max_delay;
                        return true;
                    } 
                    return false;
                    break;

                 case 2:
                    if(move_player(2, 0, game)){
                        game->joueur->nb_frame_to_change = 0;
                        game->joueur->character_delay = game->joueur->character_max_delay;
                        return true;
                    } 
                    return false;
                    break;

                 case 3:
                    if(move_player(-2, 0, game)){
                        game->joueur->nb_frame_to_change = 0;
                        game->joueur->character_delay = game->joueur->character_max_delay;
                        return true;
                    } 
                    return false;
                    break;
            }
            break;
        
        case JOLINAR:
            for(int i = 0;i<game->joueur->character_lvl;i++){
                game->projectile = list_append(game->projectile, clone_mob(spike_growth, -2, game->joueur->_y_dest/32 + game->joueur->character_value + i, 0, 0));
            }
            game->joueur->character_delay = game->joueur->character_max_delay;
            return true;
            break;
            

    }
}

void to_norm(Game* game){
    if(game->joueur->nb_frame_to_change == -1){
        return;
    }else if(game->joueur->nb_frame_to_change){
        game->joueur->nb_frame_to_change--;
    } else {
        switch(game->joueur->character_id){
            case SNITCH:
            case KABI:
                game->joueur->offset_sup=0;
                break;

            case OOGWAAY:
                switch(game->joueur->offset_y){
                case 0:
                case 1 :
                    game->joueur->_x += ((2*(rand()%2)) -1) * 16;
                    break;

                 case 2:
                 case 3:
                    game->joueur->_y += 16;
                    break;
                    
            }
            
        }
        game->joueur->nb_frame_to_change--;

    }
}

bool test_collide(Game* game){
    // test si le joueur rentre dans un mob, avance sur une case danger
    if(a_death_mob_collide(game->mobs, game->joueur)){
        return true;
    }else if(a_death_mob_collide(game->projectile, game->joueur)){
        return true;
    }else if(a_mob_collide(game->mobs, game->joueur)){
        return false;
    }else{
        return IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, game->joueur->_y_dest/32));
    }
}

bool test_death(Game* game){
    if(game->joueur->inv_frame){
        game->joueur->inv_frame--;
        return false;
    } else if(test_collide(game)) {
        if(game->joueur->character_id == KAIRON){
            if(game->joueur->character_value){
                game->joueur->character_value--;
                game->joueur->inv_frame = 64;
                return false;
            } 
        } 
        return true;
    } else {
        return false;
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
    //printf("%d\n",game->score);
    Liste_Mob* cur = game->mobs;
    while(cur!=NULL){
        if(cur->mob->move_x){
            Liste_Mob* now = game->exp->next;
            while(now!=NULL){
                if(mob_collide_mob(cur->mob,now->mob)&&(!cur->mob->delay)){
                    now->mob->_x += cur->mob->move_x;
                }
                now = now->next;
            }
            if(mob_collide(cur->mob, game->joueur)){
                if(cur->mob->delay==0){
                    game->joueur->_x += cur->mob->move_x;
                    game->joueur->_x_dest += cur->mob->move_x;
                }
            }
        }
        cur = cur->next;
    }
    move_all_mob(game->exp,game->joueur);
    move_all_mob(game->mobs,game->joueur);
    move_all_mob(game->projectile,game->joueur);
    if(game->joueur->character_id == KABI){
        add_smoke_liste_proj_collide(game->projectile->next, game->projectile->mob);
        game->projectile->next = add_smoke_liste_mob_collide(game->projectile->next, game->mobs, game->projectile->mob);
        //game->projectile->next = destroy_smoke(game->projectile->next);
        game->projectile->next = destroy_on_smoke(game->projectile->next);
        game->mobs = destroy_on_smoke(game->mobs);
    } else if(game->joueur->character_id == JOLINAR){
        Liste_Mob* act = game->projectile->next;
        while(act!=NULL){
            if(act->mob->id_mob == SPIKE_GROWTH){
                imobilize_liste_mob_collide(game->projectile->next, act->mob);
                imobilize_liste_mob_collide(game->mobs, act->mob);
            }
            act = act->next;
        }
    }
    game->joueur->_x = (game->joueur->_x + game->joueur->_x_dest)/2;
    game->joueur->_y = (game->joueur->_y + game->joueur->_y_dest)/2;
    //SDL_Delay(1000);
    if(game->score%(game->until_next) == 0){
        game->score++;
        game->biome++;
        switch(game->biome){
            case CROSSY_ROAD:
            proba_change_game(game,FOREST);
            game->until_next = 10;
            break;

            case HARD_CROSSY_ROAD:
            proba_change_game(game, BUSHES);
            game->until_next = 10;
            break;

            case FLAMING_MANOR:
            proba_change_game(game, ENTRANCE);
            game->until_next = 10;
            break;

            case CULTIST_CAVE:
            proba_change_game(game, TRANSITION_CAVE);
            game->until_next = 45;
            break;

            case WITHIN_THE_BEAST:
            proba_change_game(game, TRANSITION_FINAL);
            game->until_next = 60;
            break;

            default:
            //game->biome = CROSSY_ROAD;
            //game->until_next = 20;
            break;
        }
    }
    if(a_not_death_mob_collide(game->exp, game->joueur)){
        game->exp = destroy_liste_mob_collide(game->exp, game->joueur);
        if(game->joueur->character_id == RANTON){
            game->nb_exp += (1+game->biome)*2;
            game->score += (1+game->biome)*(game->joueur->character_value);
        } else {
            game->nb_exp += (1+game->biome);
            game->score += (1+game->biome);
        }
    }
    if(game->carte->head_buffer==game->carte->head_map){
        generator_obstacle(game);
        //print_percent(game);
    }
    to_norm(game);
}


void destroy_game(Game *game){
    // Kaboum
    destroy_map(game->carte);
    destroy_liste_mob(game->exp);
    destroy_liste_mob(game->mobs);
    destroy_liste_mob(game->projectile);
    free(game->joueur);
    free(game->proba);
    free(game);
}
