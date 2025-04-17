#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const Tile plain = {false,false,_PLAIN};

const Tile road = {false,false,_ROAD};

const Tile wall = {true,false,_WALL};

const Tile water = {false,true,_WATER};

const Tile tree = {true,false,_TREE};

const Tile danger = {false,true,_DANGER};

void clone(Tile *dest, Tile src){
    dest->is_block = src.is_block;
    dest->is_danger = src.is_danger;
    dest->type = src.type;
}
