#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mob.h"

///List_Map///

Liste_Mob* list_create(Mob* mob){
    Liste_Mob* list = malloc(sizeof(Liste_Mob));
    list->mob = mob;
    list->next = NULL;
    return list;
}

Liste_Mob *list_append(Liste_Mob* list, Mob* mob){
    ///!\Toujour mettre des clones en entrer bande de malandrin
    if(list==NULL){
        return list_create(mob);
    } else {
        list->next = list_append(list->next, mob);
        return list;
    }
}

void print_rest_liste(Liste_Mob *list){
    if(list==NULL){
        printf(" ]\n");
    }else{
        printf(", %d", list->mob->y);
        print_rest_liste(list->next);
    }
}

void print_liste(Liste_Mob *list){
    if(list==NULL){
        printf("[]\n");
    }else{
        printf("[ %d", list->mob->id_mob);
        print_rest_liste(list->next);
    }
}

void move_all_mob(Liste_Mob* list){
    if(list!=NULL){
        (list->mob->path)(list->mob);
        move_all_mob(list->next);
    }
}

void switch_mob(Liste_Mob* list){
    if(list!=NULL){
        list->mob->y--;
        switch_mob(list->next);
    }
}

bool a_death_mob_collide(Liste_Mob* list,Player* player){
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
    if(((mob->x)>LENGTH)||((mob->y)>HEIGTH)){ //Dépassement supérieur
        return false;
    } else if(((mob->x + mob->length - 1)<0)||((mob->y + mob->heigth - 1)<0)) {
        return false;
    } else { 
        return true;
    }
}

Mob* clone_mob(Mob mob, int x_ini, int y_ini){
    Mob *new_mob = malloc(sizeof(mob));
    new_mob->heigth = mob.heigth;
    new_mob->length = mob.length;
    new_mob->is_death = mob.is_death;
    new_mob->move_x = mob.move_x;
    new_mob->x = x_ini;
    new_mob->y = y_ini;
    new_mob->delay = mob.delay;
    new_mob->id_mob = mob.id_mob;
    new_mob->path = mob.path;
    return new_mob;
}

bool death_mob_collide(Mob* mob,Player* player){
    if(!mob->is_death){
        return false;
    }
    if(((mob->x)<=player->x)&&(player->x <= mob->x + mob->length - 1)){ //Dépassement supérieur
        if(((mob->y)<=player->y)&&(player->y <= mob->y + mob->heigth - 1)) {
         return true;
        }
    }
    return false;
}

bool mob_collide(Mob* mob, Player* player){
    if(((mob->x)<=player->x)&&(player->x <= mob->x + mob->length - 1)){ //Dépassement supérieur
        if(((mob->y)<=player->y)&&(player->y <= mob->y + mob->heigth - 1)) {
         return true;
        }
    }
    return false;
}

///Création de mobs et pathfinding

void path_car_right(Mob *car){
    if(!car->delay){
        if(!mob_in_map(car)){
            car->x = -1;
        } else {
            car->x +=1; //À remplacer par move_mob (?)
        }
        car->delay = 0;
    } else {
        car->delay--;
    }
}

const Mob car_right = {1,2,true,0,0,0,0,CAR_RIGHT,path_car_right}; //Note : x et y ini ne veulent rien dire ici
const Mob bus_right = {1,4,true,0,0,0,0,BUS_RIGHT,path_car_right};

void path_car_left(Mob *car){
    if(!car->delay){
        if(!mob_in_map(car)){
            car->x = LENGTH;
        } else {
            car->x -=1; //À remplacer par move_mob (?)
        }
        car->delay = 0;
    } else {
        car->delay--;
    }
}

const Mob car_left = {1,2,true,0,0,0,0,CAR_LEFT,path_car_left}; 
const Mob bus_left = {1,4,true,0,0,0,0,BUS_LEFT,path_car_left};



