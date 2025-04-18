#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const Tile plain = {false,false,_PLAIN};

const Tile plain_block = {true,false,_PLAIN_BLOCK};

const Tile road = {false,false,_ROAD};

const Tile road_up = {false,false,_ROAD_UP};

const Tile road_down = {false,false,_ROAD_DOWN};

const Tile road_middle = {false,false,_ROAD_MIDDLE};


const Tile wall = {true,false,_WALL};

const Tile water_up = {false,true,_WATER_UP};

const Tile water_down = {false,true,_WATER_DOWN};

const Tile water_middle = {false,true,_WATER_MIDDLE};


const Tile water = {false,true,_WATER};

const Tile tree = {true,false,_TREE};

const Tile danger = {true,true,_DANGER};

void clone(Tile *dest, Tile src){
    dest->is_block = src.is_block;
    dest->is_danger = src.is_danger;
    dest->type = src.type;
}




///////////////////////////////////////////////////////////////////
const Tile ruins = {true,false,_RUINS};

const Tile brokenplanks = {true,false,_BROKENPLANKS};

const Tile transition = {false,false,_TRANSITION};

const Tile planks = {false,false,_PLANKS};

const Tile burningplanks = {false,false,_BURNINGPLANKS};

const Tile low_wall = {true,false,_LOW_WALL};

const Tile cave = {false,false,_CAVE};

const Tile stalagmite = {true,false,_STALAGMITE};
