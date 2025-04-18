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

void move_all_mob(Liste_Mob* list,Player* player){
    // bouge tous les mobs selon leur path
    if(list!=NULL){
        (list->mob->path)(list->mob,player);
        move_all_mob(list->next,player);
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

void path_right(Mob *car,Player*){
    if(!car->delay){
        if(car->_x/32 >= LENGHT){
            car->_x = -car->length*32;
        } else {
            car->_x += 3; 
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_right = {1,2,true,0,0,0,0,0,CAR_RIGHT,path_right,0,0,0,0}; //Note : x et y ini ne veulent rien dire ici
const Mob plank_right = {1,1,false,3,0,0,0,0,PLANK_RIGHT,path_right,0,0,0,0}; 
const Mob bus_right = {1,4,true,0,0,0,0,0,BUS_RIGHT,path_right,0,0,0,0};

void path_left(Mob *car,Player*){
    if(!car->delay){
        if(car->_x/32 < -car->length+1){
            car->_x = LENGTH*32;
        } else {
            car->_x -=3; //À remplacer par move_mob (?)
        }
        car->delay = car->speed;
    } else {
        car->delay--;
    }
}

const Mob car_left = {1,2,true,0,0,0,0,0,CAR_LEFT,path_left,0,0,0,0};
const Mob plank_left = {1,1,false,-3,0,0,0,0,PLANK_LEFT,path_left,0,0,0,0};
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
