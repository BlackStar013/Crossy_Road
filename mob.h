#pragma once
#include "tuiles.h"
#include "map.h"
#include <stdbool.h>

typedef struct _Mob {
    int heigth;
    int length;
    int x;
    int y;
    void (*path)(struct _Mob*);
} Mob;

typedef struct _List_Mob {
    Mob* mob;
    struct _List_Mob* next;
} Liste_Mob;

//On gère la vitesse à l'aide du nombre d'actualisation

void move_mob(Mob*); //A implémenter après ajout de la structure finale de la carte (?)
bool mob_in_map(Mob*);
Mob* clone_mob(Mob,int,int);

void path_car_right(Mob*);
extern const Mob car_right;
extern const Mob bus_right;

void path_car_left(Mob*);
extern const Mob car_left;
extern const Mob bus_left;

//Fonction liés à List_Mob
Liste_Mob* list_create(Mob* mob);
void list_destroy(Liste_Mob* list);
bool list_is_empty(Liste_Mob* list);
void list_append(Liste_Mob* list, Mob* mob);
void list_print(Liste_Mob* list);
void move_all_mob(Liste_Mob* list);

