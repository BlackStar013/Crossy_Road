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

bool move_IA(int x, int y, Player *joueur, Map *map);
bool test_death_IA(Player *joueur, Map *map, Liste_Mob *liste);
Liste_Mob *clone_listemob(Liste_Mob *liste);
Mob *clone_mob_IA(Mob *mob);
void update_game_IA(Liste_Mob *liste, Player *joueur);
bool eval(Game *game);
int max_score(int *score);
