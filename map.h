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
    Line* tail_buffer;
    int tail_buffer_lenght;
    /* data */
}Map;

typedef struct _Player{
    int _x;
    int _y;
    int _x_dest;
    int _y_dest;
    int offset_x;
    int offset_y;
    int character_id;
    int delai;
}Player;

enum Personnages {
    KAIRON,
    SNITCH,
    AESIR,
    KABI,
    OOGWAAY,
    JOLINAR,
    RANTON,
    DEDE,
    NB_PERSO
};

#define DELAI 10

Line* create_empty_line(Line* prev, Line* next);

//fonctions souhaités (Louan)
Map* create_empty_map(void); //Done
bool carte_vide(Map *map); //Done
Tile tile_map(Map *carte,int x,int y); //Elle renvoie qu'elle est la tile (x,y) , done
void switch_map(Map *carte);

Player* initialise_player(int);

//fonctions bonus
void add_buffer_line(Map* map);
void remove_buffer_line(Map* map);
void destroy_line(Line *line);
void destroy_tail_buffer(Map* map);
void destroy_last_buffer(Map* map);
void destroy_map(Map *map);

