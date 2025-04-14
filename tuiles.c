#include "tuiles.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const Tile plain = {false,false,'g','_'};

const Tile road = {false,false,'r','_'};

const Tile wall = {true,false,'a','#'};

const Tile water = {false,true,'b','~'};

const Tile tree = {true,false,'g','I'};

const Tile danger = {false,true,'r','!'};

void clone(Tile *dest, Tile src){
    dest->color = src.color;
    dest->is_block = src.is_block;
    dest->is_danger = src.is_danger;
    dest->type = src.type;
}

//TODO débris,plancher,décombres,plancher ardent,muret,!grotte,!stalagmite