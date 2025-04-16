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

void move_all_mob(Liste_Mob* list,Player* Joueur){
    // bouge tous les mobs selon leur path
    if(list!=NULL){
        (list->mob->path)(list->mob,Joueur);
        move_all_mob(list->next,Joueur);
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
        return ((mob_collide(list->mob, player))||(a_death_mob_collide(list->next, player)));
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

Mob* clone_mob(Mob mob, int x_ini, int y_ini, int speed){
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
    new_mob->path = mob.path;
    new_mob->action_delay=mob.action_delay;
    new_mob->action_value=mob.action_value;
    new_mob->memory__x=x_ini*32;;
    new_mob->memory__y=y_ini*32;
    return new_mob;
}

bool death_mob_collide(Mob* mob,Player* player){
    if(!mob->is_death){
        return false;
    }
    if(((mob->_x)<=player->_x)&&(player->_x <= mob->_x + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y)<=player->_y)&&(player->_y <= mob->_y + (mob->heigth - 1))*32) {
         return true;
        }
    }
    return false;
}

bool mob_collide(Mob* mob, Player* player){
    if(((mob->_x)<=player->_x)&&(player->_x <= mob->_x + (mob->length - 1)*32)){ //Dépassement supérieur
        if(((mob->_y)<=player->_y)&&(player->_y <= mob->_y + (mob->heigth - 1)*32)) {
         return true;
        }
    }
    return false;
}

///Création de mobs et pathfinding

void path_right(Mob *car,Player* Joueur){
    if(!car->delay){
        if(car->_x/32 >= LENGHT){
            car->_x = -car->length*32;
        } else {
            car->_x += 2; 
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_right = {1,2,true,0,0,0,0,0,CAR_RIGHT,path_right,0,0,0,0}; //Note : x et y ini ne veulent rien dire ici
const Mob plank_right = {1,1,false,2,0,0,0,0,PLANK_RIGHT,path_right,0,0,0,0}; 
const Mob bus_right = {1,4,true,0,0,0,0,0,BUS_RIGHT,path_right,0,0,0,0};

void path_left(Mob *car,Player* Joueur){
    if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->_x = LENGTH*32;
        } else {
            car->_x -=2; //À remplacer par move_mob (?)
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_left = {1,2,true,0,0,0,0,0,CAR_LEFT,path_left,0,0,0,0};
const Mob plank_left = {1,1,false,-2,0,0,0,0,PLANK_LEFT,path_left,0,0,0,0}; 
const Mob bus_left = {1,4,true,0,0,0,0,0,BUS_LEFT,path_left,0,0,0,0};

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

//////////////////////////////////////////////////////////////////////
//Mobs avancés
//////////////////////////////////////////////////////////////////////

void clockwise_cycle_2(Mob *fire,Player* Joueur){
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
            fire->action_delay=16;
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

const Mob fire_2_clock = {1,1,true,0,0,0,0,0,FIRE2CLOCK,clockwise_cycle_2,0,16,0,0};

void counterclock_cycle_2(Mob *fire,Player* Joueur){
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
            fire->action_delay=16;
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

const Mob fire_2_counter = {1,1,true,0,0,0,0,0,FIRE2CLOCK,counterclock_cycle_2,0,16,0,0};

void clockwise_cycle_3(Mob *fire,Player* Joueur){
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
            fire->action_delay=32;
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

const Mob fire_3_clock = {1,1,true,0,0,0,0,0,FIRE2CLOCK,clockwise_cycle_3,0,32,0,0};

void counterclock_cycle_3(Mob *fire,Player* Joueur){
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
            fire->action_delay=32;
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

const Mob fire_3_counter = {1,1,true,0,0,0,0,0,FIRE2CLOCK,counterclock_cycle_3,0,32,0,0};

void ambush(Mob *bush,Player* player){
        if(!bush->delay){
            if (player->_x-bush->_x<32 && player->_x-bush->_x>0 && (player->_y-bush->_y<8 && bush->_y-player->_y<8)){
                bush->_x+=4;
            }
            if (bush->_x-player->_x<32 && bush->_x-player->_x>0 &&(player->_y-bush->_y<8 && bush->_y-player->_y<8)){
                bush->_x-=4;
            }
            if (player->_y-bush->_y<32 && player->_y-bush->_y>0 && (player->_x-bush->_x<8 && bush->_x-player->_x<8)){
                bush->_y+=4;
            }
            if (bush->_y-player->_y<32 && bush->_y-player->_y>0 && (player->_x-bush->_x<8 && bush->_x-player->_x<8)){
                bush->_y-=4;
            }
        if(!bush->action_delay){
            bush->action_delay=bush->action_value;
        }
        else {
            bush->action_delay--;
        }
        bush->delay = bush->speed;
    } else {
        bush->delay--;
    }
}

const Mob bush = {1,1,true,0,0,0,0,0,BUSH,ambush,0,8,0,0};

void patrol_clock4x2(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=48;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=48;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

void patrol_clock5x2(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=64;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=64;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

void patrol_clock4x3(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=32;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=48;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=32;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=48;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

void patrol_counter4x2(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=48;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=48;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

void patrol_counter5x2(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=64;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=16;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=64;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

void patrol_counter4x3(Mob *fire,Player* Joueur){
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
            switch (fire->action_value){
                case 0:
                    fire->action_delay=32;
                    fire->action_value++;
                    break;
                case 1:
                    fire->action_delay=48;
                    fire->action_value++;
                    break;
                case 2:
                    fire->action_delay=32;
                    fire->action_value++;
                    break;
                case 3:
                    fire->action_delay=48;
                    fire->action_value=0;
                    break;
            }
        }
        fire->delay = fire->speed;
    } else {
        fire->delay--;
    }
}

const Mob cult_clock1 = {1,1,true,0,0,0,0,0,CULTIST1CLOCK,patrol_clock4x2,0,48,0,0};
const Mob cult_clock2 = {1,1,true,0,0,0,0,0,CULTIST2CLOCK,patrol_clock5x2,0,64,0,0};
const Mob cult_clock3 = {1,1,true,0,0,0,0,0,CULTIST3CLOCK,patrol_clock4x3,0,48,0,0};
const Mob cult_counter1 = {1,1,true,0,0,0,0,0,CULTIST1COUNTER,patrol_counter4x2,0,48,0,0};
const Mob cult_counter2 = {1,1,true,0,0,0,0,0,CULTIST2COUNTER,patrol_counter5x2,0,64,0,0};
const Mob cult_counter3 = {1,1,true,0,0,0,0,0,CULTIST3COUNTER,patrol_counter4x3,0,48,0,0};

void still(Mob *mage,Player* player){
        if(!mage->action_delay){
        mage->action_delay=mage->action_value;
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
        projectile->action_delay=projectile->action_value;
        if (mob_in_map(projectile)){
            projectile->_x=99999999;
        } else {
            projectile->_x=projectile->memory__x;
            projectile->_y=projectile->memory__y;
        }
    }
    else {
        projectile->action_delay--;
    }
    projectile->delay = projectile->speed;
    } else {
        projectile->delay--;
    }
}


const Mob mage={1,1,true,0,0,0,0,0,HIGHCULTIST,still,0,0,0,0};
const Mob curse_arrow={1,1,true,0,0,0,0,0,CURSE,follow,0,0,0,0};
