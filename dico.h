#pragma once

#include <stdbool.h>

typedef struct _tile {
    bool is_block;
    bool is_danger;
    char type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} tile;

#define IS_BLOCK(tile) (tile->is_block)
#define IS_DANGER(tile) (tile->is_danger)
#define TYPE(tile) (tile->type)

typedef struct _map_dico
{
    /* data */
}map_dico;

//fonctions souhaités (Louan)
map_dico *create_empty_map(void);
void ajout_tile(map_dico carte,int x,int y,tile *tuile);//on modifie le dico, on en renvoie pas un nouveau
void suppression_tile(map_dico carte,int x,int y);
bool coord_remplie(map_dico carte,int x,int y);
bool carte_vide(map_dico carte);
int nb_tiles_remplies(map_dico carte);
tile *tile_carte(map_dico carte,int x,int y);
