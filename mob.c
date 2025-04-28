#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mob.h"

///List_Map///

Liste_Mob* list_create(Mob* mob){
    // créer une liste contenant un mob
    Liste_Mob* list = malloc(sizeof(Liste_Mob));
    list->mob = mob;
    list->next = NULL;
    return list;
}

Liste_Mob *list_append(Liste_Mob* list, Mob* mob){
    // !Toujours mettre des clones en entrer bande de malandrin (Yovhann)
    // ajoute une nouvelle liste mob en fin de liste
    if(list==NULL){
        return list_create(mob);
    } else {
        list->next = list_append(list->next, mob);
        return list;
    }
}

void print_rest_liste(Liste_Mob *list){
    // sert a afficher la liste des mobs (utile pour le débug)
    if(list==NULL){
        printf(" ]\n");
    }else{
        printf(", %d", list->mob->_y);
        print_rest_liste(list->next);
    }
}

void print_liste(Liste_Mob *list){
    // sert a afficher la liste des mobs (utile pour le débug)
    if(list==NULL){
        printf("[]\n");
    }else{
        printf("[ %d", list->mob->id_mob);
        print_rest_liste(list->next);
    }
}

void move_all_mob(Liste_Mob* list, Player* player){
    // bouge tous les mobs selon leur path
    if(list!=NULL){
        (list->mob->path)(list->mob,player);
        move_all_mob(list->next, player);
    }
}

void switch_mob(Liste_Mob* list){
    // fait descendre les mobs quand la map avance
    if(list!=NULL){
        list->mob->_y -= 32;
        list->mob->memory__y -= 32;
        switch_mob(list->next);
    }
}

bool a_death_mob_collide(Liste_Mob* list,Player* player){
    // test la collision avec le joueur pour tous les monstres
    if(list==NULL){
        return false;
    } else {
        return ((death_mob_collide(list->mob, player))||(a_death_mob_collide(list->next, player)));
    }
}

bool a_mob_collide(Liste_Mob* list,Player* player){
    if(list==NULL){
        return false;
    } else {
        return ((mob_collide(list->mob, player))||(a_mob_collide(list->next, player)));
    }
}

//////////////////////////////////////////////////////



bool mob_in_map(Mob *mob){
    if(((mob->_x)/32>LENGTH)||((mob->_y)/32>HEIGTH)){ //Dépassement supérieur
        return false;
    } else if(((mob->_x/32 + mob->length - 1)<0)||((mob->_y/32 + mob->heigth - 1)<0)) {
        return false;
    } else { 
        return true;
    }
}

Mob* clone_mob(Mob mob, int x_ini, int y_ini, int speed, int max_action_delay){
    // creer un mob selon le modèle rentré
    Mob *new_mob = malloc(sizeof(mob));
    new_mob->heigth = mob.heigth;
    new_mob->length = mob.length;
    new_mob->is_death = mob.is_death;
    new_mob->move_x = mob.move_x;
    new_mob->_x = x_ini*32;
    new_mob->_y = y_ini*32;
    new_mob->speed = speed;
    new_mob->delay = speed;
    new_mob->id_mob = mob.id_mob;
    new_mob->offset_x = mob.offset_x;
    new_mob->offset_y = mob.offset_y;
    new_mob->path = mob.path;
    new_mob->max_action_delay=max_action_delay;
    new_mob->action_delay=max_action_delay;
    new_mob->action_value=mob.action_value;
    new_mob->memory__x=x_ini*32;;
    new_mob->memory__y=y_ini*32;
    return new_mob;
}

bool death_mob_collide(Mob* mob, Player* player){
    if(!mob->is_death){
        return false;
    }
    if(((mob->_x - 16)<=player->_x_dest)&&(player->_x_dest <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y - 16)<=player->_y_dest)&&(player->_y_dest <= mob->_y + 16 + (mob->heigth - 1))*32) {
         return true;
        }
    }
    return false;
}

bool mob_collide(Mob* mob, Player* player){
    if(((mob->_x-16)<=player->_x_dest)&&(player->_x_dest <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y -16 )<=player->_y_dest)&&(player->_y_dest <= mob->_y + 16 + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}

///Création de mobs et pathfinding

void path_right(Mob *car, Player*p){
    if(!car->delay){
        if(car->_x/32 >= LENGTH){
            car->_x = -car->length*32;
        } else {
            car->_x += 3; 
        }
        car->action_value = 1-car->action_value;
        if(car->action_value&&(car->id_mob!=PLANK_RIGHT)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_right = {1,2,true,0,0,0,0,0,CAR_RIGHT,0,0,path_right,0,0,0,0,0}; //Note : x et y ini ne veulent rien dire ici
const Mob plank_right = {1,1,false,3,0,0,0,0,PLANK_RIGHT,0,0,path_right,0,0,0,0,0}; 
const Mob bus_right = {1,4,true,0,0,0,0,0,BUS_RIGHT,0,0,path_right,0,0,0,0,0};

void path_left(Mob *car, Player*p){
    if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->_x = LENGTH*32;
        } else {
            car->_x -=3; //À remplacer par move_mob (?)
        }
        car->action_value = 1-car->action_value;
        if(car->action_value&&(car->id_mob!=PLANK_LEFT)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_left = {1,2,true,0,0,0,0,0,CAR_LEFT,0,1,path_left,0,0,0,0,0};
const Mob plank_left = {1,1,false,-3,0,0,0,0,PLANK_LEFT,0,1,path_left,0,0,0,0,0};
const Mob bus_left = {1,4,true,0,0,0,0,0,BUS_LEFT,0,1,path_left,0,0,0,0,0};

void canon_idle(Mob* mob, Player *p){
    switch (mob->action_value) {
        case 0:
            if(mob->delay < 32){
                mob->action_value = 1;
                mob->offset_x ++;
                mob->offset_x %= 3;
            } 
            mob->delay--;
            break;

        case 1:
            if(mob->delay==0){
                mob->action_value = 2;
                mob->offset_x ++;
                mob->offset_x %= 3;
            } else {
                mob->delay--;
            }
            break;

        case 2:
            if(mob->action_delay==0){
                mob->action_delay = mob->max_action_delay;
                mob->delay = mob->speed;
                mob->action_value = 0;
                mob->offset_x ++;
                mob->offset_x %= 3;
            } else {
                mob->action_delay--;
            }
            break;

        } 
}

const Mob canon_right = {1,1,false,0,0,0,0,0,CANON_RIGHT,0,0,canon_idle,0,0,0,0,0};
const Mob canon_left = {1,1,false,0,0,0,0,0,CANON_LEFT,0,1,canon_idle,0,0,0,0,0};

void canon_right_fast(Mob* mob, Player *p){
    switch (mob->action_value) {
        case 0:
            if(mob->delay==0){
                mob->action_value = 1;
                mob->_x = 32;
            } else {
                mob->delay--;
            }
            break;

        case 1:
            if(mob->action_delay==0){
                mob->action_delay = mob->max_action_delay;
                mob->delay = mob->speed;
                mob->action_value = 0;
            } else {
                mob->action_delay--;
                mob->_x += 32;
            }
            break;

        } 
}

void canon_left_fast(Mob* mob, Player *p){
    switch (mob->action_value) {
        case 0:
            if(mob->delay==0){
                mob->action_value = 1;
                mob->_x = LENGTH*32-64;
            } else {
                mob->delay--;
            }
            break;

        case 1:
            if(mob->action_delay==0){
                mob->action_delay = mob->max_action_delay;
                mob->delay = mob->speed;
                mob->action_value = 0;
            } else {
                mob->action_delay--;
                mob->_x -= 32;
            }
            break;

        } 
}

const Mob canon_ball_right = {1,1,true,0,0,0,0,0,CANON_BALL_RIGHT,0,0,canon_right_fast,0,0,0,0,0};
const Mob canon_ball_left = {1,1,true,0,0,0,0,0,CANON_BALL_LEFT,0,1,canon_left_fast,0,0,0,0,0};


void destroy_mobs(Liste_Mob *liste){
    free(liste->mob);
    free(liste);
}

void destroy_liste_mob(Liste_Mob *liste){
    Liste_Mob *liste_mob = liste;
    while(liste_mob!=NULL){
        Liste_Mob *mem = liste_mob->next;
        destroy_mobs(liste_mob);
        liste_mob = mem;
    }
}

Liste_Mob* destroy_liste_mob_from(int y, Liste_Mob *list){
    Liste_Mob *liste_mob = list;
    Liste_Mob *prev_not_free = NULL;
    while(liste_mob!=NULL){
        Liste_Mob *mem = liste_mob->next;
        if(liste_mob->mob->memory__y < y*32){
            destroy_mobs(liste_mob);
        }else{
            if(prev_not_free==NULL){
                 list = liste_mob;
            } else {
                prev_not_free->next = liste_mob;
            }
            prev_not_free = liste_mob;
        }
        liste_mob = mem;
    }
    if(prev_not_free!=NULL){
        prev_not_free->next=NULL;
    }
    return list;
}

//////////////////////////////////////////////////////////////////////
//Mobs avancés
//////////////////////////////////////////////////////////////////////

void clockwise_cycle(Mob *fire,Player* Joueur){
    //fait des tours dans le sens horaire d'une zone de 2*2 cases
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x+=2;
            break;
        case 1:
            fire->_y-=2;
            break;
        case 2:
            fire->_x-=2;
            break;
        case 3:
            fire->_y+=2;
            break;
        default:
            break;
        }
        fire->action_delay-=1;
        if (fire->action_delay==0) {
            if (fire->action_value==3) {
                fire->action_value=0;
            }
            else {
                fire->action_value+=1;
            }
            fire->action_delay=fire->max_action_delay;
        }
        fire->delay = fire->speed;
        fire->offset_x++;
        fire->offset_x %= 4;
    } else {
        fire->delay--;
    }
}

const Mob fire_clock = {1,1,true,0,0,0,0,0,FIRECLOCK,0,0,clockwise_cycle,0,16,0,0,0};

void counterclock_cycle(Mob *fire,Player* Joueur){
    //fait des tours dans le sens antihoraire d'une zone de 2*2 cases
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x-=2;
            break;
        case 1:
            fire->_y-=2;
            break;
        case 2:
            fire->_x+=2;
            break;
        case 3:
            fire->_y+=2;
            break;
        default:
            break;
        }
        fire->action_delay-=1;
        if (fire->action_delay==0) {
            if (fire->action_value==3) {
                fire->action_value=0;
            }
            else {
                fire->action_value+=1;
            }
            fire->action_delay=fire->max_action_delay;
        }
        fire->delay = fire->speed;
        fire->offset_x++;
        fire->offset_x %= 4;
    } else {
        fire->delay--;
    }
}

const Mob fire_counter = {1,1,true,0,0,0,0,0,FIRECOUNTER,0,0,counterclock_cycle,0,16,0,0,0};

// void clockwise_cycle_3(Mob *fire,Player* Joueur){
//     //fait des tours dans le sens horaire d'une zone de 3*3 cases
//     if(!fire->delay){
//         switch (fire->action_value)
//         {
//         case 0:
//             fire->_x+=2;
//             break;
//         case 1:
//             fire->_y-=2;
//             break;
//         case 2:
//             fire->_x-=2;
//             break;
//         case 3:
//             fire->_y+=2;
//             break;
//         default:
//             break;
//         }
//         fire->action_delay-=1;
//         if (fire->action_delay==0) {
//             if (fire->action_value==3) {
//                 fire->action_value=0;
//             }
//             else {
//                 fire->action_value+=1;
//             }
//             fire->action_delay=fire->max_action_delay;
//         }
//         fire->delay = fire->speed;
//     } else {
//         fire->delay--;
//     }
// }

// const Mob fire_3_clock = {1,1,true,0,0,0,0,0,FIRE2CLOCK,0,0,clockwise_cycle_3,0,32,0,0,0};

// void counterclock_cycle_3(Mob *fire,Player* Joueur){
//     //fait des tours dans le sens antihoraire d'une zone de 3*3 cases
//     if(!fire->delay){
//         switch (fire->action_value)
//         {
//         case 0:
//             fire->_x-=2;
//             break;
//         case 1:
//             fire->_y-=2;
//             break;
//         case 2:
//             fire->_x+=2;
//             break;
//         case 3:
//             fire->_y+=2;
//             break;
//         default:
//             break;
//         }
//         fire->action_delay-=1;
//         if (fire->action_delay==0) {
//             if (fire->action_value==3) {
//                 fire->action_value=0;
//             }
//             else {
//                 fire->action_value+=1;
//             }
//             fire->action_delay=32;
//         }
//         fire->delay = fire->speed;
//     } else {
//         fire->delay--;
//     }
// }

// const Mob fire_3_counter = {1,1,true,0,0,0,0,0,FIRE2CLOCK,counterclock_cycle_3,0,32,0,0};

void ambush(Mob *bush,Player* player){
    //embuscade
    bool has_move = false;
        if(!bush->delay){
            if (player->_x-bush->_x<=32 && player->_x-bush->_x>=0 && (player->_y-bush->_y<=8 && bush->_y-player->_y<=8)){
                bush->_x+=4;
                has_move = true;
            }
            if (bush->_x-player->_x<=32 && bush->_x-player->_x>=0 &&(player->_y-bush->_y<=8 && bush->_y-player->_y<=8)){
                bush->_x-=4;
                has_move = true;
            }
            if (player->_y-bush->_y<=32 && player->_y-bush->_y>=0 && (player->_x-bush->_x<=8 && bush->_x-player->_x<=8)){
                bush->_y+=4;
                has_move = true;
            }
            if (bush->_y-player->_y<=32 && bush->_y-player->_y>=0 && (player->_x-bush->_x<=8 && bush->_x-player->_x<=8)){
                bush->_y-=4;
                has_move = true;
            }
        bush->delay = bush->speed;
        if(has_move){
            bush->offset_x++;
            bush->offset_x %=2;
        }

    } else {
        bush->delay--;
    }
    if(!bush->action_delay){
            switch(bush->action_value){
                case 0:
                bush->action_delay=2;
                bush->offset_y++;
                bush->offset_y %= 2;
                bush->action_value = 1;
                break;

                case 1:
                bush->action_delay=bush->max_action_delay;
                bush->offset_y++;
                bush->offset_y %= 2;
                bush->action_value = 0;
                break;

            }
        } else {
            bush->action_delay--;
        }
}

const Mob bush = {1,1,true,0,0,0,0,0,BUSH,0,0,ambush,0,256,0,0,0};

void patrol_clock4x2(Mob *fire,Player* Joueur){
    //fait des tours dans le sens horaire d'une zone de 4 cases de long et 2 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 2:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 1;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_clock5x2(Mob *fire,Player* Joueur){
    //fait des tours dans le sens horaire d'une zone de 5 cases de long et 2 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 1:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 2:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 3:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 1;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_clock4x3(Mob *fire,Player* Joueur){
    //fait des tours dans le sens horaire d'une zone de 4 cases de long et 3 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 1;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter4x2(Mob *fire,Player* Joueur){
    //fait des tours dans le sens antihoraire d'une zone de 4 cases de long et 2 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 2:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 0;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter5x2(Mob *fire,Player* Joueur){
    //fait des tours dans le sens antihoraire d'une zone de 5 cases de long et 2 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 1:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 2:
                    fire->action_delay=fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 3:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 0;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter4x3(Mob *fire,Player* Joueur){
    //fait des tours dans le sens antihoraire d'une zone de 4 cases de long et 3 cases de haut
    if(!fire->delay){
        switch (fire->action_value)
        {
        case 0:
            fire->_x-=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 1:
            fire->_y-=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 2:
            fire->_x+=2;
            if(fire->_x%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        case 3:
            fire->_y+=2;
            if(fire->_y%8){
                fire->offset_x++;
                fire->offset_x %=4;
            }
            break;
        default:
            break;
        }
        fire->action_delay--;
        if (fire->action_delay==0) {
            switch (fire->action_value){
                case 0:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 0;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}
//mobs allant dans le sens horaire
const Mob cult_clock1 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock4x2,0,16,0,0,0};
const Mob cult_clock2 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock5x2,0,16,0,0,0};
const Mob cult_clock3 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock4x3,0,16,0,0};
//mobs allant dans le sens antihoraire
const Mob cult_counter1 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter4x2,0,16,0,0,0};
const Mob cult_counter2 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter5x2,0,16,0,0,0};
const Mob cult_counter3 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter4x3,0,16,0,0,0};

void still(Mob *mage,Player* player){
        if(!mage->action_delay){
            mage->action_delay=(mage->action_value)*(mage->speed)*20;
            mage->offset_x++;
            mage->offset_x %=4;
        }
        else {
            mage->action_delay--;
        }
        mage->delay = mage->speed;
}

void follow(Mob *projectile,Player* player){
    if(!projectile->delay){
        if (mob_in_map(projectile)){
            if (player->_x>projectile->_x){
                projectile->_x+=2;
            }
            if (player->_x<projectile->_x){
                projectile->_x-=2;
            }
            if (player->_y>projectile->_y){
                projectile->_y+=2;
            }
            if (player->_y<projectile->_y){
                projectile->_y-=2;
            }
        }
    if(!projectile->action_delay){
        if (mob_in_map(projectile)){
            projectile->_x=99999999;
            projectile->action_delay=(projectile->speed)*4*(projectile->action_value);
        } else {
            projectile->_x=projectile->memory__x;
            projectile->_y=projectile->memory__y;
            projectile->action_delay=(projectile->speed)*16*(projectile->action_value); 
        }
    }
    else {
        projectile->action_delay--;
    }
    projectile->delay = projectile->speed;
    projectile->offset_x++;
    projectile->offset_x %= 4;
    } else {
        projectile->delay--;
    }
}


const Mob mage={1,1,true,0,0,0,0,0,HIGHCULTIST,0,0,still,10,0,0,0,0};
const Mob curse_arrow={1,1,true,0,0,0,0,0,CURSE,0,0,follow,10,0,0,0};
//const Mob curse_arrow_bis_1={1,1,true,0,0,0,0,0,CURSE,0,0,follow,10,100,0,0,0};
//const Mob curse_arrow_bis_2={1,1,true,0,0,0,0,0,CURSE,0,0,follow,10,200,0,0,0};

