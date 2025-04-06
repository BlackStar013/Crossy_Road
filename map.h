#pragma once

#include <stdbool.h>
#include "tuiles.h"


typedef struct _Line{
    //La tailles de tiles est LENGTH 
    Tile* tiles;
    struct _Line* prev;
    struct _Line* next; 
}Line;

typedef struct _Map{
    Line* head_buffer;
    Line* head_map;
    Line* tail;
    /* data */
}Map;

typedef struct _Player{
    int x;
    int y;
    int offset_x;
    int offset_y;
    int character_id;
}Player;

#define HEIGTH 16
#define LENGTH 16

//fonctions souhaités (Louan)
Map* create_empty_map(void); //Done
void ajout_tile(Map *carte,int x,int y,Tile tuile); //Done
void suppression_tile(Map *carte,int x,int y); //? A quoi sert cette fonction vu qu'on a pas malloc les tiles individuellement
bool coord_remplie(Map *carte,int x,int y); //Done
bool carte_vide(Map *map); //Done
int nb_tiles_remplies(Map *carte); //return LENGHT*HEIGHT?
Tile tile_map(Map *carte,int x,int y); //Elle renvoie qu'elle est la tile (x,y) , done
void switch_map(Map *carte);

Player* initialise_player(int);

//fonctions bonus
void add_buffer_line(Map* map);
void remove_buffer_line(Map* map);
void destroy_line(Line *line);
void destroy_map(Map *map);
