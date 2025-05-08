#include "map.h"
#include "core.h"
#include "mob.h"
#include "tuiles.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

////Partie Création d'obstacles !///


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
            int nb_bus;
            if(speed==7){
                nb_bus = rand()%3 + 1;
            } else if(speed==2){
                nb_bus = 1;
            } else {
                nb_bus = rand()%2 + 1;
            }
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
            clone(&line->tiles[i], plain_block);
        } else {
            clone(&line->tiles[i], plain);
        }
    }
    int delay = (rand()%100 * speed)/100;
    Mob* canon_ = clone_mob(canon, where_can, HEIGTH+pos,speed,(LENGTH*32)/32);
    Mob* ball = clone_mob(canon_ball,-64,HEIGTH+pos,speed,(LENGTH*32)/32);
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

        case BUSHES:
        generator_bushes(game);
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


    }

}

///////////////////////////////////////////
//générateurs avancés
///////////////////////////////////////////


void generator_bushes(Game *game){
    //les buissons
    int proba_change = (rand()%4)*5;
    if(*((game->proba)+BUSHES) <= proba_change){
        *((game->proba)+FOREST) += (2*(*((game->proba)+BUSHES)))/3 ;
        *((game->proba)+RIVER) += (*((game->proba)+BUSHES))/3 ;
        *((game->proba)+ENTRANCE)+=1;
        *((game->proba)+BUSHES) = 0 ;
        *((game->proba)+BUSHES) += check_percent(game, FOREST, ENTRANCE);

    } else {
        *((game->proba)+FOREST) += 2*proba_change/3 ;
        *((game->proba)+BUSHES) -= proba_change ;
        *((game->proba)+RIVER) += proba_change/3 ;
        *((game->proba)+ENTRANCE)+=1;
        *((game->proba)+BUSHES) += check_percent(game, FOREST, ENTRANCE);
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
        game->mobs = list_append(game->mobs, clone_mob(bush, (rand()%LENGTH)*32, HEIGTH,1,256));
    }
}

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
            game->mobs = list_append(game->mobs, clone_mob(fire_clock, pos, HEIGTH,(rand()%3)+1,16));
            break;
        case 1:
            pos=rand()%(LENGTH-3);
            game->mobs = list_append(game->mobs, clone_mob(fire_clock, pos, HEIGTH,(rand()%3)+1,32));
            break;
        case 2:
            pos=rand()%(LENGTH-2);
            game->mobs = list_append(game->mobs, clone_mob(fire_counter, pos, HEIGTH,(rand()%3)+1,16));
            break;
        case 3:
            pos=rand()%(LENGTH-3);
            game->mobs = list_append(game->mobs, clone_mob(fire_counter, pos, HEIGTH,(rand()%3)+1,32));
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
        clone(&line->tiles[i], cave);
    }
    add_buffer_line(game->carte);
    for(int i = 0; i < 2; i++){
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], planks);
        }
        int nb_Block = rand()%3;
        for(int i = 0; i<nb_Block; i++){
            clone(&line->tiles[rand()%LENGTH], brokenplanks);
        }
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    int nb_Block = rand()%3;
    for(int i = 0; i<nb_Block; i++){
        clone(&line->tiles[rand()%LENGTH], brokenplanks);
    }
    add_buffer_line(game->carte);
    line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    for(int i = 0;i<3;i++){
        add_buffer_line(game->carte);
        line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], stalagmite);
        }
        clone(&line->tiles[LENGTH/2], cave);
        if(!LENGTH%2){
            clone(&line->tiles[LENGTH/2+1], cave);
        }
    }
    proba_change_transition_cave(game);



}


void proba_change_cave(Game* game){
    int proba_change = (rand()%4)*5 + 10;
    if(*((game->proba)+CAVE) <= proba_change){
        *((game->proba)+CULTIST) += (2*(*((game->proba)+FOREST)))/3+1 ;
        *((game->proba)+MAGE) += (*((game->proba)+FOREST))/3 ;
        *((game->proba)+CAVE) = 0 ;
    } else {
        *((game->proba)+CULTIST) += 2*proba_change/3+1 ;
        *((game->proba)+CAVE) -= proba_change ;
        *((game->proba)+MAGE) += proba_change/3 ;
    }
    *((game->proba)+CAVE) += check_percent(game, CAVE, MAGE);
}


void generator_cave(Game *game){
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    int nb_wall = rand()%8+2;
    for(int i = 0; i<nb_wall; i++){
        clone(&line->tiles[rand()%LENGTH], stalagmite);
    }
    proba_change_cave(game);
}

void proba_change_cultist(Game* game){
    int proba_change = (rand()%4)*5+15;
    if(*((game->proba)+CULTIST) <= proba_change){
        *((game->proba)+CAVE) += (3*(*((game->proba)+FIRE))/4) ;
        *((game->proba)+CULTIST) = 0 ;
        *((game->proba)+MAGE) += ((*((game->proba)+FIRE))/4)+1;
    } else {
        *((game->proba)+CAVE) += (3*proba_change/4) ;
        *((game->proba)+CULTIST) = 0 ;
        *((game->proba)+MAGE) += (proba_change/4)+1;
    }
    *((game->proba)+CULTIST) += check_percent(game, CAVE, MAGE);
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
        nb_lines=6;
        }
    else {
        nb_lines=5;
    }
    int pos_cult=rand()%(LENGTH-lat_size);
    int nb_cultists=rand()%2+1;
    for (int i=0;i<nb_lines;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int j = 0; j<LENGTH; j++){
            if (i==0 || i==nb_lines-1 || ((j>=pos_cult && j<=(pos_cult+lat_size-2))&&(i==1||i==(nb_lines-2)||j==(pos_cult+lat_size-2)||j==pos_cult))) {
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
            game->mobs = list_append(game->mobs, clone_mob(cult_clock1, pos_cult+2, HEIGTH+nb_lines-2,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter1, pos_cult+1, HEIGTH+nb_lines-2,speed,16));
        }
        break;
        case 2:
                if (sens==0){
            game->mobs = list_append(game->mobs, clone_mob(cult_clock2, pos_cult+3, HEIGTH+nb_lines-2,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter2, pos_cult+1, HEIGTH+nb_lines-2,speed,16));
        }
        break;
        case 1:
        if (sens==0){
            game->mobs = list_append(game->mobs, clone_mob(cult_clock3, pos_cult+2, HEIGTH+nb_lines-2,speed,16));
        }
        else {
            game->mobs = list_append(game->mobs, clone_mob(cult_counter3, pos_cult+1, HEIGTH+nb_lines-2,speed,16));
        }
        break;
    }
    if (nb_cultists==2) {
        Mob *Double;
        switch (type) {
            case 0:
                if (sens==0){
                    Double=clone_mob(cult_clock1, pos_cult+1, HEIGTH+1,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter1, pos_cult+2, HEIGTH+1,speed,16);
                }
                break;
            case 2:
                if (sens==0){
                    Double=clone_mob(cult_clock2, pos_cult+1, HEIGTH+1,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter2, pos_cult+3, HEIGTH+1,speed,16);
                }
                break;
            case 1:
                if (sens==0){
                    Double=clone_mob(cult_clock3, pos_cult+1, HEIGTH+1,speed,16);
                }
                else {
                    Double=clone_mob(cult_counter3, pos_cult+2, HEIGTH+1,speed,16);
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
        *((game->proba)+CULTIST) += (2*(*((game->proba)+MAGE)))/2 ;
        *((game->proba)+CAVE) += (*((game->proba)+MAGE))/2+1 ;
        *((game->proba)+MAGE) = 0 ;
    } else {
        *((game->proba)+CULTIST) += 2*proba_change/2 ;
        *((game->proba)+MAGE) -= proba_change ;
        *((game->proba)+CAVE) += proba_change/2+1 ;
    }
    *((game->proba)+MAGE) += check_percent(game, CAVE, MAGE);
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
    int level =rand()%2;
    if (level==0){
        int speed=rand()%3+1;
        game->mobs = list_append(game->mobs, clone_mob(mage, pos_x, HEIGTH,speed,0));
        game->projectile = list_append(game->projectile, clone_mob(curse_arrow, pos_x, HEIGTH,speed,0));
    }
    else{
        int speed=(rand()%2+1)*2;
        game->mobs = list_append(game->mobs, clone_mob(mage, pos_x, HEIGTH,speed,0));
        game->projectile = list_append(game->projectile, clone_mob(curse_arrow, pos_x, HEIGTH,speed,0));
        if (speed==2) {game->projectile = list_append(game->projectile, clone_mob(curse_arrow, pos_x, HEIGTH,speed,100));}
        else {game->projectile = list_append(game->projectile, clone_mob(curse_arrow, pos_x, HEIGTH,speed,200));}
    }
    proba_change_mage(game);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void initialise_map(Game* game){
    for(int i=0;i<HEIGTH-4;i++){
        update_game(game);
        move_up(game);
        switch_game(game);
        update_game(game);
        game->score--;
    }
}

Game *initialise_game(int character_id){
    // explicite
    srand( time( NULL ) );
    Game* game = malloc(sizeof(Game));
    game->carte = create_empty_map();
    game->joueur = initialise_player(character_id); //0 for now
    game->score = 1;
    game->nb_exp = 0;
    game->exp = NULL;
    game->mobs = NULL;
    game->projectile = NULL;
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
    }else if(a_mob_collide(game->projectile, game->joueur)){
        return true;
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
    //printf("%d\n",game->score);
    move_all_mob(game->mobs,game->joueur);
    move_all_mob(game->projectile,game->joueur);
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
    if(game->score%(game->until_next) == 0){
        game->score++;
        game->biome++;
        switch(game->biome){
            case CROSSY_ROAD:
            proba_change_game(game,FOREST);
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

            default:
            //game->biome = CROSSY_ROAD;
            //game->until_next = 20;
            break;
        }
    }
    if(a_mob_collide(game->exp, game->joueur)){
        game->exp = destroy_liste_mob_collide(game->exp, game->joueur);
        game->nb_exp++;
    }
    if(game->carte->head_buffer==game->carte->head_map){
        generator_obstacle(game);
        //print_percent(game);
    }
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
