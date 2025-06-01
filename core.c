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
        Mob* exp = clone_mob(xp, x, y, 4, 4);
        exp->offset_x = rand()%4;
        game->exp = list_append(game->exp,exp);
    }
}

void add_row_exp(Game* game, int max_exp, int pos, int min_x, int max_x){
    int nb = rand()%(max_exp+1);
    for(int i = 0; i<nb; i++){
            add_exp(rand()%(max_x+1-min_x) + min_x, HEIGTH+pos, game);
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
    add_row_exp(game, rand()%2+1, pos,1,LENGTH-2);
    add_buffer_line(game->carte);
    Line *line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], road_);
    }
    int init_x = (rand()%3+1)*pos;
    for(int i = 0; i < nb_car; i++){
        game->mobs = list_append(game->mobs, clone_mob(car, ((init_x+(((LENGTH-1)/(nb_car) + car.length ))*i)%(LENGTH + car.length)) - car.length, HEIGTH+pos,speed,0));
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
                nb_car = rand()%3 + 2;
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
    for(int i = 0; i <= LENGTH; i++){
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
    add_row_exp(game, rand()%2+1, pos,1,LENGTH-2);
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
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed,(LENGTH*32)/16);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed,(LENGTH*32)/16);
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

        case FIRE_PATROL:
        generator_fire_patrol(game);
        break;

        case FIRELINE:
        generator_fireline(game);
        break;

        // case CAVE:
        // generator_cave(game);
        // break;

        case CULTIST:
        generator_cultist(game);
        break;
                
        case MAGE:
        generator_mage(game);
        break;

        case TRANSITION_CAVE:
        generator_transition_cave(game);
        break;

        case TRAP:
        generator_trap(game);
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

/////BIOME FEEEEEUUUUUUUUUUU/////////
void proba_change_entrance(Game* game){
    for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *((game->proba)+FIRE) = 50;
    *((game->proba)+FIRELINE) = 50;

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

void proba_change_fire(Game* game, int nb_line){
    int proba_change = 10 + ((nb_line-1)/2)*(rand()%5+5);
    if(*((game->proba)+FIRE) <= proba_change){
        *((game->proba)+FIRELINE) += (2*(*((game->proba)+FIRE)))/3 ;
        *((game->proba)+FIRE_PATROL) += ((*((game->proba)+FIRE)))/3 ;
        *((game->proba)+FIRE) = 0;
    } else {
        *((game->proba)+FIRE) -= proba_change ;
        *((game->proba)+FIRELINE) += (2*proba_change)/3 ;
        *((game->proba)+FIRE_PATROL) += (proba_change)/3 ;
    }
    *((game->proba)+FIRE_PATROL) += check_percent(game, FIRELINE, FIRE);
}

void add_fire(Game* game, int decal, int nb_flames){
    for(int i = 0; i<nb_flames; i++){
        int direction=rand()%2;
        int pos = rand()%(LENGTH)-1;
        int speed = rand()%3;
        Mob* fire;
        if(direction){
            fire = clone_mob(fire_clock, pos, HEIGTH+decal,speed,16*((rand()%2)+1));
        } else { 
            fire = clone_mob(fire_counter, pos, HEIGTH+decal,speed,16*((rand()%2)+1));
        }
        fire ->memory__x = -18;
        for(int i =0; i < rand()%4; i++){
             fire->delay = 0;
             (fire->path)(fire,game->joueur);
        }
        game->projectile = list_append(game->projectile, fire);
    }

}

void generator_fire(Game *game){
    //feu
    int nb_inside = rand()%4 + 1;
    Line* line;
    for(int i = 0; i < nb_inside; i++){
        for(int i = 0; i < 2; i++){
            add_buffer_line(game->carte);
            line = game->carte->head_buffer;
            for(int i = 0; i<LENGTH; i++){
                clone(&line->tiles[i], planks);
            }
        }
        int nb_Block = rand()%7;
        for(int j = 0; j<nb_Block; i++){
            int coinflip = rand()%2;
            if(coinflip){
                clone(&(line->next)->tiles[rand()%LENGTH], brokenplanks);
            } else {
                clone(&line->tiles[rand()%LENGTH], brokenplanks);
            }
        }
        int nb_flames = rand()%4+1;
        add_fire(game,2*i,nb_flames);
        }
    proba_change_fire(game,nb_inside);
}

void add_one_fire(Game* game, int x, int y){
   Mob* firey = clone_mob(fire_still, x, y,0,0);
    firey->memory__x = -18;
    firey->offset_x = rand()%4;
    game->projectile = list_append(game->projectile, firey);
}

void add_fire_unit(Game *game, int init_x, int direction, int pos, int speed, int nb_fire){
    Mob* firey;
    for(int i = 0; i < nb_fire; i++){
        if(direction){
            firey = clone_mob(fire_right, ((init_x+(((LENGTH-1)/(nb_fire) + 1 ))*i)%(LENGTH + 1)) - 1, HEIGTH+pos,speed,0);
        } else {
            firey = clone_mob(fire_left, ((init_x+(((LENGTH-1)/(nb_fire) + 1 ))*i)%(LENGTH + 1)) - 1, HEIGTH+pos,speed,0);
        }
        firey->memory__x = -18;
        firey->offset_x = rand()%4;
        game->projectile = list_append(game->projectile, firey);

    }
}

void add_fire_row(Game *game, int direction, int pos){
    Mob* firey;
    if(!direction){
        for(int i = 2; i < LENGTH; i++){
            firey = clone_mob(fire_still, i, HEIGTH+pos,0,0);
            firey->memory__x = -18;
            firey->offset_x = rand()%4;
            game->projectile = list_append(game->projectile, firey);
        }
    } else {
        for(int i = 0; i < LENGTH-2; i++){
            firey = clone_mob(fire_still, i, HEIGTH+pos,0,0);
            firey->memory__x = -18;
            firey->offset_x = rand()%4;
            game->projectile = list_append(game->projectile, firey);
        }
    }
}

void proba_change_fireline(Game *game, int nb_line){
    int proba_change = 10 + (nb_line-1)*(rand()%8+10);
    if(*((game->proba)+FIRELINE) <= proba_change){
        *((game->proba)+FIRE) += ((*((game->proba)+FIRELINE)))/2 ;
        *((game->proba)+FIRE_PATROL) += ((*((game->proba)+FIRELINE)))/2 ;
        *((game->proba)+FIRELINE) = 0 ;
    } else {
        *((game->proba)+FIRELINE) -= proba_change ;
        *((game->proba)+FIRE) += proba_change/2 ;
        *((game->proba)+FIRE_PATROL) += proba_change/2 ;
    }
    *((game->proba)+FIRE_PATROL) += check_percent(game, FIRELINE, FIRE);
}

void generator_fireline(Game *game){
    // explicite
    int direction = rand()%2;
    int where_enter = direction*(LENGTH-1);
    int nb_inside = rand()%2+1;
    int pos = 0;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%3, direction, pos,rand()%3+1,rand()%3+1);
    pos++;
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_row(game, direction, pos);
    pos++;
    direction = 1 - direction;
    for(int i = 0 ; i < 2*nb_inside ; i+=1){
        int speed = rand()%4 ;
        int init_x = rand()%3;
        int nb_fire = rand()%3 +1;
        for(int j = 0; j<2;j++){
            add_buffer_line(game->carte);
            line = game->carte->head_buffer;
            for(int i = 0; i<LENGTH; i++){
                clone(&line->tiles[i], planks);
            }
            add_fire_unit(game, init_x,direction, pos, speed, nb_fire);
            add_fire_unit(game, init_x-1,direction, pos, speed, nb_fire);
            pos++;
        }
        direction = 1 - direction;
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_row(game, direction, pos);
    pos++;
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%4, direction, pos,rand()%3+1,rand()%3+1);
    proba_change_fire(game, nb_inside);
}



void proba_change_fire_patrol(Game* game){
 int proba_change = 12 + rand()%9;
    if(*((game->proba)+FIRE_PATROL) <= proba_change){
        *((game->proba)+FIRELINE) += (2*(*((game->proba)+FIRE)))/3 ;
        *((game->proba)+FIRE) += ((*((game->proba)+FIRE)))/3 ;
        *((game->proba)+FIRE_PATROL) = 0;
    } else {
        *((game->proba)+FIRE_PATROL) -= proba_change ;
        *((game->proba)+FIRELINE) += (2*proba_change)/3 ;
        *((game->proba)+FIRE) += (proba_change)/3 ;
    }
    *((game->proba)+FIRELINE) += check_percent(game, FIRELINE, FIRE);

}


void generator_fire_patrol(Game *game){
    int type = rand()%2;
    if(type){
        generator_fire_patrol_1(game);
    } else {
        generator_fire_patrol_2(game);
    }
    proba_change_fire_patrol(game);

}

void generator_fire_patrol_1(Game *game){
    //les cultistes avec des dagues
    int direction = rand()%2;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%3, direction, 0,rand()%3+1,rand()%3+1);
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
    int pos_fire=rand()%(LENGTH-lat_size);
    int nb_fire=rand()%2+1;
    for (int i=0;i<nb_lines;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int j = 0; j<LENGTH; j++){
             clone(&line->tiles[j], planks);
            if (!((j>=pos_fire && j<=(pos_fire+lat_size-2))&&(i==0||i==(nb_lines-1)||j==(pos_fire+lat_size-2)||j==pos_fire))) {
                add_one_fire(game, j, i+HEIGTH+1);
            }
        }
    }
    int speed=(rand()%3);
    Mob* firey = clone_mob(fire_still, pos_fire+2, HEIGTH+nb_lines,speed,16);
    switch (type) {
        case 0:
        if (sens==0){
            firey -> path = patrol_clock4x3;
        }
        else {
            firey -> _x -= 32;
            firey -> path = patrol_counter4x3;
        }
        break;
        case 2:
        if (sens==0){
            firey -> _x += 32;
            firey -> path = patrol_clock5x3;
        }
        else {
            firey -> _x -= 32;
            firey -> path = patrol_counter5x3;
        }
        break;
        case 1:
        if (sens==0){
            firey -> path = patrol_clock4x4;
        }
        else {
            firey -> _x -= 32;
            firey -> path = patrol_counter4x4;
        }
        break;
    }
    firey -> memory__x = -64;
    game->projectile=list_append(game->projectile,firey);
    if (nb_fire==2) {
        firey = clone_mob(fire_still, pos_fire+2, HEIGTH+nb_lines,speed,16);
        firey -> _y = (HEIGTH + 1)*32;
        switch (type) {
            case 0:
                if (sens==0){
                    firey -> path = patrol_clock4x3;
                    firey -> _x = (pos_fire+1)*32;
                }
                else {
                    firey -> path = patrol_counter4x3;
                    firey -> _x = (pos_fire+2)*32;
                }
                break;
            case 2:
                if (sens==0){
                    firey -> _x = (pos_fire+1)*32;
                    firey -> path = patrol_clock5x3;
                }
                else {
                    firey -> _x = (pos_fire+3)*32;
                    firey -> path = patrol_counter5x3;
                }
                break;
            case 1:
                if (sens==0){
                    firey -> _x = (pos_fire+1)*32;
                    firey -> path = patrol_clock4x4;
                }
                else {
                    firey -> _x = (pos_fire+2)*32;
                    firey -> path = patrol_counter4x4;
                }
                break;
        }
        firey->action_value=2;
        firey->offset_x = rand()%4;
        game->projectile=list_append(game->projectile,firey);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%4, !direction, nb_lines+1,rand()%3+1,rand()%3+1);
}


void generator_fire_patrol_2(Game *game){
    int direction = rand()%2;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%3, direction, 0,rand()%3+1,rand()%3+1);
    int type=rand()%4;
    int sens=rand()%2;
    int pos_b=rand()%(LENGTH-5);
    int nb_lines = 4;
    for (int i=0;i<nb_lines;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int j = 0; j<LENGTH; j++){
            clone(&line->tiles[j], planks);
            if(j<pos_b ||j>pos_b+4){
                add_one_fire(game, j, i+HEIGTH+1);;
            }
            else {
                switch (type){
                case 0:
                    if (i==1){
                        if (j==1+pos_b ||j==3+pos_b){
                            add_one_fire(game, j, i+HEIGTH+1);
                        }
                    }
                    if (i==3){
                        if (j==0+pos_b ||j==2+pos_b||j==4+pos_b){
                            add_one_fire(game, j, i+HEIGTH+1);
                        }
                    }
                    break;
                case 1:
                    if (i==1){
                        if (j==2+pos_b){
                            add_one_fire(game, j, i+HEIGTH+1);
                        }
                    }
                    if (i==3){
                        if (j==0+pos_b ||j==4+pos_b){
                            add_one_fire(game, j, i+HEIGTH+1);
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
                            add_one_fire(game, j, i+HEIGTH+1);
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
                            add_one_fire(game, j, i+HEIGTH+1);
                        }
                    }
                    if (i==3){
                        if (j==pos_b ||j==1+pos_b ||j==3+pos_b||j==4+pos_b){
                            add_one_fire(game, j, i+HEIGTH+1);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    int speed=(rand()%3);
    Mob* firey_feet = clone_mob(fire_still, pos_b, HEIGTH+2,speed,16);
    Mob* firey_head = clone_mob(fire_still, pos_b, HEIGTH+3,speed,16);
    switch (type) {
            case 0:
                if (sens==0){
                    firey_head -> path = babel1_1;
                    firey_feet -> path = babel1_1;
                }
                else{
                    firey_head -> _x += 128;
                    firey_head -> path = babel1_2;
                    firey_feet-> _x += 128;
                    firey_feet -> path = babel1_2;
                }
                break;
            case 1:
                if (sens==0){
                    firey_head -> path = babel2_1;
                    firey_feet -> path = babel2_1;                    
                }
                else{
                    firey_head -> _x += 128;
                    firey_head -> path = babel2_2;
                    firey_feet-> _x += 128;
                    firey_feet -> path = babel2_2;                    
                }
                break;
            case 2:
                if (sens==0){
                    firey_head -> _x += 64;
                    firey_head -> _y += 32;
                    firey_head -> path = babel3_1;
                    firey_feet -> _x += 64;
                    firey_feet -> _y += 32;
                    firey_feet -> path = babel3_1;                    
                }
                else{
                    firey_head -> _x += 32;
                    firey_head -> path = babel3_2;
                    firey_feet-> _x += 32;
                    firey_feet -> path = babel3_2;
                }
                break;
            case 3:
                if (sens==0){
                    firey_head -> _x += 64;
                    firey_head -> _y -= 32;
                    firey_head -> path = babel4_1;
                    firey_feet-> _x += 64;
                    firey_feet -> _y -= 32;
                    firey_feet -> path = babel4_1;
                }
                else{
                    firey_head -> _x += 32;
                    firey_head -> path = babel4_2;
                    firey_feet-> _x += 32;
                    firey_feet -> path = babel4_2;
                }
                break;
            default:
                break;
    }
    firey_head -> offset_x = rand()%4;
    firey_feet -> offset_x = rand()%4;
    game->projectile = list_append(game->projectile, firey_head);
    game->projectile = list_append(game->projectile, firey_feet);
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    add_fire_unit(game, rand()%4, !direction, nb_lines+1,rand()%3+1,rand()%3+1);
}

void proba_change_transition_cave(Game* game){
        for(int i = 0; i<NB_OBSTACLE; i++){
        *(game->proba+i) = 0;
    }
    *((game->proba)+CULTIST) = 50;
    *((game->proba)+BABEL) = 25;
    *((game->proba)+TRAP) = 25;
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
    // int proba_change = (rand()%4)*5 + 10;
    // if(*((game->proba)+CAVE) <= proba_change){
    //     //*((game->proba)+CULTIST) += (2*(*((game->proba)+CAVE)))/5+1 ;
    //     //*((game->proba)+BABEL) += ((*((game->proba)+CAVE)))/5 ;
    //     //*((game->proba)+STONE) += ((*((game->proba)+CAVE)))/5 ;
    //     *((game->proba)+MAGE) += (*((game->proba)+CAVE))/5 ;
    //     *((game->proba)+CAVE) = 0 ;
    // } else {
    //     //*((game->proba)+CULTIST) += 2*proba_change/5+1 ;
    //     *((game->proba)+CAVE) -= proba_change ;
    //     *((game->proba)+MAGE) += proba_change/5 ;
    //     //*((game->proba)+STONE) += proba_change/5 ;
    //     *((game->proba)+MAGE) += proba_change/5 ;
    // }
    //*((game->proba)+CAVE) += check_percent(game, CAVE, BABEL);
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




void proba_change_mage(Game* game){
   int proba_change = 10 + rand()%12;
    if(*((game->proba)+MAGE) <= proba_change){
        *((game->proba)+TRAP) += ((*((game->proba)+MAGE)))/3 ;
        *((game->proba)+BABEL) += ((*((game->proba)+MAGE)))/3 ;
        *((game->proba)+CULTIST) += ((*((game->proba)+MAGE)))/3 ;
        *((game->proba)+MAGE) = 0;
    } else {
        *((game->proba)+MAGE) -= proba_change ;
        *((game->proba)+TRAP) += proba_change/3 ;
        *((game->proba)+BABEL) += proba_change/3 ;
        *((game->proba)+CULTIST) += proba_change/3;
    }
    *((game->proba)+MAGE) += check_percent(game, CULTIST, TRAP);
}

void generator_mage(Game *game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    add_buffer_line(game->carte);
    int pos_x=rand()%LENGTH;
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==pos_x){
            clone(&line->tiles[i], base);
        }else {
            clone(&line->tiles[i], channel);
        }
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    int speed= rand()%4 +1;
    int advance = 8 * (rand()%5 +2);
    int max_action_delay = advance - 1;
    Mob *c=clone_mob(mage, pos_x, HEIGTH+1,speed,max_action_delay);
    game->mobs = list_append(game->mobs, c);
    c->offset_x = 3;
    Mob *arrow=clone_mob(curse_arrow, pos_x, HEIGTH+1,speed,max_action_delay);
    game->projectile = list_append(game->projectile, arrow);
    proba_change_mage(game);
}



////New_cave////

void proba_change_trap(Game* game){
    int proba_change = 10 + rand()%12;
    if(*((game->proba)+TRAP) <= proba_change){
        *((game->proba)+MAGE) += ((*((game->proba)+TRAP)))/3 ;
        *((game->proba)+BABEL) += ((*((game->proba)+TRAP)))/3 ;
        *((game->proba)+CULTIST) += ((*((game->proba)+TRAP)))/3 ;
        *((game->proba)+TRAP) = 0;
    } else {
        *((game->proba)+TRAP) -= proba_change ;
        *((game->proba)+MAGE) += proba_change/3 ;
        *((game->proba)+BABEL) += proba_change/3 ;
        *((game->proba)+CULTIST) += proba_change/3;
    }
    *((game->proba)+TRAP) += check_percent(game, CULTIST, TRAP);

}

void generator_trap(Game* game){
    int direction = rand()%2;
    int init_x = rand()%(LENGTH-4) + 2;
    int pos = 0;
    int speed = rand()%9+1;
    int max_action_delay = rand()%13 + 4 ;
    Line* line;
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    if(direction){
        clone(&line->tiles[LENGTH-1], cave);

    } else {
        clone(&line->tiles[0], cave);
    }
    pos++;
    Mob* caillou;
    if(direction){
        caillou = clone_mob(pebble_right, LENGTH+3, HEIGTH+pos, speed, max_action_delay);
    } else {
        caillou = clone_mob(pebble_left, -4, HEIGTH+pos, speed, max_action_delay);
    }
    caillou->memory__x = init_x*32;
    caillou->memory__y = (HEIGTH + pos + 3)*32;
    caillou->heigth = 1;
    caillou->length=1;
    game->projectile = list_append(game->projectile, caillou);
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    pos++;
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    pos++;
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    if(direction){
        clone(&line->tiles[0], cave);
    } else {
        clone(&line->tiles[LENGTH-1], cave);
    }
    pos++;
    game->mobs = list_append(game->mobs, clone_mob(sentinel,init_x,HEIGTH+pos,speed,max_action_delay));
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[1], stalagmite);
    clone(&line->tiles[LENGTH-2], stalagmite);
    if(direction){
        clone(&line->tiles[LENGTH-1], stalagmite);

    } else {
        clone(&line->tiles[0], stalagmite);
    }
    pos++;
    Mob* clever = clone_mob(lever,(LENGTH-2)/2,HEIGTH+pos,speed,max_action_delay);
    clever->memory__y = (HEIGTH + pos -1)*32;
    clever->memory__x = (init_x)*32;
    game->projectile = list_append(game->projectile, clever);
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    if(!direction){
        clone(&line->tiles[LENGTH-1], cave);

    } else {
        clone(&line->tiles[0], cave);
    }
    clone(&line->tiles[(LENGTH-2)/2], _lever);
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    proba_change_trap(game);

}

//ROOMs Generator 

void generator_room_0(Game* game){
    //Première ligne blocante
    int direction = rand()%2;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[(LENGTH-1)/2], cave);
    clone(&line->tiles[(LENGTH-1)/2 +1], cave);
    //2nd ligne libre
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    //3ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*2], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*4], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*7], stalagmite);
    //4ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*1], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*2], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*4], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*5], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*7], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*8], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*9], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*10], stalagmite);
    //5ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    //6ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    clone(&line->tiles[rand()%(LENGTH)], stalagmite);
    //Dernière ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }

}

void generator_room_1(Game* game){
    //Première ligne blocante
    int direction = rand()%2;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[(LENGTH-1)/2], cave);
    clone(&line->tiles[(LENGTH-1)/2 +1], cave);
    //2nd ligne libre
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*4], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*7], stalagmite);
    //3ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*1], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*4], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*7], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*8], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*9], stalagmite);
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*10], stalagmite);
    //4ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1) + (-1 + 2*direction)*1], stalagmite);
    //5ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[(1-direction)*(LENGTH-1)], cave);
    //6ème ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[rand()%(LENGTH-2)+1], stalagmite);
    clone(&line->tiles[rand()%(LENGTH-2)+1], stalagmite);
    clone(&line->tiles[rand()%(LENGTH-2)+1], stalagmite);
    //Dernière ligne 
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }

}


void generator_room_c0(Game* game){
    //Première ligne blocante
    Line* line = game->carte->head_map->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[(LENGTH-1)/2], cave);
    //2nd ligne libre
    line = line->prev;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    //3ème ligne 
        line = line->prev;

    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[1], stalagmite);
    clone(&line->tiles[2], stalagmite);
    clone(&line->tiles[3], stalagmite);
    clone(&line->tiles[5], stalagmite);

    //4ème ligne 
        line = line->prev;

    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[1], stalagmite);
    clone(&line->tiles[2], stalagmite);
    clone(&line->tiles[3], stalagmite);
    clone(&line->tiles[5], stalagmite);
    //5ème ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[2], stalagmite);
    clone(&line->tiles[5], stalagmite);
    //6ème ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[1],  cave);
    clone(&line->tiles[2],  cave);
    clone(&line->tiles[3],  cave);
    //Dernière ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
}

void generator_room_c1(Game* game){
    //Première ligne blocante
    Line* line = game->carte->head_map->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[0], cave);
    //2nd ligne libre
    line = line->prev;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[5], stalagmite);
    //3ème ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[1], stalagmite);
    clone(&line->tiles[3], stalagmite);
    clone(&line->tiles[5], stalagmite);
    //4ème ligne 
        line = line->prev;

    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[1], stalagmite);
    clone(&line->tiles[3], stalagmite);
    clone(&line->tiles[5], stalagmite);
    //5ème ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[3], stalagmite);
    clone(&line->tiles[5], stalagmite);
    //6ème ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[0],  stalagmite);
    clone(&line->tiles[1],  stalagmite);
    clone(&line->tiles[5],  stalagmite);
    //Dernière ligne 
    line = line->prev;
    for(int i = 0; i<LENGTH/2; i++){
        clone(&line->tiles[i], cave);
    }

}

void generator_room_s0(Game* game){
    //Première ligne blocante
    Line* line = game->carte->head_map->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[LENGTH/2], cave);
    //2nd ligne libre
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    //3ème ligne 
        line = line->prev;

    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[8], stalagmite);
    clone(&line->tiles[9], stalagmite);
    clone(&line->tiles[10], stalagmite);
    clone(&line->tiles[6], stalagmite);

    //4ème ligne 
        line = line->prev;

    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[8], stalagmite);
    clone(&line->tiles[9], stalagmite);
    clone(&line->tiles[10], stalagmite);
    clone(&line->tiles[6], stalagmite);
    //5ème ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[9], stalagmite);
    clone(&line->tiles[6], stalagmite);
    //6ème ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[8],  cave);
    clone(&line->tiles[9],  cave);
    clone(&line->tiles[10], cave);
    //Dernière ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
}

void generator_room_s1(Game* game){
    //Première ligne blocante
    Line* line = game->carte->head_map->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], stalagmite);
    }
    clone(&line->tiles[LENGTH-1], cave);
    //2nd ligne libre
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[6], stalagmite);
    //3ème ligne 
        line = line->prev;

    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[6], stalagmite);
    clone(&line->tiles[8], stalagmite);
    clone(&line->tiles[10], stalagmite);
    //4ème ligne 
        line = line->prev;

    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[6], stalagmite);
    clone(&line->tiles[8], stalagmite);
    clone(&line->tiles[10], stalagmite);
    //5ème ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[6], stalagmite);
    clone(&line->tiles[8], stalagmite);
    //6ème ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    clone(&line->tiles[6],  stalagmite);
    clone(&line->tiles[10],  stalagmite);
    clone(&line->tiles[11], stalagmite);
    //Dernière ligne 
    line = line->prev;
    for(int i = LENGTH/2; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }

}




void generator_room_cultist(Game* game, int c, int s){
    for(int i = 0; i<7; i++){
        add_buffer_line(game->carte);
    }
    switch(c){
        case 0:
            generator_room_c0(game);
            break;

        case 1:
            generator_room_c1(game);
            break;
    }
    switch(s){
        case 0:
            generator_room_s0(game);
            break;

        case 1:
            generator_room_s1(game);
            break;
    }
}

void proba_change_babel(Game* game){
    int proba_change = 13 + rand()%21;
    if(*((game->proba)+BABEL) <= proba_change){
        *((game->proba)+MAGE) += ((*((game->proba)+BABEL)))/3 ;
        *((game->proba)+CULTIST) += ((*((game->proba)+BABEL)))/3 ;
        *((game->proba)+TRAP) += ((*((game->proba)+BABEL)))/3 ;
        *((game->proba)+BABEL) = 0;
    } else {
        *((game->proba)+BABEL) -= proba_change ;
        *((game->proba)+MAGE) += proba_change/3 ;
        *((game->proba)+CULTIST) += proba_change/3 ;
        *((game->proba)+TRAP) += proba_change/3 ;
    }
    *((game->proba)+BABEL) += check_percent(game, CULTIST, TRAP);

}

void generator_babel(Game* game){
    int speed = rand()%4+3;
    int max_action_delay = rand()%9 + 2 ;
    int room = rand()%5;
    switch(room){
        case 0:
        case 2:
            generator_room_0(game);
            break;

        case 1:
        case 3:
            generator_room_1(game);
            break;

        case 4:
            generator_room_c0(game);
            break;
    }
    Mob * bebert = clone_mob(babel, 5, HEIGTH+(4), speed, max_action_delay);
    bebert -> memory__y = (HEIGTH+(6))*32;
    game->mobs = list_append(game->mobs, bebert);
    proba_change_babel(game);
}

void proba_change_cultist(Game* game){
    int proba_change = 10 + rand()%12;
    if(*((game->proba)+CULTIST) <= proba_change){
        *((game->proba)+MAGE) += ((*((game->proba)+CULTIST)))/3 ;
        *((game->proba)+BABEL) += ((*((game->proba)+CULTIST)))/3 ;
        *((game->proba)+TRAP) += ((*((game->proba)+CULTIST)))/3 ;
        *((game->proba)+CULTIST) = 0;
    } else {
        *((game->proba)+CULTIST) -= proba_change ;
        *((game->proba)+MAGE) += proba_change/3 ;
        *((game->proba)+BABEL) += proba_change/3 ;
        *((game->proba)+TRAP) += proba_change/3;
    }
    *((game->proba)+CULTIST) += check_percent(game, CULTIST, TRAP);
}

void generator_cultist(Game* game){
    int c = rand()%2;
    int s = rand()%2;
    int tour_max;
    int init_x;
    int init_y;
    int nb_tour;
    generator_room_cultist(game, c, s);
    Mob* cultisto;
    switch(c){
        case 0:
            tour_max = 15;
            init_x = 2;
            init_y = HEIGTH+5;
            break;

        case 1:
            tour_max = 22;
            init_x = 3;
            init_y = HEIGTH + 5;
            break;
    }
    int speed = rand()%3+1;
    int direction = rand()%2;
    int nb_cultisto = rand()%3+2;
        int init = rand()%tour_max;
    for(int nb = 0; nb < nb_cultisto;nb++){
         cultisto = clone_mob(cultist, init_x, init_y, speed, c);
         cultisto->offset_y = 2 + direction;
         switch(nb){
            case 0:
                nb_tour = init;
                break;

            case 1:
                nb_tour = (tour_max)/2 + init + rand()%3;
                break;

            default:
                nb_tour = rand()%tour_max;
                break;
         }
         for(int i = 0;i<nb_tour;i++){
            for(int i = 0; i<16;i++){
                cultisto->delay = 0;
                (cultisto->path)(cultisto,game->joueur);
            }
         }
         game->mobs = list_append(game->mobs, cultisto);
    }
    switch(s){
        case 0:
            tour_max = 15;
            init_x = 9;
            init_y = HEIGTH+5;
            break;

        case 1:
            tour_max = 22;
            init_x = 8;
            init_y = HEIGTH + 5;
            break;
    }
    direction = rand()%2;
    nb_cultisto = rand()%3+2;
    init = rand()%tour_max;
    for(int nb = 0; nb < nb_cultisto;nb++){
         cultisto = clone_mob(cultist, init_x, init_y, speed, s);
         cultisto->offset_y = 2 + direction;
         cultisto->action_delay=0;
        switch(nb){
            case 0:
                nb_tour = init;
                break;
            case 1:
                nb_tour = (tour_max)/2 + init + rand()%3;
                break;
            default:
                nb_tour = rand()%tour_max;
                break;
        }  
         for(int i = 0;i<(nb_tour);i++){
             for(int j = 0; j<16;j++){
                cultisto->delay = 0;
                (cultisto->path)(cultisto,game->joueur);
            }
         }
         game->mobs = list_append(game->mobs, cultisto);
    }
    proba_change_cultist(game);
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
     int init_x = (rand()%3+1)*pos;
    for(int i = 0; i < nb_car; i++){
        game->mobs = list_append(game->mobs, clone_mob(car, ((init_x+(((LENGTH-1)/(nb_car) + car.length ))*i)%(LENGTH + car.length)) - car.length, HEIGTH+pos,speed,0));
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
    for(int i = 0; i <= LENGTH; i++){
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
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed,(LENGTH*32)/16);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed,(LENGTH*32)/16);
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
    int nb_shifts = rand()%7+2;
    int speed=rand()%4;
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
        mob=clone_mob(shooter_trap_left, x, HEIGTH+pos,rand()%3,0);
        mob->offset_x=1;
        game->mobs = list_append(game->mobs, mob);
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
        mob->offset_x=1;
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
    for(int i = 0; i <= LENGTH; i++){
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
        int nb_danger = rand()%(LENGTH/2-1);
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
            Mob* mob=clone_mob(eye_wall,x,HEIGTH+2+cycle,speed*16,0);
            game->mobs=list_append(game->mobs,mob);
            mob=clone_mob(eye_wall,x+1,HEIGTH+2+cycle,speed*16,0);
            mob->offset_x++;
            game->mobs=list_append(game->mobs,mob);
            Proj=clone_mob(eye_drop,x,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
        }else {
            x=rand()%(LENGTH/2-1);
            speed=rand()%3+1;
            Mob* mob=clone_mob(eye_wall,x,HEIGTH+2+cycle,speed*16,0);
            game->mobs=list_append(game->mobs,mob);
            mob=clone_mob(eye_wall,x+1,HEIGTH+2+cycle,speed*16,0);
            mob->offset_x++;
            game->mobs=list_append(game->mobs,mob);
            Proj=clone_mob(eye_drop,x,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            x=rand()%(LENGTH/2-1)+LENGTH/2;
            speed=rand()%3+1;
            Proj=clone_mob(eye_drop,x,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=16;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=32;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=48;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            Proj=clone_mob(eye_drop,x+1,HEIGTH+2+cycle,speed,80);
            Proj->action_delay=64;
            Proj->_x=-32;
            game->projectile=list_append(game->projectile,Proj);
            speed=rand()%3+1;
            mob=clone_mob(eye_wall,x,HEIGTH+2+cycle,speed*16,0);
            game->mobs=list_append(game->mobs,mob);
            mob=clone_mob(eye_wall,x+1,HEIGTH+2+cycle,speed*16,0);
            mob->offset_x++;
            game->mobs=list_append(game->mobs,mob);
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
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+1,64,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==1||i==3||i==4||i==5){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+2,64,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==3||i==5||i==7||i==8||i==9||i==10){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+3,64,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==0||i==1||i==2||i==3||i==5||i==6||i==7||i==10){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+4,64,0));
            }
        }
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], Maw);
            if (i==10||i==9||i==8){
                clone(&line->tiles[i], Flesh);
                game->mobs=list_append(game->mobs,clone_mob(spike_trap,i,HEIGTH+5,64,0));
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

Game *initialise_game(int character_id, int tot_exp, int short_mode){
    // explicite
    srand( time( NULL ) );
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(character_id,tot_exp); //0 for now
    game->score = 0;
    game->nb_exp = 0;
    game->exp = NULL;
    game->exp = list_append(game->exp, clone_mob(cible, -80085, 80085, 0, 0));
    game->mobs = NULL;
    game->projectile = NULL;
    game->projectile = list_append(game->projectile, clone_mob(arrow_kabi, -4242, 6969, 0, 0));
    game->proba = initialise_proba();
    game->biome = CROSSY_ROAD;
    game->short_mode = short_mode;
    game->step = 1;
    if(game->short_mode){
        game->until_next = 51;
    } else {
        game->until_next = 101;
    }
    initialise_map(game);
    return game;
}

void switch_game(Game* game){
    // quand le joueur avance suffisamment, on avance la map, et les mob, et incrément le score
    //printf("%d\n",game->score+1);
    switch_map(game->carte);
    game->score++;
    game->step++;
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

        default:
            return false;
            
    }
    return false;
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
                case 0 :
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
    } else if (game->joueur->character_id == OOGWAAY){
        if(game->joueur->character_delay){
            game->joueur->character_delay--;
        }
    }
    game->joueur->_x = (game->joueur->_x + game->joueur->_x_dest)/2;
    game->joueur->_y = (game->joueur->_y + game->joueur->_y_dest)/2;
    //SDL_Delay(1000);
    if(game->step%(game->until_next) == 0){
        game->step = 1;
        if(game->biome != NB_BIOME-1){
            game->biome++;
        }
        else{
            game->biome = CROSSY_ROAD;
        }
        switch(game->biome){
            case CROSSY_ROAD:
            proba_change_game(game,FOREST);
            if(game->short_mode){
                game->until_next = 51;
            } else {
                game->until_next = 101;
            }
            break;

            case HARD_CROSSY_ROAD:
            proba_change_game(game, BUSHES);
            if(game->short_mode){
                game->until_next = 51;
            } else {
                game->until_next = 101;
            }
            break;

            case FLAMING_MANOR:
            proba_change_game(game, ENTRANCE);
            if(game->short_mode){
                game->until_next = 51;
            } else {
                game->until_next = 101;
            }
            break;

            case CULTIST_CAVE:
            proba_change_game(game, TRANSITION_CAVE);
            if(game->short_mode){
                game->until_next = 51;
            } else {
                game->until_next = 101;
            }
            break;

            case WITHIN_THE_BEAST:
            proba_change_game(game, TRANSITION_FINAL);
            if(game->short_mode){
                game->until_next = 51;
            } else {
                game->until_next = 101;
            }
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
