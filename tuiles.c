#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



const tile plain = {false,false,'g','_'};

const tile road = {false,false,'r','_'};

const tile  wall = {true,false,'a','#'};

const tile water = {false,true,'b','~'};

const tile tree = {true,false,'g','I'};

const tile danger = {false,true,'r','!'};

tile* clone(tile t){
    tile* new_tile = malloc(sizeof(tile));
    new_tile->color = t.color;
    new_tile->is_block = t.is_block;
    new_tile->is_danger = t.is_danger;
    new_tile->type = t.type;
    return new_tile;
}
