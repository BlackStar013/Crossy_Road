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

#define LENGTH 12
#define HEIGTH 12


enum TILE{
    _PLAIN,
    _PLAIN_BLOCK_LEFT,
    _PLAIN_BLOCK_RIGHT,
    _ROAD,
    _ROAD_UP,
    _ROAD_DOWN,
    _ROAD_MIDDLE,
    _WALL,
    _WATER,
    _WATER_UP,
    _WATER_DOWN,
    _WATER_MIDDLE,
    _TREE,
    _DANGER,
    _RUINS,
    _BROKENPLANKS,
    _TRANSITION,
    _PLANKS,
    _CHANNEL,
    _BASE,
    _LOW_WALL,
    _CAVE,
    _STALAGMITE,
    _MANOR_WALL,
    _DOOR,
    _FLESH,
    _ACID,
    _ACID_UP,
    _ACID_DOWN,
    _ACID_MIDDLE,
    _MAW,
    _SHOOTER_BLOCK_LEFT,
    _SHOOTER_BLOCK_RIGHT,
    _TRANSITION_HEART,
    NB_TILE

};

extern const Tile plain;

extern const Tile plain_block_left;

extern const Tile plain_block_right;

extern const Tile road;

extern const Tile road_up;

extern const Tile road_down;

extern const Tile road_middle;

extern const Tile  wall;

extern const Tile water;

extern const Tile water_up;

extern const Tile water_down;

extern const Tile water_middle;


extern const Tile tree;

extern const Tile danger;

void clone(Tile *dest,Tile src);



//////////////////////////////////////
extern const Tile ruins;

extern const Tile brokenplanks;

extern const Tile transition;

extern const Tile planks;

extern const Tile low_wall ;

extern const Tile cave;

extern const Tile stalagmite;

extern const Tile channel;

extern const Tile base;

extern const Tile MurManoir;

extern const Tile Porte;

extern const Tile Flesh;
extern const Tile Acid;
extern const Tile Acid_up;
extern const Tile Acid_down;
extern const Tile Acid_middle;
extern const Tile Maw;
extern const Tile Shooter_block_right;
extern const Tile Shooter_block_left;
extern const Tile transition_finale;
