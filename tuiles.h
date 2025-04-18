#pragma once
#include <stdbool.h>

//Table de couleur : 
typedef struct _Tile {
    bool is_block;
    bool is_danger;
    int type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} Tile;
//g = green, r = red, b = blue, a = gray 
#define IS_BLOCK(tile) (tile.is_block)
#define IS_DANGER(tile) (tile.is_danger)
#define TYPE(tile) (tile.type)
#define COLOR(tile) (tile.color)

#define LENGHT 16
#define HEIGHT 16


enum TILE{
    _PLAIN,
    _ROAD,
    _WALL,
    _WATER,
    _TREE,
    _DANGER,
    _RUINS,
    _BROKENPLANKS,
    _TRANSITION,
    _PLANKS,
    _BURNINGPLANKS,
    _LOW_WALL,
    _CAVE,
    _STALAGMITE,
    NB_TILE

};

extern const Tile plain;

extern const Tile road;

extern const Tile  wall;

extern const Tile water;

extern const Tile tree;

extern const Tile danger;

void clone(Tile *dest,Tile src);



//////////////////////////////////////
extern const Tile ruins;

extern const Tile brokenplanks;

extern const Tile transition;

extern const Tile planks;

extern const Tile burningplanks;

extern const Tile low_wall ;

extern const Tile cave;

extern const Tile stalagmite;

