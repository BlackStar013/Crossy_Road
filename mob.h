#pragma once
#include "tuiles.h"
#include "map.h"
#include <stdbool.h>

typedef struct _Mob {
    int heigth;
    int length;
    bool is_death;
    int move_x;
    int x;
    int y;
    int speed; // ==Delay_max 
    int delay;
    int id_mob;
    void (*path)(struct _Mob*);
} Mob;

enum MOB{
    CAR_RIGHT,
    CAR_LEFT,
    BUS_RIGHT,
    BUS_LEFT,
};

typedef struct _List_Mob {
    Mob* mob;
    struct _List_Mob* next;
} Liste_Mob;

bool death_mob_collide(Mob*,Player*);
bool mob_collide(Mob*,Player*);

//On gère la vitesse à l'aide du nombre d'actualisation

bool mob_in_map(Mob*);
Mob* clone_mob(Mob,int,int,int);

void path_right(Mob*);
extern const Mob car_right;
extern const Mob bus_right;

void path_left(Mob*);
extern const Mob car_left;
extern const Mob bus_left;

//Fonction liés à List_Mob
Liste_Mob* list_create(Mob* mob);
void list_destroy(Liste_Mob* list);
bool list_is_empty(Liste_Mob* list);
Liste_Mob *list_append(Liste_Mob* list, Mob* mob);
void print_liste(Liste_Mob* list);
void move_all_mob(Liste_Mob* list);
void switch_mob(Liste_Mob* list);
bool a_death_mob_collide(Liste_Mob* list,Player* player);
bool a_mob_collide(Liste_Mob* list,Player* player);
void destroy_mobs(Liste_Mob *liste);
void destroy_liste_mob(Liste_Mob *liste);
