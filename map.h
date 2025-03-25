#pragma once

#include <stdbool.h>

typedef struct _Line {
    //La tailles de tiles est LENGTH 
    tile* tiles;
    _Line* prev;
    _Line* next; 
} Line;

typedef struct _Tile {
    bool is_block;
    bool is_danger;
    char color;
    char type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} Tile;

#define IS_BLOCK(tile) (tile->is_block)
#define IS_DANGER(tile) (tile->is_danger)
#define TYPE(tile) (tile->type)

typedef struct _Map
{
    Line* buffer;
    Line* head;
    Line* tail;
    /* data */
}Map;


//fonctions souhaités (Louan)
Map* create_empty_map(void); //Done
void ajout_tile(Map *carte,int x,int y,Tile tuile); //Done
void suppression_tile(Map *carte,int x,int y); //? A quoi sert cette fonction vu qu'on a pas malloc les tiles individuellement
bool coord_remplie(Map *carte,int x,int y); //Done
bool carte_vide(Map *map); //Done
int nb_tiles_remplies(Map *carte); //return LENGHT*HEIGHT?
Tile* tile_carte(Map *carte,int x,int y); //Que fait cette fonction?

//fonctions bonus
void add_buffer_line(Map* map);
void remove_buffer_line(Map* map);