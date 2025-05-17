#pragma once
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _Point{
    int vu;
    int val;
    int i;
    int j;
    int *chemin;
}Point;

typedef struct _File{
    //mettre (i,j) du joueur à (0,0) comme ça profondeur et nbupdate == i+j
    struct _File *next;
    Point *sommet;
}File;

#define PROFONDEUR 7
//vraie profondeur = 5/2 car 5 == taille du tableau pour l'evaluation, mais donc pour profondeur 2 => PROFONDEUR = 2*2+1; 3=> PROFONDEUR = 3*2+1; ...


int val_absolue(int x);
File *create_file(Point *sommet);
void append_file(File *file, Point *sommet);
File *defile(File *file);
void destroy_file(File *file);
bool move_IA(int x, int y, Player *joueur, Map *map);
bool test_death_IA(Player *joueur, Map *map, Liste_Mob *liste, Liste_Mob *projectiles);
Liste_Mob *clone_listemob(Liste_Mob *liste);
Mob *clone_mob_IA(Mob *mob);
void update_game_IA(Liste_Mob *liste, Player *joueur);
Point *create_point(int i, int j, int val);
Point ***matrice_evaluation(void);
void print_matrice(Point ***evaluation);
void destroy_matrice(Point ***matrice_eval);
int eval(Game *game);
