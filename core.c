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
    *(proba+FOREST) = 0;
    *(proba+ROAD) = 0;
    *(proba+RIVER) = 0;
    *(proba+BUSHES) = 0;
    *(proba+ENTRANCE) = 100;
    *(proba+FIRE) = 0;
    *(proba+CAVE) = 0;
    *(proba+CULTIST) = 0;
    *(proba+MAGE) = 0;
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
        generator_Cultist(game);
        break;
                
        case MAGE:
        generator_Cultist(game);
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

///////////////////////////////////////////
//générateurs avancés
///////////////////////////////////////////


void generator_bushes(Game *game){
    //les buissons
    int proba_change = (rand()%4)*5;
    if(*((game->proba)+BUSHES) <= proba_change){
        // *((game->proba)+FOREST) += (2*(*((game->proba)+BUSHES)))/3 ;
        // *((game->proba)+RIVER) += (*((game->proba)+BUSHES))/3+1 ;
        // *((game->proba)+BUSHES) = 0 ;
    } else {
        // *((game->proba)+FOREST) += 2*proba_change/3 ;
        // *((game->proba)+BUSHES) -= proba_change ;
        // *((game->proba)+RIVER) += proba_change/3+1 ;
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

void generator_entrance(Game *game){
    //les buissons
    *((game->proba)+FIRE) += (*((game->proba)+ENTRANCE));
    *((game->proba)+ENTRANCE) = 0 ;
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==LENGTH/2 || i==((LENGTH/2)+1)) {
            clone(&line->tiles[i],transition);
        }
        else {
            clone(&line->tiles[i],muret);
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
}

void generator_fire(Game *game){
    //feu
    int proba_change = (rand()%4)*2;
    if(*((game->proba)+FIRE) <= proba_change){
        *((game->proba)+CAVE) += (*((game->proba)+FIRE)) ;
        *((game->proba)+FIRE) = 0 ;
    } else {
        *((game->proba)+CAVE) += proba_change ;
        *((game->proba)+FIRE) -= proba_change ;
    }
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], planks);
    }
    int nb_Block = rand()%3;
    for(int i = 0; i<nb_Block; i++){
        clone(&line->tiles[rand()%LENGTH], brokenplanks);
    }
    int nb_flames = rand()%3;
    for(int i = 0; i<nb_flames; i++){
        int pattern=rand()%4;
        int pos;
        switch (pattern){
        case 0:
            pos=rand()%(LENGTH-2);
            game->mobs = list_append(game->mobs, clone_mob(fire_2_clock, pos*32, HEIGHT,(rand()%3)+1));
            break;
        case 1:
            pos=rand()%(LENGTH-3);
            game->mobs = list_append(game->mobs, clone_mob(fire_3_clock, pos*32, HEIGHT,(rand()%3)+1));
            break;
        case 2:
            pos=rand()%(LENGTH-2);
            game->mobs = list_append(game->mobs, clone_mob(fire_2_counter, pos*32, HEIGHT,(rand()%3)+1));
            break;
        case 3:
            pos=rand()%(LENGTH-3);
            game->mobs = list_append(game->mobs, clone_mob(fire_3_counter, pos*32, HEIGHT,(rand()%3)+1));
            break;
        default:
            break;
        }
        
    }
}

void generator_cave(Game *game){
    // explicite
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
    add_buffer_line(game->carte);
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        clone(&line->tiles[i], cave);
    }
    int nb_wall = rand()%8+2;
    for(int i = 0; i<nb_wall; i++){
        clone(&line->tiles[rand()%LENGTH], stalagmite);
    }

}

void generator_Cultist(Game *game){
    //les cultistes avec des dagues
    int proba_change = (rand()%4)*5+5;
    if(*((game->proba)+CULTIST) <= proba_change){
        *((game->proba)+CAVE) += (3*(*((game->proba)+FIRE))/4) ;
        *((game->proba)+CULTIST) = 0 ;
        *((game->proba)+MAGE) += ((*((game->proba)+FIRE))/4)+1;
    } else {
        *((game->proba)+CAVE) += (3*proba_change/4) ;
        *((game->proba)+CULTIST) = 0 ;
        *((game->proba)+MAGE) += (proba_change/4)+1;
    }
    int nb_lines=rand()%3+2;
    int nb_cultists=rand()%3+2+(nb_lines/2);
    int nb_used=0;
    for (int i=0;i<nb_lines;i++) {
        add_buffer_line(game->carte);
        Line* line = game->carte->head_buffer;
        for(int i = 0; i<LENGTH; i++){
            clone(&line->tiles[i], cave);
        }
        if (i==0) { //on ne fait rien
        }else if (i==1){
            int nb_here = rand()%(nb_cultists-nb_used+1);
            nb_used+=nb_here;
            for(int i = 0; i<nb_here; i++){
                int pattern=rand()%4;
                int pos;
                switch (pattern)
                {
                case 0:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 1:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 2:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 3:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                default:
                    break;
                }
            }
        } else if (i<nb_lines-1){
            int nb_here = rand()%(nb_cultists-nb_used+1);
            nb_used+=nb_here;
            for(int i = 0; i<nb_here; i++){
                int pattern=rand()%6;
                int pos;
                switch (pattern)
                {
                case 0:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 1:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 2:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 3:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 4:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock3, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 5:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter3, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                default:
                    break;
                }
            }
        }else if (i<nb_lines-1){
            int nb_here = nb_cultists-nb_used;
            for(int i = 0; i<nb_here; i++){
                int pattern=rand()%6;
                int pos;
                switch (pattern)
                {
                case 0:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 1:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 2:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter1, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 3:
                    pos=rand()%(LENGTH-5);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter2, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 4:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_clock3, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                case 5:
                    pos=rand()%(LENGTH-4);
                    game->mobs = list_append(game->mobs, clone_mob(cult_counter3, pos*32, HEIGHT,(rand()%3)+1));
                    break;
                default:
                    break;
                }
            }
        }
        
    }
}

void generator_mage(Game *game){
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
    add_buffer_line(game->carte);
    int pos_x=rand()%LENGTH;
    Line* line = game->carte->head_buffer;
    for(int i = 0; i<LENGTH; i++){
        if (i==pos_x){
            clone(&line->tiles[i], socle);
        }else {
            clone(&line->tiles[i], canal);
        }
    }
    int level =rand()%2;
    if (level==0){
        int speed=rand()%3+1;
        game->mobs = list_append(game->mobs, clone_mob(mage, pos_x*32, HEIGHT,speed));
        game->mobs = list_append(game->mobs, clone_mob(curse_arrow, pos_x*32, HEIGHT,speed));
    }
    else{
        int speed=(rand()%2+1)*2;
        game->mobs = list_append(game->mobs, clone_mob(mage, pos_x*32, HEIGHT,speed));
        game->mobs = list_append(game->mobs, clone_mob(curse_arrow, pos_x*32, HEIGHT,speed));
        if (speed==2) {game->mobs = list_append(game->mobs, clone_mob(curse_arrow_bis_1, pos_x*32, HEIGHT,speed));}
        else {game->mobs = list_append(game->mobs, clone_mob(curse_arrow_bis_2, pos_x*32, HEIGHT,speed));}
    }
}
