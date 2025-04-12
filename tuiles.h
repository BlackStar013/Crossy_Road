#pragma once
#include <stdbool.h>

//Table de couleur : 
typedef struct _Tile {
    bool is_block;
    bool is_danger;
    char color;
    char type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} Tile;
//g = green, r = red, b = blue, a = gray 
#define IS_BLOCK(tile) (tile.is_block)
#define IS_DANGER(tile) (tile.is_danger)
#define TYPE(tile) (tile.type)
#define COLOR(tile) (tile.color)

#define LENGHT 16
#define HEIGHT 16

extern const Tile plain;

extern const Tile road;

extern const Tile  wall;

extern const Tile water;

extern const Tile tree;

extern const Tile danger;

void clone(Tile *dest,Tile src);
