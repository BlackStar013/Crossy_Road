#pragma once
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// typedef struct _IA{
//     int _x;
//     int _y;
//     int _x_dest;
//     int _y_dest;
//     int offset_x;
//     int offset_y;
//     int character_id;
// }IA;

void eval(Game *game);
int max_score(int *score);
