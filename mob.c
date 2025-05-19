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

Liste_Mob *list_append_list(Liste_Mob* init, Liste_Mob* to_app){
    // !Toujours mettre des clones en entrer bande de malandrin (Yovhann)
    // ajoute une nouvelle liste mob en fin de liste
    if(init==NULL){
        return to_app;
    } else {
        init->next = list_append_list(init->next, to_app);
        return init;
    }
}

void print_rest_liste(Liste_Mob *list){
    // sert a afficher la liste des mobs (utile pour le débug)
    if(list==NULL){
        printf(" ]\n");
    }else{
        printf(", (%d,%d)", list->mob->_x, list->mob->_y);
        print_rest_liste(list->next);
    }
}

void print_liste(Liste_Mob *list){
    // sert a afficher la liste des mobs (utile pour le débug)
    if(list==NULL){
        printf("[]\n");
    }else{
        printf("[ (%d,%d)", list->mob->_x, list->mob->_y);
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

bool a_not_death_mob_collide(Liste_Mob* list,Player* player){
    // test la collision avec le joueur pour tous les monstres
    if(list==NULL){
        return false;
    } else {
        return ((not_death_mob_collide(list->mob, player))||(a_not_death_mob_collide(list->next, player)));
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
    new_mob->was_smoked = 0;
    return new_mob;
}

bool death_mob_collide(Mob* mob, Player* player){
    if(!mob->is_death){
        return false;
    }
    if(((mob->_x -16)<=player->_x_dest)&&(player->_x_dest <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y - 16) <=player->_y_dest)&&(player->_y_dest <= (mob->_y + 16) + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}

bool not_death_mob_collide(Mob* mob, Player* player){
    if(mob->is_death){
        return false;
    }
    if(((mob->_x -16)<=player->_x_dest)&&(player->_x_dest <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y - 16) <=player->_y_dest)&&(player->_y_dest <= (mob->_y + 16) + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}


bool mob_collide(Mob* mob, Player* player){
    if(((mob->_x)-16<=player->_x_dest)&&(player->_x_dest <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y -16 )<=player->_y_dest)&&(player->_y_dest <= (mob->_y + 16) + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}

bool mob_collide_mob(Mob* mob, Mob* mob2){
    if(((mob->_x-16)<=mob2->_x)&&(mob2->_x <= mob->_x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y -16 )<=mob2->_y)&&(mob2->_y <= mob->_y + 16 + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}

bool mob_collide_mob_on_memory(Mob* mob, Mob* mob2){
    if(((mob->memory__x-16)<=mob2->_x)&&(mob2->_x <= mob->memory__x + 16 + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->memory__y -16 )<=mob2->_y)&&(mob2->_y <= mob->memory__y + 16 + (mob->heigth - 1)*32)) {
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
        if(car->action_value&&(car->id_mob!=PLANK)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_right = {2,1,true,0,0,0,0,0,CAR,0,0,path_right,0,0,0,0,0,0}; //Note : x et y ini ne veulent rien dire ici
const Mob plank_right = {1,1,false,3,0,0,0,0,PLANK,0,0,path_right,0,0,0,0,0,0}; 
const Mob bus_right = {4,1,true,0,0,0,0,0,BUS,0,0,path_right,0,0,0,0,0,0};

void path_left(Mob *car, Player*p){
    if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->_x = LENGTH*32;
        } else {
            car->_x -=3; //À remplacer par move_mob (?)
        }
        car->action_value = 1-car->action_value;
        if(car->action_value&&(car->id_mob!=PLANK)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_left = {2,1,true,0,0,0,0,0,CAR,0,1,path_left,0,0,0,0,0,0};
const Mob plank_left = {1,1,false,-3,0,0,0,0,PLANK,0,1,path_left,0,0,0,0,0,0};
const Mob bus_left = {4,1,true,0,0,0,0,0,BUS,0,1,path_left,0,0,0,0,0,0};

void canon_idle(Mob* mob, Player *p){
    switch (mob->action_value) {
        case 0:
            if(mob->delay < 64){
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

const Mob canon_right = {1,1,true,0,0,0,0,0,CANON_MOB,0,0,canon_idle,0,0,0,0,0,0};
const Mob canon_left = {1,1,true,0,0,0,0,0,CANON_MOB,0,1,canon_idle,0,0,0,0,0,0};

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

const Mob canon_ball_right = {1,1,true,0,0,0,0,0,CANON_BALL,0,0,canon_right_fast,0,0,0,0,0,0};
const Mob canon_ball_left = {1,1,true,0,0,0,0,0,CANON_BALL,0,1,canon_left_fast,0,0,0,0,0,0};


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
    } else {
        list = NULL;
    }
    return list;
}

Liste_Mob* destroy_liste_mob_collide(Liste_Mob *liste,Player* player){
    if(liste==NULL){
        return NULL;
    } else if((!liste->mob->is_death)&&mob_collide(liste->mob,player)){
        Liste_Mob* ret = liste->next;
        destroy_mobs(liste);
        return destroy_liste_mob_collide(ret, player);
    } else {
        liste->next = destroy_liste_mob_collide(liste->next,player);
        return liste;
    }
}

Liste_Mob* destroy_liste_mob_collide_mob(Liste_Mob *liste,Mob* mob){
    if(liste==NULL){
        return NULL;
    } else if(mob_collide_mob(liste->mob,mob)){
        Liste_Mob* ret = liste->next;
        destroy_mobs(liste);
        return destroy_liste_mob_collide_mob(ret, mob);
    } else {
        liste->next = destroy_liste_mob_collide_mob(liste->next,mob);
        return liste;
    }
}


Liste_Mob* add_smoke_liste_mob_collide(Liste_Mob* proj, Liste_Mob *liste_mob,Mob* mob){ // !! La liste retour est la liste de projectile
    if(liste_mob==NULL){
        return proj;
    } else if((liste_mob->mob->is_death)&&(mob_collide_mob(liste_mob->mob,mob))){
        mob->action_value = 0;
        liste_mob->mob->path = perfectly_still;
        liste_mob->mob->was_smoked = 1;
        Mob* smoky;
        switch(liste_mob->mob->length){
            case 4:
                smoky = clone_mob(smoke4x1, 0, 0, 4, 0);
                break;

            case 2:
                if(liste_mob->mob->heigth==2){
                    smoky = clone_mob(smoke2x2, 0, 0, 4, 0);
                } else {
                     smoky = clone_mob(smoke2x1, 0, 0, 4, 0);
                }
                break;

            case 1:
            if(liste_mob->mob->heigth==2){
                     smoky = clone_mob(smoke1x2, 0, 0, 4, 0);
                } else {
                     smoky = clone_mob(smoke1x1, 0, 0, 4, 0);
                }
                break;

        }
        smoky->_x = liste_mob->mob->_x;
        smoky->_y = liste_mob->mob->_y;
        proj = list_append(proj, smoky);
        return add_smoke_liste_mob_collide(proj,liste_mob->next, mob);
    } else {
        return add_smoke_liste_mob_collide(proj,liste_mob->next, mob);
    }
}

void add_smoke_liste_proj_collide(Liste_Mob *liste_proj,Mob* mob){ // !! La liste retour est la liste de projectile
    if(liste_proj==NULL){
        return;
    } else if(mob_collide_mob_on_memory(liste_proj->mob,mob)){
        liste_proj->mob->was_smoked = 1;
        add_smoke_liste_proj_collide(liste_proj->next,mob);
    } else {
        add_smoke_liste_proj_collide(liste_proj->next,mob);
    }
}

Liste_Mob* destroy_smoke(Liste_Mob* liste){
    if(liste==NULL){
        return NULL;
    } else if((liste->mob->id_mob == SMOKE1x1)||(liste->mob->id_mob == SMOKE2x1)||(liste->mob->id_mob == SMOKE1x2)||(liste->mob->id_mob == SMOKE2x2)||(liste->mob->id_mob == SMOKE4x1)){
        if(liste->mob->action_value >= 2){
            //printf("%d\n",liste->mob->id_mob);
            Liste_Mob* ret = liste->next;
            destroy_mobs(liste);
            return destroy_smoke(ret);
        } else {
            liste->next = destroy_smoke(liste->next);
            return liste;
        }
    } else {
        liste->next = destroy_smoke(liste->next);
        return liste;
    }
}

Liste_Mob* destroy_on_smoke(Liste_Mob* liste){
    if(liste==NULL){
        return NULL;
    } else if(liste->mob->was_smoked){
        if(liste->mob->was_smoked>=9){
            //printf("Smoked : (%d,%d)\n",liste->mob->_x/32,liste->mob->_y/32);
            Liste_Mob* ret = liste->next;
            destroy_mobs(liste);
            return destroy_on_smoke(ret);
        } else {
           liste->mob->was_smoked++;
           liste->next = destroy_on_smoke(liste->next);
           return liste;
        }
    } else {
        liste->next = destroy_on_smoke(liste->next);
        return liste;
    }
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

const Mob fire_clock = {1,1,true,0,0,0,0,0,FIRE_MOB,0,0,clockwise_cycle,0,16,0,0,0,0};

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

const Mob fire_counter = {1,1,true,0,0,0,0,0,FIRE_MOB,0,0,counterclock_cycle,0,16,0,0,0,0};

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

void ambush(Mob *bush,Player* p){
    //embuscade
    bool has_move = false;
        if(!bush->delay){
             if(((bush->_x-16+36)<=p->_x_dest)&&(p->_x_dest <= bush->_x + 16 +36)){ 
                if(((bush->_y -16 )<=p->_y_dest)&&(p->_y_dest <= bush->_y + 16)) {
                    bush->_x+=4;
                    has_move = true;
                }
            }
             if(((bush->_x-16-36)<=p->_x_dest)&&(p->_x_dest <= bush->_x + 16 -36)){ //Dépassement supérieur
                if(((bush->_y -16 )<=p->_y_dest)&&(p->_y_dest <= bush->_y + 16)) {
                    bush->_x-=4;
                    has_move = true;
                }
            }
            if(((bush->_y-16+36)<=p->_y_dest)&&(p->_y_dest <= bush->_y + 16 +36)){ 
                if(((bush->_x -16 )<=p->_x_dest)&&(p->_x_dest <= bush->_x + 16)) {
                    bush->_y+=4;
                    has_move = true;
                }
            }
             if(((bush->_y-16-36)<=p->_y_dest)&&(p->_y_dest <= bush->_y + 16 -36)){ //Dépassement supérieur
                if(((bush->_x -16 )<=p->_x_dest)&&(p->_x_dest <= bush->_x + 16)) {
                    bush->_y-=4;
                    has_move = true;
                }
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
                bush->action_delay=32;
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

const Mob bush = {1,1,true,0,0,0,0,0,BUSH,0,0,ambush,0,256,0,0,0,0};

void patrol_clock4x3(Mob *fire,Player* Joueur){
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
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 2;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_clock5x3(Mob *fire,Player* Joueur){
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
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 2;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_clock4x4(Mob *fire,Player* Joueur){
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
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 3;
                    break;
                case 2:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 2;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter4x3(Mob *fire,Player* Joueur){
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
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 3;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter5x3(Mob *fire,Player* Joueur){
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
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 2:
                    fire->action_delay=2*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=4*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 3;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}

void patrol_counter4x4(Mob *fire,Player* Joueur){
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
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 0;
                    break;
                case 1:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 2;
                    break;
                case 2:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value++;
                    fire->offset_y = 1;
                    break;
                case 3:
                    fire->action_delay=3*fire->max_action_delay;
                    fire->action_value=0;
                    fire->offset_y = 3;
                    break;
            }
        }
        fire->delay = fire->speed;

    } else {
        fire->delay--;
    }
}
//mobs allant dans le sens horaire
const Mob cult_clock1 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock4x3,0,16,0,0,0,0};
const Mob cult_clock2 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock5x3,0,16,0,0,0,0};
const Mob cult_clock3 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_clock4x4,0,16,0,0,0,0};
//mobs allant dans le sens antihoraire
const Mob cult_counter1 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter4x3,0,16,0,0,0,0};
const Mob cult_counter2 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter5x3,0,16,0,0,0,0};
const Mob cult_counter3 = {1,1,true,0,0,0,0,0,MOB_CULTIST,0,0,patrol_counter4x4,0,16,0,0,0,0};

void still(Mob *mage,Player* player){
        if(!mage->action_delay){
            mage->action_delay=((mage->action_value)*8);
            mage->offset_x++;
            mage->offset_x %=4;
        }
        else {
            mage->action_delay--;
        }
        mage->delay = mage->speed; //????
}

void follow(Mob *projectile,Player* player){
    int test_horiz=player->_x-projectile->_x;
    int test_vert=player->_y-projectile->_y;
    if(!projectile->delay){
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
    if(!projectile->action_delay){
        projectile->_x=projectile->memory__x;
        projectile->_y=projectile->memory__y;
        projectile->action_delay=8*(projectile->action_value); 
    }
    else {
        projectile->action_delay--;
    }
    projectile->delay = projectile->speed;
    projectile->offset_x++;
    projectile->offset_x %= 4;
    if (test_horiz<-2){
        projectile->offset_y=4;
        if (test_vert>2) {
            projectile->offset_y=5;
        }else if (test_vert<-2) {
            projectile->offset_y=3;
        } 
    }else if (test_horiz>2){
        projectile->offset_y=0;
        if (test_vert>2) {
            projectile->offset_y=7;
        }else if (test_vert<-2) {
            projectile->offset_y=1;
        } 
    }else {
        if (test_vert>0) {
            projectile->offset_y=6;
        }else if (test_vert<0) {
            projectile->offset_y=2;
        }
    }
    } else {
        projectile->delay--;
    }
}


const Mob mage={1,1,true,0,0,0,0,0,HIGHCULTIST,0,0,still,12,0,0,0,0,0};
const Mob curse_arrow={1,1,true,0,0,0,0,0,CURSE,0,0,follow,12,0,0,0,0,0};
//const Mob curse_arrow_bis_1={1,1,true,0,0,0,0,0,CURSE,0,0,follow,10,100,0,0,0};
//const Mob curse_arrow_bis_2={1,1,true,0,0,0,0,0,CURSE,0,0,follow,10,200,0,0,0};
const Mob pebble_left = {2,2,true,0,0,0,0,0,PEBBLE,0,1,path_left,0,0,0,0,0,0};
const Mob pebble_right = {2,2,true,0,0,0,0,0,PEBBLE,0,1,path_right,0,0,0,0,0,0};


void babel1_1(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x+=2;
            mob->_y-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x+=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 3:
            mob->_y+=2;
            mob->_x-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 4:
            mob->_y-=2;
            mob->_x-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 5:
            mob->_y+=2;
            mob->_x-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 0;
                    break;
                case 1:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 2:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 3:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 4:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 5:
                    mob->action_delay=16;
                    mob->action_value=0;
                    mob->offset_y = 2;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel1_2(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x-=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x-=2;
            mob->_y+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 3:
            mob->_y-=2;
            mob->_x+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 4:
            mob->_y+=2;
            mob->_x+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 5:
            mob->_y-=2;
            mob->_x+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 1:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 0;
                    break;
                case 2:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 3:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 4:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 5:
                    mob->action_delay=16;
                    mob->action_value=0;
                    mob->offset_y = 0;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel2_1(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x+=4;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x+=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 3:
            mob->_y-=2;
            mob->_x-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 4:
            mob->_x-=4;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 5:
            mob->_y+=2;
            mob->_x-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 1:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 0;
                    break;
                case 2:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y =3;
                    break;
                case 3:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 4:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 5:
                    mob->action_delay=16;
                    mob->action_value=0;
                    mob->offset_y = 2;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel2_2(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x-=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x-=4;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 3:
            mob->_y+=2;
            mob->_x+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 4:
            mob->_x+=4;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 5:
            mob->_y-=2;
            mob->_x+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 1:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 2:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 3:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 4:
                    mob->action_delay=16;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 5:
                    mob->action_delay=16;
                    mob->action_value=0;
                    mob->offset_y = 0;
                    break;
            }
        }
        mob->delay = mob->speed;
    } else {
        mob->delay--;
    }
}

void babel3_1(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=4;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x-=2;
            mob->_y-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 1:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 2:
                    mob->action_delay=32;
                    mob->action_value=0;
                    mob->offset_y = 2;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel3_2(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x+=2;
            mob->_y+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=4;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 2;
                    break;
                case 1:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 2:
                    mob->action_delay=32;
                    mob->action_value=0;
                    mob->offset_y = 0;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel4_1(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=4;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x-=2;
            mob->_y+=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=2;
            mob->_y-=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 1;
                    break;
                case 1:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 2:
                    mob->action_delay=32;
                    mob->action_value=0;
                    mob->offset_y = 2;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

void babel4_2(Mob *mob,Player* Joueur){
    if(!mob->delay){
        switch (mob->action_value)
        {
        case 0:
            mob->_x+=2;
            mob->_y+=2;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 1:
            mob->_x+=2;
            mob->_y-=2;
            if(mob->_y%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        case 2:
            mob->_x-=4;
            if(mob->_x%8){
                mob->offset_x++;
                mob->offset_x %=4;
            }
            break;
        default:
            break;
        }
        mob->action_delay--;
        if (mob->action_delay==0) {
            switch (mob->action_value){
                case 0:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 0;
                    break;
                case 1:
                    mob->action_delay=32;
                    mob->action_value++;
                    mob->offset_y = 3;
                    break;
                case 2:
                    mob->action_delay=32;
                    mob->action_value=0;
                    mob->offset_y = 2;
                    break;
            }
        }
        mob->delay = mob->speed;

    } else {
        mob->delay--;
    }
}

const Mob babel11 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel1_1,1,16,0,0,0};
const Mob babel12 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel1_2,5,16,0,0,0};
const Mob babel21 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel2_1,1,16,0,0,0};
const Mob babel22 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel2_2,5,16,0,0,0};
const Mob babel31 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel3_1,0,16,0,0,0};
const Mob babel32 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel3_2,0,16,0,0,0};
const Mob babel41 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel4_1,2,16,0,0,0};
const Mob babel42 = {1,2,true,0,0,0,0,0,BEBERT,0,0,babel4_2,1,16,0,0,0};



///////New_mobs -> Biome Hard_Croussy_Rod

void path_hard_right(Mob *car, Player*p){
    if(car->action_value==2){
        if(!car->action_delay){
            car->offset_y = 1;
            car->action_value=1;
            car->path = path_hard_left;

        } else {
            car->action_delay--;
        }
    } else if(!car->delay){
        if(car->_x/32 >= LENGTH){
            car->action_value=0;
            car->_x = -car->length*32;
        } else {
            car->_x += 3; 
        }
        car->action_delay = !car->action_delay;
        if(car->action_delay&&(car->id_mob!=PLANK)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
        if(car->action_value==0){
            if(((car->_x-16-64)<=p->_x_dest)&&(p->_x_dest <= car->_x + 16 -64 + (car->length - 1)*32)){ //Dépassement supérieur
                if(((car->_y -16 )<=p->_y_dest)&&(p->_y_dest <= car->_y + 16 + (car->heigth - 1)*32)) {
                    car->action_value = 2;
                    car->action_delay = car->max_action_delay;
                }
            }
        }
    } else {
        car->delay--;
    }
}

const Mob magic_car_right = {2,1,true,0,0,0,0,0,MAGIC_CAR,0,0,path_hard_right,0,0,0,0,0,0}; //Note : x et y ini ne veulent rien dire ici 

void path_hard_left(Mob *car, Player*p){
    if(car->action_value==2){
        if(!car->action_delay){
            car->offset_y = 0;
            car->action_value=1;
            car->path = path_hard_right;

        } else {
            car->action_delay--;
        }
    } else if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->action_value=0;
            car->_x = LENGTH*32;
        } else {
            car->_x -= 3; 
        }
        car->action_delay = !car->action_delay;
        if(car->action_delay&&(car->id_mob!=PLANK)){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
        if(car->action_value==0){
            if(((car->_x-16+64)<=p->_x_dest)&&(p->_x_dest <= car->_x + 16 +64 + (car->length - 1)*32)){ 
                if(((car->_y -16 )<=p->_y_dest)&&(p->_y_dest <= car->_y + 16 + (car->heigth - 1)*32)) {
                    car->action_value = 2;
                    car->action_delay = car->max_action_delay;
                }
            }
        }
    } else {
        car->delay--;
    }
}

const Mob magic_car_left = {2,1,true,0,0,0,0,0,MAGIC_CAR,0,1,path_hard_left,0,0,0,0,0,0};

void path_still_right(Mob *car, Player*p){
    if(car->action_value==0){
        if(!car->action_delay){
            car->action_value=car->max_action_delay;

        } else {
            car->action_delay--;
        }
    } else if(!car->delay){
        if(car->_x/32 >= LENGTH){
            car->_x = LENGTH*32;
        } else {
            car->_x += 3; 
        }
        car->action_delay = !car->action_delay;
        if(car->action_delay){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
        car->action_value--;
        if(car->action_value==0){
            car->offset_x = 0;
            car->action_delay = car->max_action_delay;
        }
    }
}

const Mob big_bus_right = {4,1,true,0,0,0,0,0,BUS,0,0,path_still_right,0,0,0,0,0,0};


void path_still_left(Mob *car, Player*p){
    if(car->action_value==0){
        if(!car->action_delay){
            car->action_value=car->max_action_delay;

        } else {
            car->action_delay--;
        }
    } else if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->_x = LENGTH*32;
        } else {
            car->_x -= 3; 
        }
        car->action_delay = !car->action_delay;
        if(car->action_delay){
            car->offset_x ++;
            car->offset_x %= 2;
        }
        car->delay = car->speed;
    } else {
        car->delay--;
        car->action_value--;
        if(car->action_value==0){
            car->offset_x = 0;
            car->action_delay = car->max_action_delay;
        }
    }
}

const Mob big_bus_left = {4,1,true,0,0,0,0,0,BUS,0,1,path_still_left,0,0,0,0,0,0};


void perfectly_still(Mob* mob,Player* player){
}


void get_smoked(Mob* mob,Player*player){
    if(!mob->delay){
        mob->action_value++;
        mob->offset_x++;
        mob->delay = 4;
    } else {
        mob->delay--;
    }
}

const Mob smoke1x1 = {1,1,false,0,0,0,0,0,SMOKE1x1,0,0,get_smoked,0,0,0,0,0,0};
const Mob smoke2x1 = {2,1,false,0,0,0,0,0,SMOKE2x1,0,0,get_smoked,0,0,0,0,0,0};
const Mob smoke1x2 = {1,2,false,0,0,0,0,0,SMOKE1x2,0,0,get_smoked,0,0,0,0,0,0};
const Mob smoke2x2 = {2,2,false,0,0,0,0,0,SMOKE2x2,0,0,get_smoked,0,0,0,0,0,0};
const Mob smoke4x1 = {4,1,false,0,0,0,0,0,SMOKE4x1,0,0,get_smoked,0,0,0,0,0,0};


void go_arrow_go(Mob* mob,Player* player){
    if(mob->action_value){
            mob->delay = 1 - mob->delay;
            if(mob->delay){
                mob->offset_x += 1;
                mob->offset_x %= 4;
            }
            switch(mob->offset_y){
                case 0:
                    mob->_y -= 4;
                    break;

                case 1:
                    mob->_y += 4;
                    break;
                
                case 2:
                    mob->_x += 4;
                    break;

                case 3:
                    mob->_x -= 4;
                    break;
            }

            if(!mob_in_map(mob)){
                mob->_x = -4242;
                mob->_y = 6969;
                mob->action_value = 0;
            }
    } else {
         mob->_x = -4242;
         mob->_y = 6969;
    }


}
const Mob arrow_kabi = {1,1,false,0,0,0,0,0,ARROW_KABI,0,0,go_arrow_go,0,0,0,0,0,0};

void get_cibled(Mob* mob, Player* player){
    if(player->character_id == OOGWAAY){
            switch(player->offset_y){
                case 0:
                    mob->_x = player->_x_dest;
                    mob->_y = player->_y_dest -64;
                    break;

                case 1:
                    mob->_x = player->_x_dest;
                    mob->_y = player->_y_dest + 64;
                    break;

                case 2:
                    mob->_x = player->_x_dest + 64;
                    mob->_y = player->_y_dest;
                    break;

                case 3:
                    mob->_x = player->_x_dest - 64;
                    mob->_y = player->_y_dest;
                    break;

        }

    }
}

const Mob cible = {1,1,true,0,0,0,0,0,CIBLE,0,0,get_cibled,0,0,0,0,0,0};
