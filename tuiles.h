#pragma once
//#include "fonctions.h"
#include <stdbool.h>

//Table de couleur : 
//g = green, r = red, b = blue, a = gray 

//Aide : struct utilisée :
typedef struct _tile {
    bool is_block;
    bool is_danger;
    char color;
    char type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} tile;

extern const tile plain;

extern const tile road;

extern const tile  wall;

extern const tile water;

extern const tile tree;

extern const tile danger;

tile* clone(tile);
