#include "map.h"

Line* create_empty_line(Line* prev, Line* next){
    //create a new empty line and connects it with the previous one and the next one if there are
    Line* nl = malloc(sizeof(Line));
    nl->tiles = malloc(sizeof(tile)*LENGHT);
    for(int i = 0; i<LENGHT; i++){
        (nl->tiles[i]).is_block = false;
        (nl->tiles[i]).is_danger = false;
        (nl->tiles[i]).color = '\033[0m';
        (nl->tiles[i]).type = ' ';
    }
    if(prev != NULL){
        nl->next = next;
        (nl->next)->prev = nl;
    }
    if(next != NULL){
        nl->prev = prev;
        (nl->prev)->next = nl;
    }
    return nl;
}

Map* create_empty_map(void){
    Map* map = malloc(sizeof(Map));
    map->head = create_empty_line(NULL, NULL)
    Line* cur = map->head;
    for(int i = 1; i<HEIGHT; i++){
        cur = create_empty_line(cur, NULL);
    }
    map->tail = cur;
    return map;
}

bool carte_vide(Map* map){
    Line* cur = map 
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

bool coord_remplie(Map *carte,int x,int y){
    if(x<0 || x>LENGTH)
}

void ajout_tile(Map *carte,int x,int y,Tile tuile);//on modifie le dico, on en renvoie pas un nouveau et la tuile provenant de la pile, on la copie.
void suppression_tile(Map *carte,int x,int y);
int nb_tiles_remplies(Map *carte); //
Tile* tile_carte(Map *carte,int x,int y);
