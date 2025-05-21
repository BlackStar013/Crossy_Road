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

const Tile low_wall = {true,false,_LOW_WALL};

const Tile cave = {false,false,_CAVE};

const Tile stalagmite = {true,false,_STALAGMITE};

const Tile channel = {false,false,_CHANNEL};

const Tile base = {false,false,_BASE};

const Tile MurManoir = {true,false,_MANOR_WALL};

const Tile Porte = {false,false,_DOOR};

const Tile Flesh= {false,false,_FLESH};
const Tile Acid= {false,true,_ACID};
const Tile Acid_up= {false,true,_ACID_UP};
const Tile Acid_down= {false,true,_ACID_DOWN};
const Tile Acid_middle= {false,true,_ACID_MIDDLE};
const Tile Maw= {false,true,_MAW};
const Tile Shooter_block_right= {true,false,_SHOOTER_BLOCK_RIGHT};
const Tile Shooter_block_left= {true,false,_SHOOTER_BLOCK_LEFT};
const Tile transition_finale= {true,false,_TRANSITION_HEART};
