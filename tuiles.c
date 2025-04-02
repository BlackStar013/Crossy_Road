#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



const Tile plain = {false,false,'g','_'};

const Tile road = {false,false,'r','_'};

const Tile  wall = {true,false,'a','#'};

const Tile water = {false,true,'b','~'};

const Tile tree = {true,false,'g','I'};

const Tile danger = {false,true,'r','!'};

Tile* clone(Tile t){
    Tile* new_tile = malloc(sizeof(Tile));
    new_tile->color = t.color;
    new_tile->is_block = t.is_block;
    new_tile->is_danger = t.is_danger;
    new_tile->type = t.type;
    return new_tile;
}
