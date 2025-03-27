#pragma once
//#include "fonctions.h"
#include "tuiles.h"
#include <stdbool.h>

typedef struct _Mob {
    int heigth;
    int length;
    int x;
    int y;
    void (*path)(struct _Mob*);
} Mob;

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


