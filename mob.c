#include "PrototypeRoutes.h"
#include "tuiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mob.h"

bool mob_in_map(Mob *mob){
    if(((mob->x - mob->length + 1)>LENGTH)||((mob->y - mob->heigth + 1)>HEIGTH)){ //Dépassement supérieur
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
    new_mob->x = x_ini;
    new_mob->y = y_ini;
    new_mob->path = mob.path;
    return new_mob;
}


void path_car_right(Mob *car){
    if(!mob_in_map(car)){
        car->x = -1;
    } else {
        car->x +=1; //À remplacer par move_mob (?)
    } 
}

const Mob car_right = {1,3,0,0,path_car_right}; //Note : x et y ini ne veulent rien dire ici
const Mob bus_right = {1,6,0,0,path_car_right};

void path_car_left(Mob *car){
    if(!mob_in_map(car)){
        car->x = LENGTH;
    } else {
        car->x -=1; //À remplacer par move_mob (?)
    } 
}

const Mob car_left = {1,3,0,0,path_car_left}; 
const Mob bus_left = {1,6,0,0,path_car_left};
