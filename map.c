#include "map.h"
#include "tuiles.h"
#include <stdio.h>
#include <stdlib.h>

Line* create_empty_line(Line* prev, Line* next){
    //create a new empty line and connects it with the previous one and the next one if there are
    Line* nl = malloc(sizeof(Line));
    nl->tiles = malloc(sizeof(Tile)*LENGTH);
    for(int i = 0; i<LENGTH; i++){
        (nl->tiles[i]).is_block = false;
        (nl->tiles[i]).is_danger = false;
        (nl->tiles[i]).color = 'g';
        (nl->tiles[i]).type = '_';
    }
    if(next == NULL){
        nl->next = NULL;
    }
    if(prev == NULL){
        nl->prev = NULL;
    }
    if(next != NULL){
        nl->next = next;
        (nl->next)->prev = nl;
    }
    if(prev != NULL){
        nl->prev = prev;
        (nl->prev)->next = nl;
    }
    return nl;
}

void add_buffer_line(Map* map){
    //buffer ->* head ->* tail
    map->head_buffer = create_empty_line(NULL, map->head_buffer);
}

void remove_buffer_line(Map* map){
    //removes buffer lines if there are
    if (map->head_buffer != map->head_map){
        Line* tmp = map->head_buffer;
        map->head_buffer = (map->head_buffer)->next;
        free(tmp->tiles);
        free(tmp);
    }
}

Map* create_empty_map(void){
    Map* map = malloc(sizeof(Map));
    map->head_map = create_empty_line(NULL, NULL);
    map->head_buffer = map->head_map;
    Line* cur = map->head_map;
    for(int i = 1; i<HEIGTH; i++){
        cur = create_empty_line(cur, NULL);
    }
    map->tail = cur;
    return map;
}

bool carte_vide(Map* map){
    Line* cur = map->head_map;
    for(int i = 0; i<HEIGTH; i++){
        for(int j = 0; j<LENGTH; j++){
            if((cur->tiles[j]).type != ' '){
                return false;
            }
        }
        cur = cur->next;
    }
    return true;
}

void switch_map(Map *carte){
    // ajoute la premiere ligne du buffer a la map, et retire la dernière
    carte->head_map = carte->head_map->prev;
    Line* last = carte->tail;
    carte->tail = carte->tail->prev;
    carte->tail->next = NULL;
    destroy_line(last);
}

void suppression_tile(Map *carte,int x,int y);
int nb_tiles_remplies(Map *carte);


Tile tile_map(Map *carte, int x, int y){
    // renvoie la tuile de la map a ces coordonnées, et empêche de sortir des côtés de la carte
    if((y>=HEIGTH)||(x>=LENGTH)||(y<0)||(x<0)){
        return wall;
    }
    Line* cur = carte->tail;
    while(y>0){
        cur = cur->prev;
        y--;
    }
    //printf("%d\n", cur->tiles[x].is_block);
    return ((cur->tiles)[x]);
}

Player* initialise_player(int id){
    // intitialise le joueur au milieu en bas de la carte
    Player* joueur = malloc(sizeof(Player));
    joueur->x = (LENGTH-1)/2;
    joueur->y = 2;
    joueur->offset_x = 0;
    joueur->offset_y = 0;
    joueur->character_id = id;
    return joueur;
}

void destroy_line(Line *line){
    free(line->tiles);
    free(line);
}

void destroy_map(Map *map){
    Line *line = map->head_buffer;
    while(line != NULL){
        Line *mem = line->next;
        destroy_line(line);
        line = mem;
    }
}
